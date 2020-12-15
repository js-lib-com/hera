#include "DHTSensor.h"
#include "Log.h"

#define INVALID_HUMIDITY 0.0
#define INVALID_TEMPERATURE -273.15

const char* DHTSensor::deviceClass = "js.hera.dev.DHTSensor";
Action DHTSensor::metaActions[] = {
  ACTION("getHumidity", &DHTSensor::getHumidity),
  ACTION("getTemperature", &DHTSensor::getTemperature),
  ACTION("getValue", &DHTSensor::getValue),
};

DHTSensor::DHTSensor(const char* deviceName, byte port, byte type, int period, float humidityThreshold, float temperatureThreshold):
  Device(deviceClass, deviceName),
  sensor(port, type),
  period(1000 * period),
  humidityThreshold(humidityThreshold),
  temperatureThreshold(temperatureThreshold)
{
  actions = metaActions;
  actionsCount = sizeof(metaActions) / sizeof(metaActions[0]);
  timestamp = 0;
  humidity = INVALID_HUMIDITY;
  temperature = INVALID_TEMPERATURE;

}

void DHTSensor::setup() {
  Log::trace("DHTSensor::setup");
  sensor.begin();
}

void DHTSensor::loop() {
  if (millis() - timestamp < period) {
    return;
  }
  timestamp = millis();

  float sensorHumidity = readHumidity();
  if (fabs(sensorHumidity - humidity) >= humidityThreshold) {
    humidity = sensorHumidity;
    MessagePublisher::publishDeviceState(deviceName, "humidity", humidity);
  }

  float sensorTemperature = readTemperature();
  if (fabs(sensorTemperature - temperature) >= temperatureThreshold) {
    temperature = sensorTemperature;
    //MessagePublisher::publishDeviceState(deviceName, "temperature", temperature);
  }
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

