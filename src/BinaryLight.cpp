#include "BinaryLight.h"
#include "MessagePublisher.h"
#include "Log.h"

const char* BinaryLight::deviceClass = "js.hera.dev.BinaryLight";
Action BinaryLight::metaActions[] = {
  ACTION("turnON", &BinaryLight::turnON),
  ACTION("turnOFF", &BinaryLight::turnOFF),
  ACTION("toggle", &BinaryLight::toggle),
  ACTION("getState", &BinaryLight::getState)
};

BinaryLight::BinaryLight(const char* deviceName, byte bulbPort, OutMode outMode, byte eepromAddr):
  Device(deviceClass, deviceName),
  bulb(bulbPort, outMode),
  indicatorLED(0),
  ledOnColor(0),
  ledOffColor(0),
  eepromAddr(eepromAddr)
{
  ctor();
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
  ctor();
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
  ctor();
}

void BinaryLight::ctor() {
  actions = metaActions;
  actionsCount = sizeof(metaActions) / sizeof(metaActions[0]);
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

const char* BinaryLight::update() {
  if (eepromAddr != NO_EEPROM) {
    Log::debug("Write device state to EEPROM: ", deviceName);
    E2PROM::write(eepromAddr, bulb.getState());
  }

  MessagePublisher::publishDeviceState(deviceName, bulb.getState());
  if (indicatorLED) {
    indicatorLED->setPixelColor(0, bulb.getState() == 0 ? ledOffColor : ledOnColor);
    indicatorLED->show();
  }
  return bulb.toString();
}

