#include "ThermostatSensor.h"
#include "Log.h"

#define CONTROLLER_ACTION "setTemperature"

const char* ThermostatSensor::deviceClass = "js.hera.dev.ThermostatSensor";

ThermostatSensor::ThermostatSensor(const char* deviceName, const char* controllerName, byte sensorAddress, byte period, float threshold, float offset):
  TemperatureSensor(deviceClass, deviceName, sensorAddress, period, threshold, offset),
  controllerAction(controllerName, CONTROLLER_ACTION)
{
}

void ThermostatSensor::loop() {
  TemperatureSensor::loop();
  if (controllerAction.error()) {
    Log::error("Controller action error. Retry sending.");
    controllerAction.exec(lastTemperature);
  }
}

void ThermostatSensor::publish(float temperature) {
  lastTemperature = temperature;
  controllerAction.exec(temperature);
}

