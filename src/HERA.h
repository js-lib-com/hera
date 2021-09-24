#ifndef __HERA_FACADE
#define __HERA_FACADE

#include <IPAddress.h>

#include "Log.h"
#include "BinaryLight.h"
#include "LightDimmer.h"
#include "Actuator.h"
#include "ContactSwitch.h"
#include "PushButton.h"
#include "RadioSwitch.h"
#include "DHTSensor.h"
#include "ColorLED.h"
#include "NeoPixel.h"
#include "LightSensor.h"
#include "TemperatureSensor.h"
#include "Thermostat.h"
#include "ThermostatSensor.h"
#include "PowerMeter.h"
#include "NetSwitch.h"
#include "RollerBlinds.h"

extern const char* HOST_NAME;
extern const char* WIFI_SSID;
extern const char* WIFI_PASSWORD;

class HERA {
  public:
    void setup();
    void setup(Device** devices, byte devicesCount);
    void loop();

  public:
    void onSubscribe();
    void onInvoke();
    void onVersion();

  private:
    void advertise(IPAddress address, int port);
    void sendVoid();
    void sendResult(const String& result);
    void sendServerError(const String& error);

  private:
    Device** devices;
    byte devicesCount;
};

extern HERA hera;

#endif


