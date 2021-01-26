#include "LightDimmer.h"
#include "Log.h"

const char* LightDimmer::deviceClass = "js.hera.dev.LightDimmer";
Action LightDimmer::metaActions[] = {
  ACTION("updateValue", &LightDimmer::updateValue),
  ACTION("setValue", &LightDimmer::setValue),
  ACTION("getValue", &LightDimmer::getValue),
  ACTION("turnON", &LightDimmer::turnON),
  ACTION("turnOFF", &LightDimmer::turnOFF),
  ACTION("setState", &LightDimmer::setState),
  ACTION("getState", &LightDimmer::getState)
};

LightDimmer::LightDimmer(const char* deviceName, byte port, byte eepromAddr):
  Device(deviceClass, deviceName),
  port(port),
  value(0),
  active(false),
  eepromAddr(eepromAddr)
{
  actions = metaActions;
  actionsCount = sizeof(metaActions) / sizeof(metaActions[0]);
}

void LightDimmer::setup() {
  Log::trace("LightDimmer::setup");
  if (eepromAddr != NO_EEPROM) {
    value = E2PROM::read(eepromAddr);
    update();
  }
}

String LightDimmer::updateValue(const String& parameter) {
  value = parameter.toInt();
  update();
  return state();
}

String LightDimmer::setValue(const String& parameter) {
  value = parameter.toInt();
  if (eepromAddr != NO_EEPROM) {
    E2PROM::write(eepromAddr, value);
  }
  update();
  return state();
}

String LightDimmer::setState(const String& parameter) {
  active = parameter.toInt() == 1;
  update();
  return state();
}

void LightDimmer::update() {
  int pwd = active ? 1023 * value / 255 : 0;
  analogWrite(port, pwd);
}

