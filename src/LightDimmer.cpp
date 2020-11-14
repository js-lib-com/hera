#include "LightDimmer.h"
#include "Log.h"

const char* LightDimmer::deviceClass = "js.hera.dev.LightDimmer";

LightDimmer::LightDimmer(const char* deviceName, byte port, byte eepromAddr):
  Device(deviceClass, deviceName),
  port(port),
  value(0),
  eepromAddr(eepromAddr)
{
}

void LightDimmer::setup() {
  Log::trace("LightDimmer::setup");
  if (eepromAddr != NO_EEPROM) {
    value = E2PROM::read(eepromAddr);
  }
}

void LightDimmer::loop()
{
}

String LightDimmer::invoke(const String& action, const String& parameter)
{
  Log::trace("LightDimmer::invoke");

  if (action == "updateValue") {
    value = parameter.toInt();
    analogWrite(port, value);
  }
  else if (action == "setValue") {
    value = parameter.toInt();
    if (eepromAddr != NO_EEPROM) {
      E2PROM::write(eepromAddr, value);
    }
    analogWrite(port, value);
  }
  else if (action == "getValue") {
    return String(value);
  }
  
  return Device::invoke(action, parameter);
}

