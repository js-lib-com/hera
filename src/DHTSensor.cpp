#include "DHTSensor.h"
#include "Log.h"

const char* DHTSensor::deviceClass = "js.hera.dev.DHTSensor";

DHTSensor::DHTSensor(const char* deviceName, byte port, byte type):
  Device(deviceClass, deviceName),
  sensor(port, type)
{
}

void DHTSensor::setup() {
  Log::trace("DHTSensor::setup");
  sensor.begin();
}

void DHTSensor::loop() {
}

String DHTSensor::invoke(const String& action, const String& parameter) {
  Log::trace("DHTSensor::invoke");

  if (action == "getValue") {
    float humidity = sensor.readHumidity();
    if (isnan(humidity)) {
      humidity = 0;
    }

    float temperature = sensor.readTemperature();
    if (isnan(temperature)) {
      temperature = 0;
    }

    String response;
    response += "{\"humidity\":";
    response += humidity;
    response += ",\"temperature\":";
    response += temperature;
    response += "}";
    return response;
  }
}

