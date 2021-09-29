#ifndef __HERA_DHT_SENSOR
#define __HERA_DHT_SENSOR

#include <DHT.h>
#include "VirtualDHTSensor.h"

class DHTSensor : public VirtualDHTSensor {
public:
	DHTSensor(const char* deviceName, byte port, byte type = DHT22, int period = 10, float humidityThreshold = 0.0, float temperatureThreshold = 0.0);

private:
	DHT sensor;
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

#endif // __HERA_DHT_SENSOR
