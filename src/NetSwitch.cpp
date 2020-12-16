#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include "NetSwitch.h"
#include "Log.h"

#define TARGET_ACTION "toggle"

const char* NetSwitch::deviceClass = "js.hera.dev.NetSwitch";
Action NetSwitch::metaActions[] = {
  ACTION("getState", &NetSwitch::getState),
};

NetSwitch::NetSwitch(const char* deviceName, const char* targetDeviceName, byte port):
  Device(deviceClass, deviceName), targetAction(targetDeviceName, TARGET_ACTION), switchPort(port)
{
  pressedCounter = 0;
  actions = metaActions;
  actionsCount = sizeof(metaActions) / sizeof(metaActions[0]);
}

void NetSwitch::setup() {
  Log::trace("NetSwitch::setup");
  switchPort.setup();
}

void NetSwitch::loop() {
  if (targetAction.error()) {
    Log::error("Target action error. Retry sending.");
    targetAction.exec();
    return;
  }

  if (switchPort.pressed()) {
    Log::debug("Net switch pressed.");
    targetAction.exec();
    ++pressedCounter;
    // MessagePublisher::publishDeviceState(deviceName, pressedCounter);
  }
}

