#include "DHTSensor.h"
#include "Log.h"

const char* DHTSensor::deviceClass = "js.hera.dev.DHTSensor";
Action DHTSensor::metaActions[] = {
  ACTION("getHumidity", &DHTSensor::getHumidity),
  ACTION("getTemperature", &DHTSensor::getTemperature),
  ACTION("getValue", &DHTSensor::getValue),
};

DHTSensor::DHTSensor(const char* deviceName, byte port, byte type):
  Device(deviceClass, deviceName),
  sensor(port, type)
{
  actions = metaActions;
  actionsCount = sizeof(metaActions) / sizeof(metaActions[0]);
}

void DHTSensor::setup() {
  Log::trace("DHTSensor::setup");
  sensor.begin();
}

String DHTSensor::getHumidity(const String& parameter) {
  float humidity = sensor.readHumidity();
  if (isnan(humidity)) {
    humidity = 0;
  }
  return String(humidity);
}

String DHTSensor::getTemperature(const String& parameter) {
  float temperature = sensor.readTemperature();
  if (isnan(temperature)) {
    temperature = 0;
  }
  return String(temperature);
}

String DHTSensor::getValue(const String& parameter) {
  String response;
  response += "{\"humidity\":";
  response += getHumidity(parameter);
  response += ",\"temperature\":";
  response += getTemperature(parameter);
  response += "}";
  return response;
}

