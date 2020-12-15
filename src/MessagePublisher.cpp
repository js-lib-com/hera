#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include "MessagePublisher.h"
#include "E2PROM.h"

bool MessagePublisher::serialEnabled;
String MessagePublisher::messageBrokerURL;

void MessagePublisher::init() {
  serialEnabled = true;
  E2PROM::read(MessagePublisher::messageBrokerURL);
  Serial.print("messageBrokerURL: ");
  Serial.println(messageBrokerURL);
}

void MessagePublisher::subscribe(const String& _messageBrokerURL) {
  if (messageBrokerURL != _messageBrokerURL) {
    messageBrokerURL = _messageBrokerURL;
    Serial.print("Update message broker URL: ");
    Serial.println(messageBrokerURL);
    E2PROM::write(messageBrokerURL);
  }
}

void MessagePublisher::publishDeviceState(const char* deviceName, const char* valueName, float value) {
  if (!messageBrokerURL.length()) {
    return;
  }
  if (serialEnabled) {
    // do not use logger since it depends on this publisher
    Serial.println("Send device state event");
    Serial.print("- device name: ");
    Serial.println(deviceName);
    Serial.print("- ");
    Serial.print(valueName);
    Serial.print(": ");
    Serial.println(value);
  }

  // message has a type and a string value that is a JSON object, in this case device state object
  // {"type":"DEVICE_STATE","value":"{"deviceName":"kitchen-light","value":"1"}"}

  String deviceState = "[{\"type\":\"DEVICE_STATE\",\"value\":\"{\\\"deviceName\\\":\\\"";
  deviceState += deviceName;
  deviceState += "\\\",\\\"value\\\":";
  deviceState += value;
  deviceState += "}\"}]";

  publish(deviceState);
}

void MessagePublisher::publishLogRecord(const char* level, const char* message) {
  if (serialEnabled) {
    Serial.print(level);
    Serial.print(": ");
    Serial.print(millis());
    Serial.print(": ");
    Serial.println(message);
    return;
  }
  if (!messageBrokerURL.length()) {
    return;
  }

  // message has a type and a string value that is a JSON object, in this case device state object
  // {"type":"LOG_RECORD","value":"{"level":"DEBUG","message":"Log message."}"}

  String logRecord = "[{\"type\":\"LOG_RECORD\",\"value\":\"{\\\"level\\\":\\\"";
  logRecord += level;
  logRecord += "\\\",\\\"message\\\":\\\"";
  logRecord += message;
  logRecord += "\\\"}\"}]";

  publish(logRecord);
}

void MessagePublisher::publish(const String& message) {
  HTTPClient client;
  client.begin(messageBrokerURL);
  client.addHeader("Content-Type", "application/json", true);
  int statusCode = client.POST(message);
  client.end();

  if (statusCode != 204) {
    if (serialEnabled) {
      Serial.println("Publish error: " + String(statusCode));
    }
  }
}

