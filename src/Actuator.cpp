#include "Actuator.h"
#include "Log.h"

const char* Actuator::deviceClass = "js.hera.dev.Actuator";

Actuator::Actuator(const char* deviceName, byte port, OutMode outMode, byte eepromAddr):
  Device(deviceClass, deviceName),
  port(port, outMode),
  indicatorLED(0),
  ledOnColor(0),
  ledOffColor(0),
  eepromAddr(eepromAddr)
{
}

Actuator::Actuator(const char* deviceName, byte port, OutMode outMode, byte indicatorPort, uint32_t ledOnColor, uint32_t ledOffColor, byte eepromAddr):
  Device(deviceClass, deviceName),
  port(port, outMode),
  ledOnColor(ledOnColor),
  ledOffColor(ledOffColor),
  eepromAddr(eepromAddr)
{
  indicatorLED = new Adafruit_NeoPixel(1, indicatorPort, NEO_GRB + NEO_KHZ800);
}

void Actuator::setup() {
  Log::trace("Actuator::setup");
  port.setup();
  if (eepromAddr != NO_EEPROM) {
    Log::debug("Read device state from EEPROM: ", deviceName);
    port.setState(E2PROM::read(eepromAddr));
  }

  if (!indicatorLED) {
    return;
  }

  Log::debug("Initialize NeoPixel");
  indicatorLED->begin();
  indicatorLED->setPixelColor(0, port.getState() == 0 ? ledOffColor : ledOnColor);
  indicatorLED->show();
}

String Actuator::invoke(const String& action, const String& parameter) {
  Log::trace("Actuator::invoke");

  if (action == "turnON") {
    port.setState(1);
    update();
    return "";
  }
  else if (action == "turnOFF") {
    port.setState(0);
    update();
    return "";
  }
  else if (action == "toggle") {
    port.toggle();
    update();
    return "";
  }
  else if (action == "setState") {
    port.setState(parameter.toInt());
    update();
    return "";
  }
  else if (action == "getState") {
    return String(port.getState() ? "true" : "false");
  }

  return Device::invoke(action, parameter);
}

void Actuator::update() {
  if (eepromAddr != NO_EEPROM) {
    Log::debug("Write device state to EEPROM: ", deviceName);
    E2PROM::write(eepromAddr, port.getState());
  }

  MessagePublisher::publishDeviceState(deviceName, port.getState());
  if (indicatorLED) {
    indicatorLED->setPixelColor(0, port.getState() == 0 ? ledOffColor : ledOnColor);
    indicatorLED->show();
  }
}


