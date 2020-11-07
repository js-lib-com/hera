#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>

#define SERVER_PORT 80
#define SERVICE_PROTO "tcp"

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

char packetBuffer[256]; //buffer to hold incoming packet

Device* nodevices[] = {};

void HERA::setup() {
  setup(nodevices, 0);
}

void HERA::setup(Device** _devices, byte _devicesCount) {
  devices = _devices;
  devicesCount = _devicesCount;

  Serial.begin(115200);
  Serial.println();
  Serial.println("HERA v1.0");
  Serial.println();

  MessagePublisher::init();
  Log::trace("HERA::setup");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Log::debug("Scanning for WiFi");
    delay(1000);
  }

  Log::debug("Connected to: ", WIFI_SSID);
  Log::debug("WiFi RSSI: ", String(WiFi.RSSI()));
  Log::debug("MAC address:", WiFi.macAddress());
  Log::debug("IP address: ", WiFi.localIP().toString());
  Log::debug("Host name: ", HOST_NAME);

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

  if (MDNS.begin(HOST_NAME, WiFi.localIP())) {
    Log::debug("MDNS started");
  }
  else {
    Log::error("Error setting up MDNS");
  }

  http.on("/subscribe.rmi", SubscribeHandler);
  http.on("/invoke.rmi", InvokeHandler);

  http.begin();
  Log::debug("HTTP server started");

  for (int i = 0; i < devicesCount; ++i) {
    devices[i]->setup();
    MDNS.addService(devices[i]->getName(), SERVICE_PROTO, SERVER_PORT);
  }
  Log::debug("Devices initialized");
}

void HERA::loop() {
  MDNS.update();
  ArduinoOTA.handle();
  http.handleClient();

  for (int i = 0; i < devicesCount; ++i) {
    devices[i]->loop();
  }
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

  sendServerError();
}

// --------------------------------------------------------------------------
// utility functions

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
