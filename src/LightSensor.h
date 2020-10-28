#ifndef __HERA_LIGHT_SENSOR
#define __HERA_LIGHT_SENSOR

#include "Device.h"

class LightSensor: public Device {
  public:
    LightSensor(const char* deviceName, byte analogPort, int period, int threshold);

    void setup();
    void loop();

  private:
    byte analogPort;
    unsigned long timestamp;
    int period;
    int threshold;
    int currentValue;

  private:
    static const char* deviceClass;
};

#endif

