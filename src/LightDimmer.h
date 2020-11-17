#ifndef __HERA_LIGHT_DIMMER
#define __HERA_LIGHT_DIMMER

#include "Device.h"
#include "E2PROM.h"

class LightDimmer : public Device {
  public:
    LightDimmer(const char* deviceName, byte port, byte eepromAddr = NO_EEPROM);

    void setup();
    void loop();

  protected:
    String updateValue(const String& parameter);
    String setValue(const String& parameter);
    String getValue(const String& parameter);

  private:
    void update();

  private:
    // pwm port
    byte port;

    // pwm current value, initialized from EEPROM at setup
    byte value;

    // EEPROM address to persist this binary lights state
    byte eepromAddr;

  private:
    static const char* deviceClass;
    static Action metaActions[];
};

inline String LightDimmer::getValue(const String& parameter) {
  return String(value);
}

inline void LightDimmer::update() {
  analogWrite(port, 1023 * value / 255);
}

#endif // __HERA_LIGHT_DIMMER

