#include <ESP8266WiFi.h>

#include "HERA.h"
#include "E2PROM.h"

//#define __THERMOSTAT__
//#define __THERMOSTAT_SENSOR__
//#define __ACTUATOR__
//#define __POWER_METER__
//#define __NET_SWITCH__
//#define __COLOR_LED__
//#define __DHT_SENSOR__
//#define __LIGHT_DIMMER__
//#define __RADIO_SWITCH__
#define __ROLLER_BLINDS__

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
#ifdef __LIGHT_DIMMER__
const char* HOST_NAME = "light-dimmer";
#endif
#ifdef __RADIO_SWITCH__
const char* HOST_NAME = "radio-switch";
#endif
#ifdef __ROLLER_BLINDS__
const char* HOST_NAME = "roller-blinds";
#endif

const char* WIFI_SSID = "DIR-825-A936";
const char* WIFI_PASSWORD = "mami1964";

Device* devices[] = {
#ifdef __THERMOSTAT__
  new Thermostat("thermostat", D2, INVERSE, 0.0, 0)
#endif

#ifdef __THERMOSTAT_SENSOR__
  new ThermostatSensor("thermostat-sensor", "thermostat", 72, 1, 0.1, -1.87)
#endif

#ifdef __ACTUATOR__
  new BinaryLight("binary-light-1", D1, D2, DIRECT),
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
  new Actuator("actuator-5", D4, INVERSE),
  new ColorLED("color-led", D3, D2, D1),
  new LightDimmer("light-dimmer", D7),
  new DHTSensor("dht-sensor", D5, DHT22, 30, 0.5, 0.2)
#endif

#ifdef __LIGHT_DIMMER__
  new LightDimmer("light-dimmer", 4)
#endif

#ifdef __RADIO_SWITCH__
  new RadioSwitch("radio-switch", 2, 0L)
#endif

#ifdef __ROLLER_BLINDS__
  new RollerBlinds("roller-blinds", D5, D6, D7, D8, LEFT)
#endif
};

void setup() {
  hera.setup(devices, sizeof(devices) / sizeof(Device*));
}

void loop() {
  hera.loop();
}

