#ifndef __HERA_DHT12_SENSOR
#define __HERA_DHT12_SENSOR

#include <DHT12.h>
#include "VirtualDHTSensor.h"

class DHT12Sensor : public VirtualDHTSensor {
public:
	DHT12Sensor(const char* deviceName, int period = 10, float humidityThreshold = 0.0, float temperatureThreshold = 0.0);

private:
	DHT12 sensor;
	static const char* deviceClass;

protected:
	virtual void sensorBegin() {
		sensor.begin();
	}

	virtual float sensorHumidity() {
		return sensor.readHumidity();
	}

	virtual float sensorTemperature() {
		return sensor.readTemperature();
	}
};

#endif // __HERA_DHT12_SENSOR
