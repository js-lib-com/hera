#include "BinaryLight.h"
#include "MessagePublisher.h"
#include "Log.h"

const char* BinaryLight::deviceClass = "js.hera.dev.BinaryLight";

BinaryLight::BinaryLight(const char* deviceName, byte bulbPort, OutMode outMode, byte eepromAddr):
  Device(deviceClass, deviceName),
  bulb(bulbPort, outMode),
  indicatorLED(0),
  ledOnColor(0),
  ledOffColor(0),
  eepromAddr(eepromAddr)
{
}

BinaryLight::BinaryLight(const char* deviceName, byte switchPort, byte bulbPort, OutMode outMode, byte eepromAddr):
  Device(deviceClass, deviceName),
  wallSwitch(switchPort),
  bulb(bulbPort, outMode),
  indicatorLED(0),
  ledOnColor(0),
  ledOffColor(0),
  eepromAddr(eepromAddr)
{
}

BinaryLight::BinaryLight(const char* deviceName, byte switchPort, byte bulbPort, OutMode outMode, byte indicatorPort, uint32_t ledOnColor, uint32_t ledOffColor, byte eepromAddr):
  Device(deviceClass, deviceName),
  wallSwitch(switchPort),
  bulb(bulbPort, outMode),
  ledOnColor(ledOnColor),
  ledOffColor(ledOffColor),
  eepromAddr(eepromAddr)
{
  indicatorLED = new Adafruit_NeoPixel(1, indicatorPort, NEO_GRB + NEO_KHZ800);
}

void BinaryLight::setup() {
  Log::trace("BinaryLight::setup");
  wallSwitch.setup();
  bulb.setup();

  if (eepromAddr != NO_EEPROM) {
    Log::debug("Read device state from EEPROM: ", deviceName);
    bulb.setState(E2PROM::read(eepromAddr));
  }
  if (!indicatorLED) {
    return;
  }

  Log::debug("Initialize NeoPixel");
  indicatorLED->begin();
  indicatorLED->setPixelColor(0, ledOffColor);
  indicatorLED->show();
}

void BinaryLight::loop()
{
  if (wallSwitch.pressed()) {
    bulb.toggle();
    update();
  }
}

String BinaryLight::invoke(const String& action, const String& parameter)
{
  Log::trace("BinaryLight::invoke");

  if (action == "turnON") {
    bulb.setState(1);
    update();
  }
  else if (action == "turnOFF") {
    bulb.setState(0);
    update();
  }
  else if (action == "toggle") {
    bulb.toggle();
    update();
  }
  else if (action == "getState") {
    return String(bulb.getState() ? "true" : "false");
  }
  return "";
}

void BinaryLight::update() {
  if (eepromAddr != NO_EEPROM) {
    Log::debug("Write device state to EEPROM: ", deviceName);
    E2PROM::write(eepromAddr, bulb.getState());
  }

  MessagePublisher::publishDeviceState(deviceName, bulb.getState());
  if (indicatorLED) {
    indicatorLED->setPixelColor(0, bulb.getState() == 0 ? ledOffColor : ledOnColor);
    indicatorLED->show();
  }
}

