#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define __MDNS__
#define __OTA__

#define WATCHDOG_TIMEOUT 8000
#define DISCOVERY_PORT 1902
#define SERVER_PORT 80
#define SERVICE_PROTO "tcp"

#ifdef __MDNS__
#include <ESP8266mDNS.h>
#endif

#ifdef __OTA__
#include <ArduinoOTA.h>
#endif

#include "RequestBody.h"
#include "MessagePublisher.h"
#include "Log.h"
#include "HERA.h"

HERA hera;

void SubscribeHandler() {
  hera.onSubscribe();
}

void InvokeHandler() {
  hera.onInvoke();
}

ESP8266WebServer http(SERVER_PORT);
WiFiUDP udp;

char packetBuffer[256]; //buffer to hold incoming packet

Device* nodevices[] = {};

void HERA::setup() {
  setup(nodevices, 0);
}

void HERA::setup(Device** _devices, byte _devicesCount) {
  devices = _devices;
  devicesCount = _devicesCount;

  Serial.begin(115200);
  MessagePublisher::enableSerial();
  Log::trace("HERA::setup");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Log::debug("Scanning for WiFi");
    delay(1000);
  }

  Log::debug("Connected to: ", WIFI_SSID);
  Log::debug("WiFi RSSI: ", String(WiFi.RSSI()));
  Log::debug("IP address: ", WiFi.localIP().toString());
  Log::debug("Host name: ", HOST_NAME);

#ifdef __OTA__
  ArduinoOTA.setHostname(HOST_NAME);

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
#endif

#ifdef __MDNS__
  if (MDNS.begin(HOST_NAME, WiFi.localIP())) {
    Log::debug("MDNS started");
  }
  else {
    Log::error("Error setting up MDNS");
  }
#endif

  udp.begin(DISCOVERY_PORT);
  Log::debug("Discovery service started");

  http.on("/js/hera/dev/HostSystem/subscribe.rmi", SubscribeHandler);
  http.on("/js/hera/dev/HostSystem/invoke.rmi", InvokeHandler);

  http.begin();
  Log::debug("HTTP server started");

  for (int i = 0; i < devicesCount; ++i) {
    devices[i]->setup();
#ifdef __MDNS__
    MDNS.addService(devices[i]->getName(), SERVICE_PROTO, SERVER_PORT);
#endif
  }
  Log::debug("Devices initialized");

  // it seems current ESP SDK does not support 'this network' broadcast, that is, all IPV4 addres bits on 1
  // so need to set to 1 only host part from local IP address
  // TODO patch SDK to support 'this network' broadcast as requested by HERA device contract
  // advertise(~uint32_t(WiFi.subnetMask()) | uint32_t(WiFi.gatewayIP()), DISCOVERY_PORT);
  advertise(~uint32_t(WiFi.subnetMask()) | uint32_t(WiFi.localIP()), DISCOVERY_PORT);
}

void HERA::loop() {
#ifdef __MDNS__
  MDNS.update();
#endif

#ifdef __OTA__
  ArduinoOTA.handle();
#endif

  http.handleClient();

  for (int i = 0; i < devicesCount; ++i) {
    devices[i]->loop();
  }

  // handle broadcast requests
  int packetSize = udp.parsePacket();
  if (!packetSize) {
    return;
  }

  // do not use Log::debug while processing datagram
  // if Log is configured to send log records to hub it will open nested HTTP transaction while inside UDP processing
  // this condition is not supported and HTTP transaction fails

  Log::debug("Received packet");
  Log::debug("Size: ", String(packetSize));
  Log::debug("IP: ", udp.remoteIP().toString());
  Log::debug("Port: ", String(udp.remotePort()));

  // read the packet into packetBufffer
  int len = udp.read(packetBuffer, 255);
  if (len > 0) {
    packetBuffer[len] = 0;
  }
  Log::debug("Datagram: ", packetBuffer);

  const String& packet(packetBuffer);
  if (packet.indexOf("js.hera.DeviceDiscoveryMessage") != -1) {
    // send a reply, to the IP address and port that sent us the packet we received
    advertise(udp.remoteIP(), udp.remotePort());
  }

  // ensure all packet bytes are read
  while (udp.read() != -1);
}


