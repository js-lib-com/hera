#ifndef __HERA_THERMOSTAT
#define __HEAR_THERMOSTAT

#include "Device.h"
#include "OutPort.h"

class Thermostat: public Device {
  public:
    Thermostat(const char* deviceName, byte port, OutMode outMode, byte eepromAddr = 0);

    void setup();
    String invoke(const String& action, const String& parameter = "");

  private:
    void setSetpoint(float setpoint);
    void updatePort();

  private:
    // output port, driver for actuator
    OutPort port;

    // desired temperature value set from hub
    float setpoint;

    // current temperature value mesured by sensor
    float temperature;

    // output port current state
    byte state;

    // EEPROM address to persist this thermostat state
    const byte eepromAddr;

  private:
    static const char* deviceClass;
};

#endif // __HERA_THERMOSTAT

