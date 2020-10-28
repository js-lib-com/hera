#ifndef __HERA_THERMOSTAT_SENSOR__
#define __HERA_THERMOSTAT_SENSOR__

#include "TemperatureSensor.h"
#include "DeviceAction.h"

class ThermostatSensor: public TemperatureSensor {
  public:
    ThermostatSensor(const char* deviceName, const char* controllerName, byte sensorAddress, byte period = 1, float threshold = 0, float offset = 0);

    virtual void loop();

  protected:
    virtual void publish(float temperature);

  private:
    DeviceAction controllerAction;
    float lastTemperature;

  private:
    static const char* deviceClass;
};

#endif // __HERA_THERMOSTAT_SENSOR__

