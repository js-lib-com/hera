#ifndef __HERA_THERMOSTAT
#define __HEAR_THERMOSTAT

#include "Device.h"
#include "OutPort.h"

class Thermostat: public Device {
  public:
    Thermostat(const char* deviceName, byte port, OutMode outMode, float hysteresis, byte eepromAddr = 0);
    void setup();

  protected:
    String updateSetpoint(const String& parameter);
    String setSetpoint(const String& parameter);
    String getSetpoint(const String& parameter);
    String setTemperature(const String& parameter);
    String getTemperature(const String& parameter);
    String update(const String& parameter);
    String getState(const String& parameter);

  private:
    void updatePort();
    String& updateState(String& state);

  private:
    // output port, driver for actuator
    OutPort port;

    // desired temperature value set from hub
    float setpoint;

    // current temperature value mesured by sensor
    float temperature;

    // output port current state
    byte state;

    float hysteresis;

    // EEPROM address to persist this thermostat state
    const byte eepromAddr;

  private:
    static const char* deviceClass;
    static Action metaActions[];
};

#endif // __HERA_THERMOSTAT

