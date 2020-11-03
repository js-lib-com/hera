#include <Wire.h>
#include "E2PROM.h"
#include "Thermostat.h"
#include "DeviceAction.h"
#include "Log.h"

const char* Thermostat::deviceClass = "js.hera.dev.Thermostat";

Thermostat::Thermostat(const char* deviceName, byte port, OutMode outMode, byte eepromAddr):
  Device(deviceClass, deviceName),
  port(port, outMode),
  eepromAddr(eepromAddr)
{
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

String Thermostat::invoke(const String& action, const String& parameter)
{
  Log::trace("Thermostat::invoke");

  if (action == "updateSetpoint") {
    setSetpoint(parameter.toFloat());
    updatePort();
  }
  else if (action == "setSetpoint")  {
    setSetpoint(parameter.toFloat());
  }
  else if (action == "getSetpoint") {
    return String(setpoint);
  }
  else if (action == "setTemperature") {
    temperature = parameter.toFloat();
    updatePort();
  }
  else if (action == "getTemperature") {
    return String(temperature);
  }
  else if (action == "update") {
    updatePort();
    String state;
    return updateState(state);
  }
  else if (action == "getState") {
    String state;
    return updateState(state);
  }

  return "";
}

void Thermostat::setSetpoint(float _setpoint) {
  setpoint = _setpoint;
  Log::debug("Write device state to EEPROM: ", deviceName);
  E2PROM::put(eepromAddr, setpoint);
}

void Thermostat::updatePort() {
  Log::trace("Thermostat::updatePort()");
  byte currentState = setpoint > temperature ? 1 : 0;
  if (state != currentState) {
    Log::debug("port.setState: " + String(currentState));
    port.setState(currentState);
    MessagePublisher::publishDeviceState(deviceName, port.getState());
    state = currentState;
  }
}

String& Thermostat::updateState(String& state) {
    state = "{\"setpoint\":";
    state += setpoint;
    state += ",\"temperature\":";
    state += temperature;
    state += ",\"running\":";
    state += (port.getState() ? "true" : "false");
    state += "}";
    return state;
}

