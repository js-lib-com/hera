#ifndef __HERA_NET_TEMPERATURE_SENSOR
#define __HERA_NET_TEMPERATURE_SENSOR

#include "Device.h"
#include "InPort.h"

class TemperatureSensor: public Device {
  public:
    TemperatureSensor(const char* deviceName, byte sensorAddress, byte period = 1, float threshold = 0, float offset = 0);

    void setup();
    virtual void loop();

  protected:
    TemperatureSensor(const char* deviceClass, const char* deviceName, byte sensorAddress, byte period, float threshold, float offset);
    virtual void publish(float temperature);

    String getValue(const String& parameter);
    String readValue(const String& parameter);

  private:
    float readTemperature();

  private:
    // I2C address for TMP102 device
    const byte sensorAddress;

    // temperature sensor reading period, in milliseconds
    const unsigned long period;

    // millis timestamp of the last successful sensor read
    unsigned long lastReadMillis;
    
    // temperature thresold to trigger compensated value compute and publish
    const float threshold;

    // signed offset added to measured value for ambient compensation
    const float offset;

    // temperature value compensated with configured offset
    float compensatedTemperature;

  private:
    static const char* deviceClass;
    static Action metaActions[];
};

inline String TemperatureSensor::getValue(const String& parameter) {
  return String(compensatedTemperature);
}

inline String TemperatureSensor::readValue(const String& parameter) {
  return String(readTemperature());
}

#endif

