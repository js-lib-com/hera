#ifndef __HERA_LIGHT_DIMMER
#define __HERA_LIGHT_DIMMER

#include "Device.h"
#include "E2PROM.h"

class LightDimmer : public Device {
  public:
    LightDimmer(const char* deviceName, byte port, byte eepromAddr = NO_EEPROM);

    void setup();
    void loop();
    String invoke(const String& action, const String& parameter = "");

  private:
    // pwm port
    byte port;

    // pwm current value, initialized from EEPROM at setup
    byte value;

    // EEPROM address to persist this binary lights state
    byte eepromAddr;

  private:
    static const char* deviceClass;
};

#endif // __HERA_LIGHT_DIMMER

