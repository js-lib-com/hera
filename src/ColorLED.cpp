#include "ColorLED.h"
#include "Log.h"

#define ADDR_RED 0
#define ADDR_GREEN 1
#define ADDR_BLUE 2

const char* ColorLED::deviceClass = "js.hera.dev.ColorLED";

ColorLED::ColorLED(const char* deviceName, byte redPort, byte greenPort, byte bluePort, byte eepromAddr):
  Device(deviceClass, deviceName),
  redPort(redPort),
  greenPort(greenPort),
  bluePort(bluePort),
  red(0),
  green(0),
  blue(0),
  eepromAddr(eepromAddr)
{
}

void ColorLED::setup() {
  Log::trace("ColorLED::setup");
  pinMode(redPort, OUTPUT);
  pinMode(greenPort, OUTPUT);
  pinMode(bluePort, OUTPUT);

  if (eepromAddr != NO_EEPROM) {
    Log::debug("Read device state from EEPROM: ", deviceName);
    red = E2PROM::read(eepromAddr + ADDR_RED);
    green = E2PROM::read(eepromAddr + ADDR_GREEN);
    blue = E2PROM::read(eepromAddr + ADDR_BLUE);
    update();
  }
}

String ColorLED::invoke(const String& action, const String& parameter) {
  Log::trace("ColorLED::invoke");
  if (action == "setColor") {
    uint32_t color = parameter.toInt();

    red = (color >> 16) & 0xff;
    green = (color >> 8) & 0xff;
    blue = (color) & 0xff;

    if (eepromAddr != NO_EEPROM) {
      Log::debug("Write device state to EEPROM: ", deviceName);
      E2PROM::write(eepromAddr + ADDR_RED, red);
      E2PROM::write(eepromAddr + ADDR_GREEN, green);
      E2PROM::write(eepromAddr + ADDR_BLUE, blue);
    }

    update();
    return "";
  }
  else if (action == "getColor") {
    return String((red & 0xff) << 16 | (green & 0xff) << 8 | (blue & 0xff));
  }

  return Device::invoke(action, parameter);
}

void ColorLED::update() {
  analogWrite(redPort, 1023 * red / 255);
  analogWrite(greenPort, 1023 * green / 255);
  analogWrite(bluePort, 1023 * blue / 255);
}

