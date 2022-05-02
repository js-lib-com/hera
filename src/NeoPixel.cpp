#include "NeoPixel.h"
#include "Log.h"

const char* NeoPixel::deviceClass = "js.hera.dev.NeoPixel";
Action NeoPixel::metaActions[] = {
  ACTION("setColor", &NeoPixel::setColor),
  ACTION("getColor", &NeoPixel::getColor),
  ACTION("setBrightness", &NeoPixel::setBrightness),
  ACTION("getBrightness", &NeoPixel::getBrightness),
  ACTION("turnON", &NeoPixel::turnON),
  ACTION("turnOFF", &NeoPixel::turnOFF),
  ACTION("setState", &NeoPixel::setState),
  ACTION("getState", &NeoPixel::getState)
};

NeoPixel::NeoPixel(const char* deviceName, byte ledPort):
  Device(deviceClass, deviceName),
  red(0),
  green(0),
  blue(0),
  brightness(1.0),
  active(true)
{
  led = new Adafruit_NeoPixel(1, ledPort, NEO_GRB + NEO_KHZ800);
  actions = metaActions;
  actionsCount = sizeof(metaActions) / sizeof(metaActions[0]);
}

void NeoPixel::setup() {
  Log::trace("NeoPixel::setup");
  Log::debug("Initialize NeoPixel");
  led->begin();
  led->show();
}

String NeoPixel::setColor(const String& parameter) {
  uint32_t color = parameter.toInt();
  red = (color >> 16) & 0xff;
  green = (color >> 8) & 0xff;
  blue = (color) & 0xff;
  if (active) {
    led->setPixelColor(0, red, green, blue);
    led->show();
  }
  return state();
}

String NeoPixel::setBrightness(const String& parameter) {
  brightness = parameter.toFloat();
  led->setBrightness(brightness);
  return state();
}

String NeoPixel::setState(const String& parameter) {
  active = parameter.toInt() == 1;
  if (active) {
    led->setPixelColor(0, red, green, blue);
    led->show();
  }
  else {
    led->setPixelColor(0, 0, 0, 0);
    led->show();
  }
  return state();
}

String NeoPixel::state() {
  String state = "{\"red\":";
  state += red;
  state += ",\"green\":";
  state += green;
  state += ",\"blue\":";
  state += blue;
  state += ",\"color\":";
  state += color();
  state += ",\"brightness\":";
  state += brightness;
  state += ",\"active\":";
  state += (active ? "true" : "false");
  state += "}";
  return state;
}

