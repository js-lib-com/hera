#include <ESP8266WiFi.h>

#include "HERA.h"

//#define __SENSOR__
//#define __THERMOSTAT__
//#define __ACTUATOR__
//#define __ACTION__
//#define __POWER_METER__
#define __NET_SWITCH__

#ifdef __SENSOR__
const char* HOST_NAME = "thermostat-sensor";
#endif
#ifdef __THERMOSTAT__
const char* HOST_NAME = "thermostat";
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

const char* WIFI_SSID = "DIR-825-A936";
const char* WIFI_PASSWORD = "mami1964";

Device* devices[] = {
#ifdef __SENSOR__
  new ThermostatSensor("thermostat-sensor", "thermostat", 72, 1, 0.1, -1.87)
#endif

#ifdef __ACTUATOR__
  new Actuator("actuator-1", D1, DIRECT),
  new Actuator("actuator-2", D2, DIRECT),
  new Actuator("actuator-3", D3, DIRECT),
  new Actuator("actuator-4", D4, INVERSE)
#endif

#ifdef __THERMOSTAT__
  //  new Actuator("thermostat-actuator", D2, DIRECT)
  new Thermostat("thermostat", D2, INVERSE)
#endif

#ifdef __POWER_METER__
  new PowerMeter("power-meter", 4)
#endif

#ifdef __NET_SWITCH__
  new NetSwitch("net-switch", "actuator-4", D4)
#endif
};

#ifdef __ACTION__
#include "DeviceAction.h"
DeviceAction action("thermostat-sensor", "getValue");
int counter;
#endif

void setup() {
  hera.setup(devices, sizeof(devices) / sizeof(Device*));
}

void loop() {
  hera.loop();
#ifdef __ACTION__
  if (action.exec()) {
    Log::debug(action.value());
  }
  Serial.println(counter++);
#endif
}

