#include <ESP8266WiFi.h>

#include "HERA.h"
#include "DHT.h"

#define __THERMOSTAT__
//#define __THERMOSTAT_SENSOR__
//#define __ACTUATOR__
//#define __ACTION__
//#define __POWER_METER__
//#define __NET_SWITCH__
//#define __COLOR_LED__
//#define __DHT_SENSOR__

#ifdef __THERMOSTAT__
const char* HOST_NAME = "thermostat";
#endif
#ifdef __THERMOSTAT_SENSOR__
const char* HOST_NAME = "thermostat-sensor";
#endif
#ifdef __ACTUATOR__
const char* HOST_NAME = "actuators";
#endif
#ifdef __POWER_METER__
const char* HOST_NAME = "power-meter";
#endif
#ifdef __NET_SWITCH__
const char* HOST_NAME = "net-switch";
#endif
#ifdef __COLOR_LED__
const char* HOST_NAME = "color-led";
#endif
#ifdef __DHT_SENSOR__
const char* HOST_NAME = "dht-sensor";
#endif

const char* WIFI_SSID = "DIR-825-A936";
const char* WIFI_PASSWORD = "mami1964";

Device* devices[] = {
#ifdef __THERMOSTAT__
  new Thermostat("thermostat", D2, INVERSE, 0.2, 0)
#endif

#ifdef __THERMOSTAT_SENSOR__
  new ThermostatSensor("thermostat-sensor", "thermostat", 72, 1, 0.1, -1.87)
#endif

#ifdef __ACTUATOR__
  new Actuator("actuator-1", D1, DIRECT),
  new Actuator("actuator-2", D2, DIRECT),
  new Actuator("actuator-3", D3, DIRECT),
  new Actuator("actuator-4", D4, INVERSE)
#endif

#ifdef __POWER_METER__
  new PowerMeter("power-meter", D4)
#endif

#ifdef __NET_SWITCH__
  new NetSwitch("net-switch", "actuator-4", D4)
#endif

#ifdef __COLOR_LED__
  new ColorLED("color-led", 2, 3, 4)
#endif

#ifdef __DHT_SENSOR__
  new Actuator("actuator-5", D6, INVERSE),
  new ColorLED("color-led", D4, D2, D1),
  new Actuator("actuator-6", D7, INVERSE),
  new DHTSensor("dht-sensor", D5)
#endif
};

#ifdef __ACTION__
#include "DeviceAction.h"
DeviceAction action("thermostat-sensor", "getValue");
int counter;
#endif

#ifdef __DHT_SENSOR__
//DHT sensor(D2, DHT22);
int readings;
int errors;
#endif

void setup() {
  hera.setup(devices, sizeof(devices) / sizeof(Device*));

#ifdef __DHT_SENSOR__
  readings = 0;
  errors = 0;
  //sensor.begin();
#endif
}

void loop() {
  hera.loop();
#ifdef __ACTION__
  if (action.exec()) {
    Log::debug(action.value());
  }
  Serial.println(counter++);
#endif

#ifdef __DHT_SENSOR__
  if (millis() < 4000) {
    return;
  }

  /*
    const String& humidity = devices[0]->invoke("getHumidity");
    if (humidity.toFloat() == 0.0) {
      ++errors;
    }
    const String& temperature = devices[0]->invoke("getTemperature");
    if (temperature.toFloat() == 0.0) {
      ++errors;
    }

    Serial.print(++readings);
    Serial.print(" : ");
    Serial.print(errors);
    Serial.print(" : ");
    Serial.print(humidity);
    Serial.print(" : ");
    Serial.println(temperature);
  */
#endif
}