// --------------------------------------------------------------------------
// HTTP request handlers

void HERA::onSubscribe() {
  Log::trace("HERA::onSubscribe");

  // a little misterious and do not know if portable but it seems 'plain' argument is the HTTP body
  const String& requestBody = http.arg("plain");
  Log::debug(requestBody);

  // here requets body is JSON array with a single string, something like
  // ["http://192.168.1.3/hera/js/hera/DeviceEventListener/onDeviceEvent.rmi"]
  // trim leading and trailing quotes and rectangular parenthesis

  MessagePublisher::subscribe(requestBody.substring(2, requestBody.length() - 2));
  sendVoid();
}

void HERA::onInvoke() {
  Log::trace("HERA::onInvoke");

  // a little misterious and do not know if portable but it seems 'plain' argument is the HTTP body
  RequestBody requestBody(http.arg("plain"));
  Log::debug("Device name: ", requestBody.getDeviceName());
  Log::debug("Action name: ", requestBody.getActionName());
  Log::debug("Parameter  : ", requestBody.getParameter());

  // ["device-name","action-name","param_1","param_2",..."param_j"]

  // for now only one parameter
  // ["device-name","action-name","parameter"]

  for (int i = 0; i < devicesCount; ++i) {
    if (requestBody.getDeviceName() == devices[i]->getName()) {
      const String& result = devices[i]->invoke(requestBody.getActionName(), requestBody.getParameter());
      sendResult(result);
      return;
    }
  }

  // for unknown reasons HTTP request body is sometimes empty
  // strange enough at TCP link things seems corrent because this server error reach the client
  // it is observed only when client is wireless, more precise, NetSwitch device from another target host
  // current solution is to send server error and client to retry
  // anyway, this adds delay that is observed by user and that can be annoying

  // now, because server response reaches the client - it was recorded on client serial monitor
  // I can conclude HTTP transaction is complete and that TCP was able to compensate all lost packets
  // considering above, the only explanation for missing HTTP body is a bug in library
  // but I do not have evidence

  sendServerError();
}

// --------------------------------------------------------------------------
// utility functions

/**
  Advertise device descriptor on UDP.
*/
void HERA::advertise(IPAddress address, int port) {
  Log::trace("advertise");

  // {"class":"js.hera.HostAdvertise","hostName":"ESP01","hostURL":"http://192.168.1.71:80/","devices":[{"deviceClass":"js.hera.dev.Actuator","name":"living-room"},...]}

  udp.beginPacket(address, port);
  udp.write("{\"class\":\"js.hera.HostAdvertise\",\"hostName\":\"");
  udp.write(HOST_NAME);
  udp.write("\",\"hostURL\":\"http://");
  udp.write(WiFi.localIP().toString().c_str());
  udp.write(":80/\",\"devices\":[");

  bool first = true;
  for (int i = 0; i < devicesCount; ++i) {
    if (devices[i]->isLocal()) {
      continue;
    }
    // do not write comma before first device from array
    if (first) {
      first = false;
    }
    else {
      udp.write(",");
    }
    udp.write(devices[i]->JSON());
  }

  udp.write("]}");
  udp.endPacket();
}

void HERA::sendVoid() {
  Log::trace("sendVoid");
  http.setContentLength(0);
  http.send(204, "application/json");
}

void HERA::sendResult(const String& result) {
  Log::debug("Send results: ", result.c_str());
  http.setContentLength(result.length());
  if (result.length() == 0) {
    http.send(204, "application/json");
  }
  else {
    http.send(200, "application/json", result);
  }
}

void HERA::sendServerError() {
  Log::trace("sendServerError");
  http.setContentLength(0);
  http.send(500, "text/plain");
}
