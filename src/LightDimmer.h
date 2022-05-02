#ifndef __HERA_LIGHT_DIMMER
#define __HERA_LIGHT_DIMMER

#include "Device.h"
#include "E2PROM.h"

class LightDimmer : public Device {
  public:
    LightDimmer(const char* deviceName, byte port, byte eepromAddr = NO_EEPROM);

    void setup();

  protected:
    String updateValue(const String& parameter);
    String setValue(const String& parameter);
    String getValue(const String& parameter);

    String turnON(const String& parameter);
    String turnOFF(const String& parameter);
    String setState(const String& parameter);
    String getState(const String& parameter);

  private:
    void update();

    // Return current device state format JSON
    String state();

  private:
    // pwm port
    byte port;

    // pwm current value, initialized from EEPROM at setup
    byte value;

    // on / off state, true if active
    boolean active;

    // EEPROM address to persist this binary lights state
    byte eepromAddr;

  private:
    static const char* deviceClass;
    static Action metaActions[];
};

inline String LightDimmer::getValue(const String& parameter) {
  return String(value);
}

inline String LightDimmer::turnON(const String& parameter) {
  return setState("1");
}

inline String LightDimmer::turnOFF(const String& parameter) {
  return setState("0");
}

inline String LightDimmer::getState(const String& parameter) {
  return state();
}

#endif // __HERA_LIGHT_DIMMER

