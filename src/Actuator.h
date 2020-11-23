#ifndef __HERA_ACTUATOR
#define __HERA_ACTUATOR

#include <Adafruit_NeoPixel.h>

#include "Device.h"
#include "OutPort.h"
#include "E2PROM.h"

class Actuator: public Device {
  public:
    Actuator(const char* deviceName, byte port, OutMode outMode, int pulseLength = 4000, byte eepromAddr = NO_EEPROM);
    Actuator(const char* deviceName, byte port, OutMode outMode, byte indicatorPort, uint32_t ledOnColor, uint32_t ledOffColor, byte eepromAddr = NO_EEPROM);

    void ctor();
    void setup();
    void loop();

  protected:
    String turnON(const String& parameter);
    String turnOFF(const String& parameter);
    String toggle(const String& parameter);
    String pulse(const String& parameter);
    String setState(const String& parameter);
    String getState(const String& parameter);

  private:
    const char* update();

  private:
    // output port, driver for actuator
    OutPort port;

    // pulse length, in milliseconds; relevant only for pulse action
    const int pulseLength;

    unsigned long pulseStart;
    
    // optional adressable LED for actuator state local indication; used only if indicator port is set on constructor
    Adafruit_NeoPixel* indicatorLED;

    // color used to signal ON state
    uint32_t ledOnColor;

    // color used to signal OFF state
    uint32_t ledOffColor;

    // EEPROM address to persist this actuator state
    byte eepromAddr;

  private:
    static const char* deviceClass;
    static Action metaActions[];
};

inline String Actuator::turnON(const String& parameter) {
  port.setState(1);
  return update();
}

inline String Actuator::turnOFF(const String& parameter) {
  port.setState(0);
  return update();
}

inline String Actuator::toggle(const String& parameter) {
  port.toggle();
  return update();
}

inline String Actuator::setState(const String& parameter) {
  port.setState(parameter.toInt());
  return update();
}

inline String Actuator::getState(const String& parameter) {
  return port.toString();
}

#endif


