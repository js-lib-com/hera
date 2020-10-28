#ifndef __HERA_DHT_SENSOR
#define __HERA_DHT_SENSOR

#include "DHT.h"
#include "Device.h"

class DHTSensor: public Device {
  public:
    DHTSensor(const char* deviceName, byte port, byte type = DHT22);

    void setup();
    void loop();
    String invoke(const String& action, const String& parameter = "");

  private:
    DHT sensor;

  private:
    static const char* deviceClass;
};

#endif // __HERA_DHT_SENSOR



