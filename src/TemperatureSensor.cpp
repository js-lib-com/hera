#include <Wire.h>
#include <ESP8266mDNS.h>

#include "TemperatureSensor.h"
#include "MessagePublisher.h"
#include "Log.h"

#define INVALID_TEMPERATURE -273.15

const char* TemperatureSensor::deviceClass = "js.hera.dev.TemperatureSensor";
Action TemperatureSensor::metaActions[] = {
  ACTION("getValue", &TemperatureSensor::getValue),
  ACTION("readValue", &TemperatureSensor::readValue),
};

TemperatureSensor::TemperatureSensor(const char* deviceName, byte sensorAddress, byte period, float threshold, float offset):
  TemperatureSensor(deviceClass, deviceName, sensorAddress, period, threshold, offset)
{
  ctor();
}

TemperatureSensor::TemperatureSensor(const char* deviceClass, const char* deviceName, byte sensorAddress, byte period, float threshold, float offset):
  Device(deviceClass, deviceName),
  sensorAddress(sensorAddress),
  period(1000 * period),
  threshold(threshold),
  offset(offset)
{
  compensatedTemperature = 0;
  timestamp = 0;
  ctor();
}

void TemperatureSensor::ctor() {
  actions = metaActions;
  actionsCount = sizeof(metaActions) / sizeof(metaActions[0]);
}

void TemperatureSensor::setup() {
  Log::trace("TemperatureSensor::setup");
  Wire.begin();
}

void TemperatureSensor::loop() {
  // do not read temperature sensor at every loop iteration to filter out quick changes
  if (millis() - timestamp < period) {
    return;
  }

  float temperature = readTemperature();
  timestamp = millis();
  if (temperature == INVALID_TEMPERATURE) {
    return;
  }

  if (fabs(compensatedTemperature - temperature) >= threshold) {
    compensatedTemperature = temperature;
    Log::debug("threshold detected: " + String(compensatedTemperature));
    publish(compensatedTemperature);
  }
}

void TemperatureSensor::publish(float temperature) {
  MessagePublisher::publishDeviceState(deviceName, temperature);
}

float TemperatureSensor::readTemperature() {
  if (Wire.requestFrom(sensorAddress, 2) != 2) {
    // Log::error("No data from sensor.");
    return INVALID_TEMPERATURE;
  }

  byte MSB = Wire.read();
  byte LSB = Wire.read();
  float temperature = (((MSB << 8) | LSB) >> 4) * 0.0625;
  Log::debug("temperature: " + String(temperature));
  temperature += offset;
  Log::debug("compensated temperature: " + String(temperature));
  return temperature;
}

