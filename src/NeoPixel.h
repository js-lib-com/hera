#ifndef __HERA_NEOPIXEL_LED
#define __HERA_NEOPIXEL_LED

#include <Adafruit_NeoPixel.h>

#include "Device.h"
#include "E2PROM.h"

class NeoPixel: public Device {
  public:
    NeoPixel(const char* deviceName, byte ledPort);

    void setup();

  protected:
    String setColor(const String& parameter);
    String getColor(const String& parameter);
    String setBrightness(const String& parameter);
    String getBrightness(const String& parameter);
    String turnON(const String& parameter);
    String turnOFF(const String& parameter);
    String setState(const String& parameter);
    String getState(const String& parameter);

  private:
    Adafruit_NeoPixel* led;

    byte red;
    byte green;
    byte blue;
    float brightness;
    
    boolean active;

  private:
    static const char* deviceClass;
    static Action metaActions[];
};

inline String NeoPixel::getColor(const String& parameter) {
  return String(red << 16 | green << 8 | blue);
}

inline String NeoPixel::getBrightness(const String& parameter) {
  return String(brightness);
}

inline String NeoPixel::turnON(const String& parameter) {
  return setState("1");
}

inline String NeoPixel::turnOFF(const String& parameter) {
  return setState("0");
}

inline String NeoPixel::getState(const String& parameter) {
  return active ? "1" : "0";
}

#endif
