#ifndef __HERA_COLOR_LED
#define __HERA_COLOR_LED

#include "Device.h"
#include "E2PROM.h"

class ColorLED: public Device {
  public:
    ColorLED(const char* deviceName, byte redPort, byte greenPort, byte bluePort, byte eepromAddr = NO_EEPROM);

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
    void update();
    int pwm(byte colorComponent);

  private:
    byte redPort;
    byte greenPort;
    byte bluePort;

    byte red;
    byte green;
    byte blue;
    float brightness;
    boolean active;

    // EEPROM address to persist this binary lights state
    byte eepromAddr;

  private:
    static const char* deviceClass;
    static Action metaActions[];
};

inline String ColorLED::getColor(const String& parameter) {
  return String(red << 16 | green << 8 | blue);
}

inline String ColorLED::getBrightness(const String& parameter) {
  return String(brightness);
}

inline String ColorLED::turnON(const String& parameter) {
  return setState("1");
}

inline String ColorLED::turnOFF(const String& parameter) {
  return setState("0");
}

inline String ColorLED::getState(const String& parameter) {
  return active ? "1" : "0";
}

inline int ColorLED::pwm(byte colorComponent) {
  return active ? brightness * (1023 * colorComponent / 255) : 0;
}

#endif
