#ifndef __HERA_BINARY_LIGHT
#define __HERA_BINARY_LIGHT

#include <Adafruit_NeoPixel.h>

#include "Device.h"
#include "InPort.h"
#include "OutPort.h"
#include "E2PROM.h"

class BinaryLight : public Device {
  public:
    BinaryLight(const char* deviceName, byte bulbPort, OutMode outMode, byte eepromAddr = NO_EEPROM);
    BinaryLight(const char* deviceName, byte switchPort, byte bulbPort, OutMode outMode, byte eepromAddr = NO_EEPROM);
    BinaryLight(const char* deviceName, byte switchPort, byte bulbPort, OutMode outMode, byte indicatorPort, uint32_t ledOnColor, uint32_t ledOffColor, byte eepromAddr = NO_EEPROM);

    void ctor();
    void setup();
    void loop();

  protected:
    String turnON(const String& parameter);
    String turnOFF(const String& parameter);
    String toggle(const String& parameter);
    String getState(const String& parameter);

  private:
    const char* update();

  private:
    // input port that reads the status of the wall switch
    InPort wallSwitch;

    // bulb output port
    OutPort bulb;

    // optional adressable LED for actuator state local indication; used only if indicator port is set on constructor
    Adafruit_NeoPixel* indicatorLED;

    // color used to signal ON state
    uint32_t ledOnColor;

    // color used to signal OFF state
    uint32_t ledOffColor;

    // EEPROM address to persist this binary lights state
    byte eepromAddr;

  private:
    static const char* deviceClass;
    static Action metaActions[];
};

inline String BinaryLight::turnON(const String& parameter) {
  bulb.setState(1);
  return update();
}

inline String BinaryLight::turnOFF(const String& parameter) {
  bulb.setState(0);
  return update();
}

inline String BinaryLight::toggle(const String& parameter) {
  bulb.toggle();
  return update();
}

inline String BinaryLight::getState(const String& parameter) {
  return bulb.toString();
}

#endif

