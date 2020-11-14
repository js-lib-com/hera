#ifndef __HERA_POWER_METER
#define __HERA_POWER_METER

#include "Device.h"

class PowerMeter : public Device {
  public:
    PowerMeter(const char* deviceName, const byte pin, const int ppwh=1);

    void setup();
    void loop();

  public:
    void onPulse();

  private:
    const byte pin;
    const int ppwh;
    unsigned long pulseCount;
    unsigned long pulseTime;
    unsigned long lastTime;

    volatile double power;
    volatile double energy;

  private:
    static const char* deviceClass;
};

#endif

