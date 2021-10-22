#include "Actuator.h"
#include "Log.h"

const char* Actuator::deviceClass = "js.hera.dev.Actuator";
Action Actuator::metaActions[] = {
  ACTION("turnON", &Actuator::turnON),
  ACTION("turnOFF", &Actuator::turnOFF),
  ACTION("toggle", &Actuator::toggle),
  ACTION("pulse", &Actuator::pulse),
  ACTION("setState", &Actuator::setState),
  ACTION("getState", &Actuator::getState)
};

Actuator::Actuator(const char* deviceName, byte port, PortMode outMode, int pulseLength, byte eepromAddr):
  Device(deviceClass, deviceName),
  port(port, outMode),
  pulseLength(pulseLength),
  indicatorLED(0),
  ledOnColor(0),
  ledOffColor(0),
  eepromAddr(eepromAddr)
{
  ctor();
}

Actuator::Actuator(const char* deviceName, byte port, PortMode outMode, byte indicatorPort, uint32_t ledOnColor, uint32_t ledOffColor, byte eepromAddr):
  Device(deviceClass, deviceName),
  port(port, outMode),
  pulseLength(0),
  ledOnColor(ledOnColor),
  ledOffColor(ledOffColor),
  eepromAddr(eepromAddr)
{
  indicatorLED = new Adafruit_NeoPixel(1, indicatorPort, NEO_GRB + NEO_KHZ800);
  ctor();
}

void Actuator::ctor() {
  pulseStart = 0;
  actions = metaActions;
  actionsCount = sizeof(metaActions) / sizeof(metaActions[0]);
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

void Actuator::loop() {
  if (pulseStart > 0 && millis() - pulseStart >= pulseLength) {
    pulseStart = 0;
    port.setState(0);
  }
}

String Actuator::pulse(const String& parameter) {
  port.setState(1);
  pulseStart = millis();
  return "";
}

const char* Actuator::update() {
  if (eepromAddr != NO_EEPROM) {
    Log::debug("Write device state to EEPROM: ", deviceName);
    E2PROM::write(eepromAddr, port.getState());
  }

  MessagePublisher::publishDeviceState(deviceName, port.getState());
  if (indicatorLED) {
    indicatorLED->setPixelColor(0, port.getState() == 0 ? ledOffColor : ledOnColor);
    indicatorLED->show();
  }
  return port.toString();
}


