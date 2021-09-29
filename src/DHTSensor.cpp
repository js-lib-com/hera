#include "DHTSensor.h"

const char* DHTSensor::deviceClass = "js.hera.dev.DHTSensor";

DHTSensor::DHTSensor(const char* deviceName, byte port, byte type, int period, float humidityThreshold, float temperatureThreshold) :
	VirtualDHTSensor(deviceClass, deviceName, period, humidityThreshold, temperatureThreshold),
	sensor(port, type)
{
}
