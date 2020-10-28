#ifndef __HERA_NET_TEMPERATURE_SENSOR
#define __HERA_NET_TEMPERATURE_SENSOR

#include "Device.h"
#include "InPort.h"

class TemperatureSensor: public Device {
  public:
    TemperatureSensor(const char* deviceName, byte sensorAddress, byte period = 1, float threshold = 0, float offset = 0);

    void setup();
    virtual void loop();
    String invoke(const String& action, const String& parameter = "");

  protected:
    TemperatureSensor(const char* deviceClass, const char* deviceName, byte sensorAddress, byte period, float threshold, float offset);
    virtual void publish(float temperature);
    
  private:
    float readTemperature();

  private:
    // I2C address for TMP102 device
    const byte sensorAddress;

    // temperature sensor reading period, in seconds
    const long period;

    // temperature thresold to trigger compensated value compute and publish
    const float threshold;

    // signed offset added to measured value for ambient compensation
    const float offset;

    // hysteresis timeout timestamp
    long timestamp;

    // temperature value compensated with configured offset
    float compensatedTemperature;

  private:
    static const char* deviceClass;
};

#endif

