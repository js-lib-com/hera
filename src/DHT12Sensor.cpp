#include "DHT12Sensor.h"

const char* DHT12Sensor::deviceClass = "js.hera.dev.DHT12Sensor";

DHT12Sensor::DHT12Sensor(const char* deviceName, int period, float humidityThreshold, float temperatureThreshold) :
	VirtualDHTSensor(deviceClass, deviceName, period, humidityThreshold, temperatureThreshold)
{
}
