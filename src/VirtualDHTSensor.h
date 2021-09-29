#ifndef __HERA_VIRTUAL_DHT_SENSOR
#define __HERA_VIRTUAL_DHT_SENSOR

#include "Device.h"

class VirtualDHTSensor : public Device {
public:
	VirtualDHTSensor(const char* deviceClass, const char* deviceName, int period, float humidityThreshold, float temperatureThreshold);

	void setup();
	void loop();

protected:
	String getHumidity(const String& parameter);
	String getTemperature(const String& parameter);
	String getHeatIndex(const String& parameter);
	String getValue(const String& parameter);
	String getState(const String& parameter);

private:
	float readHumidity();
	float readTemperature();

protected:
	virtual void sensorBegin() = 0;
	virtual float sensorHumidity() = 0;
	virtual float sensorTemperature() = 0;

private:
	int period;
	long timestamp;
	float humidityThreshold;
	float temperatureThreshold;
	float humidity;
	float temperature;

private:
	static Action metaActions[];
};

inline float VirtualDHTSensor::readHumidity() {
	float humidity = sensorHumidity();
	return isnan(humidity) ? 0.0 : humidity;
}

inline float VirtualDHTSensor::readTemperature() {
	float temperature = sensorTemperature();
	return isnan(temperature) ? 0.0 : temperature;
}

inline String VirtualDHTSensor::getHumidity(const String& parameter) {
	return String(readHumidity());
}

inline String VirtualDHTSensor::getTemperature(const String& parameter) {
	return String(readTemperature());
}

#endif // __HERA_VIRTUAL_DHT_SENSOR
