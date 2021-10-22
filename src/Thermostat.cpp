#include <Wire.h>
#include "E2PROM.h"
#include "Thermostat.h"
#include "DeviceAction.h"
#include "Log.h"

const char* Thermostat::deviceClass = "js.hera.dev.Thermostat";
Action Thermostat::metaActions[] = {
  ACTION("updateSetpoint", &Thermostat::updateSetpoint),
  ACTION("setSetpoint", &Thermostat::setSetpoint),
  ACTION("getSetpoint", &Thermostat::getSetpoint),
  ACTION("setTemperature", &Thermostat::setTemperature),
  ACTION("getTemperature", &Thermostat::getTemperature),
  ACTION("update", &Thermostat::update),
  ACTION("getState", &Thermostat::getState),
};

Thermostat::Thermostat(const char* deviceName, byte port, PortMode outMode, float hysteresis, byte eepromAddr):
  Device(deviceClass, deviceName),
  port(port, outMode),
  hysteresis(hysteresis),
  eepromAddr(eepromAddr)
{
  actions = metaActions;
  actionsCount = sizeof(metaActions) / sizeof(metaActions[0]);
}

void Thermostat::setup() {
  Log::trace("Thermostat::setup");

  port.setup();
  state = 0;

  Log::debug("Read device state from EEPROM: ", deviceName);
  E2PROM::get(eepromAddr, setpoint);
  if (setpoint < 16 || setpoint > 28) {
    setpoint = 20;
  }

  // initialize temperature sensor value
  DeviceAction sensorAction("thermostat-sensor", "getValue");
  sensorAction.retry(4);
  if (sensorAction.exec()) {
    temperature = sensorAction.value().toFloat();
  }
  else {
    // on fail, set initial temperature to high value to prevent activating port in transient state
    temperature = 28.0;
  }

  Log::debug("setpoint: ", setpoint);
  Log::debug("temperature: ", temperature);
}

String Thermostat::updateSetpoint(const String& parameter) {
  setSetpoint(parameter);
  return update(parameter);
}

String Thermostat::setSetpoint(const String& parameter) {
  setpoint = parameter.toFloat();
  Log::debug("Write device state to EEPROM: ", deviceName);
  E2PROM::put(eepromAddr, setpoint);
  return String(setpoint);
}

String Thermostat::getSetpoint(const String& parameter) {
  return String(setpoint);
}

String Thermostat::setTemperature(const String& parameter) {
  temperature = parameter.toFloat();
  updatePort();
  return String(temperature);
}

String Thermostat::getTemperature(const String& parameter) {
  return String(temperature);
}

String Thermostat::update(const String& parameter) {
  updatePort();
  String state;
  return updateState(state);
}

String Thermostat::getState(const String& parameter) {
  String state;
  return updateState(state);
}

void Thermostat::updatePort() {
  Log::trace("Thermostat::updatePort()");

  float currentSetpoint;
  switch (state) {
    case 0:
      currentSetpoint = setpoint - hysteresis;
      break;

    case 1:
      currentSetpoint = setpoint + hysteresis;
      break;
  }

  byte currentState = currentSetpoint > temperature ? 1 : 0;
  if (state != currentState) {
    state = currentState;
    Log::debug("port.setState: " + String(currentState));
    port.setState(currentState);
    MessagePublisher::publishDeviceState(deviceName, currentState);
  }
}

String& Thermostat::updateState(String& state) {
  state = "{\"setpoint\":";
  state += setpoint;
  state += ",\"hysteresis\":";
  state += hysteresis;
  state += ",\"temperature\":";
  state += temperature;
  state += ",\"running\":";
  state += (port.getState() ? "true" : "false");
  state += "}";
  return state;
}

