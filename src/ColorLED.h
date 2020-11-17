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

  private:
    void update();

  private:
    byte redPort;
    byte greenPort;
    byte bluePort;

    byte red;
    byte green;
    byte blue;

    // EEPROM address to persist this binary lights state
    byte eepromAddr;

  private:
    static const char* deviceClass;
    static Action metaActions[];
};

inline String ColorLED::getColor(const String& parameter) {
  return String((red & 0xff) << 16 | (green & 0xff) << 8 | (blue & 0xff));
}

#endif
