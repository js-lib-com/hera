#ifndef __HERA_DHT_SENSOR
#define __HERA_DHT_SENSOR

#include "DHT.h"
#include "Device.h"

class DHTSensor: public Device {
  public:
    DHTSensor(const char* deviceName, byte port, byte type = DHT22);

    void setup();

  protected:
    String getHumidity(const String& parameter);
    String getTemperature(const String& parameter);
    String getValue(const String& parameter);

  private:
    DHT sensor;

  private:
    static const char* deviceClass;
    static Action metaActions[];
};

#endif // __HERA_DHT_SENSOR



