#include "VirtualDHTSensor.h"
#include "Log.h"

#define INVALID_HUMIDITY 0.0
#define INVALID_TEMPERATURE -273.15

Action VirtualDHTSensor::metaActions[] = {
  ACTION("getHumidity", &VirtualDHTSensor::getHumidity),
  ACTION("getTemperature", &VirtualDHTSensor::getTemperature),
  ACTION("getHeatIndex", &VirtualDHTSensor::getHeatIndex),
  ACTION("getValue", &VirtualDHTSensor::getValue),
  ACTION("getState", &VirtualDHTSensor::getState)
};

VirtualDHTSensor::VirtualDHTSensor(const char* deviceClass, const char* deviceName, int period, float humidityThreshold, float temperatureThreshold) :
	Device(deviceClass, deviceName),
	period(1000 * period),
	humidityThreshold(humidityThreshold),
	temperatureThreshold(temperatureThreshold)
{
	actions = metaActions;
	actionsCount = sizeof(metaActions) / sizeof(metaActions[0]);
	timestamp = 0;
	humidity = INVALID_HUMIDITY;
	temperature = INVALID_TEMPERATURE;

}

void VirtualDHTSensor::setup() {
	Log::trace("VirtualDHTSensor::setup");
	sensorBegin();
}

void VirtualDHTSensor::loop() {
	if (millis() - timestamp < period) {
		return;
	}
	timestamp = millis();

	float sensorHumidity = readHumidity();
	if (fabs(sensorHumidity - humidity) >= humidityThreshold) {
		humidity = sensorHumidity;
		MessagePublisher::publishDeviceState(deviceName, "humidity", humidity);
	}

	float sensorTemperature = readTemperature();
	if (fabs(sensorTemperature - temperature) >= temperatureThreshold) {
		temperature = sensorTemperature;
		//MessagePublisher::publishDeviceState(deviceName, "temperature", temperature);
	}
}

String VirtualDHTSensor::getValue(const String& parameter) {
	String response;
	response += "{\"humidity\":";
	response += getHumidity(parameter);
	response += ",\"temperature\":";
	response += getTemperature(parameter);
	response += ",\"heatIndex\":";
	response += getHeatIndex(parameter);
	response += "}";
	return response;
}

String VirtualDHTSensor::getState(const String& parameter) {
	String response;
	response += "{\"period\":";
	response += period;
	response += ",\"timestamp\":";
	response += timestamp;
	response += ",\"humidityThreshold\":";
	response += humidityThreshold;
	response += ",\"temperatureThreshold\":";
	response += temperatureThreshold;
	response += ",\"humidity\":";
	response += humidity;
	response += ",\"temperature\":";
	response += temperature;
	response += "}";
	return response;
}

String VirtualDHTSensor::getHeatIndex(const String& parameter)
{
	// temperature is Celcius but this algorithm is based on Fahrenheit
	const float t = temperature * 1.8 + 32;
	const float h = humidity;

	float heatIndex = 0.5 * (t + 61.0 + ((t - 68.0) * 1.2) + (h * 0.094));

	if (heatIndex > 79) {
		heatIndex = -42.379 +
			2.04901523 * t + 10.14333127 * h +
			-0.22475541 * t * h +
			-0.00683783 * pow(t, 2) +
			-0.05481717 * pow(h, 2) +
			0.00122874 * pow(t, 2) * h +
			0.00085282 * t * pow(h, 2) +
			-0.00000199 * pow(t, 2) * pow(h, 2);

		if ((h < 13) && (t >= 80.0) && (t <= 112.0)) {
			heatIndex -= ((13.0 - h) * 0.25) * sqrt((17.0 - abs(t - 95.0)) * 0.05882);
		}
		else if ((h > 85.0) && (t >= 80.0) && (t <= 87.0)) {
			heatIndex += ((h - 85.0) * 0.1) * ((87.0 - t) * 0.2);
		}
	}

	return String((heatIndex - 32) * 0.55555);
}

