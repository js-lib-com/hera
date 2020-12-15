#ifndef __HERA_DHT_SENSOR
#define __HERA_DHT_SENSOR

#include "DHT.h"
#include "Device.h"

class DHTSensor: public Device {
  public:
    DHTSensor(const char* deviceName, byte port, byte type = DHT22, int period = 0, float humidityThreshold = 0.0, float temperatureThreshold = 0.0);

    void setup();
    void loop();

  protected:
    String getHumidity(const String& parameter);
    String getTemperature(const String& parameter);
    String getValue(const String& parameter);

  private:
    float readHumidity();
    float readTemperature();

  private:
    DHT sensor;
    int period;
    long timestamp; 
    float humidityThreshold;
    float temperatureThreshold;
    float humidity;
    float temperature;

  private:
    static const char* deviceClass;
    static Action metaActions[];
};

inline float DHTSensor::readHumidity() {
  float humidity = sensor.readHumidity();
  return isnan(humidity) ? 0.0 : humidity;
}

inline float DHTSensor::readTemperature() {
  float temperature = sensor.readTemperature();
  return isnan(temperature) ? 0.0 : temperature;
}

inline String DHTSensor::getHumidity(const String& parameter) {
  return String(readHumidity());
}

inline String DHTSensor::getTemperature(const String& parameter) {
  return String(readTemperature());
}

#endif // __HERA_DHT_SENSOR

