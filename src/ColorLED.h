#ifndef __HERA_COLOR_LED
#define __HERA_COLOR_LED

#include "Device.h"
#include "E2PROM.h"

class ColorLED: public Device {
  public:
    ColorLED(const char* deviceName, byte redPort, byte greenPort, byte bluePort, byte eepromAddr = NO_EEPROM);

    void setup();
    String invoke(const String& action, const String& parameter = "");
    virtual const char* getDeviceClass() const;

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
};

inline const char* ColorLED::getDeviceClass() const {
  return deviceClass;
}
#endif
