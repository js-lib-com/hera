#include "LightSensor.h"
#include "MessagePublisher.h"
#include "Log.h"

const char* LightSensor::deviceClass = "js.hera.dev.LightSensor";

LightSensor::LightSensor(const char* deviceName, byte analogPort, int period, int threshold):
  Device(deviceClass, deviceName),
  analogPort(analogPort),
  timestamp(0),
  period(period),
  threshold(threshold),
  currentValue(0)
{
}

void LightSensor::setup() {
  Log::trace("LightSensor::setup");
}

void LightSensor::loop() {
  if (millis() - timestamp < period) {
    return;
  }

  int value = analogRead(analogPort);
  Log::debug("value: " + String(value));
  timestamp = millis();

  if (abs(currentValue - value) >= threshold) {
    currentValue = value;
    Log::debug("threshold detected");
    MessagePublisher::publishDeviceState(deviceName, currentValue);
  }
}

