#include "RadioSwitch.h"
#include "Log.h"

#define TRIGGER_DELAY 2000

const char* RadioSwitch::deviceClass = "js.hera.dev.RadioSwitch";
Action RadioSwitch::metaActions[] = {
  ACTION("getState", &RadioSwitch::getState),
};

RadioSwitch::RadioSwitch(const char* deviceName, byte port, long unsigned int code, byte bits, byte protocol):
  Device(deviceClass, deviceName),
  targetAction(0),
  port(port),
  code(code),
  bits(bits),
  protocol(protocol)
{
  ctor();
}

RadioSwitch::RadioSwitch(const char* deviceName, const char* targetDeviceName, const char* targetActionName, byte port, long unsigned int code, byte bits, byte protocol):
  Device(deviceClass, deviceName),
  targetAction(new DeviceAction(targetDeviceName, targetActionName)),
  port(port),
  code(code),
  bits(bits),
  protocol(protocol)
{
  ctor();
}

void RadioSwitch::ctor() {
  actions = metaActions;
  actionsCount = sizeof(metaActions) / sizeof(metaActions[0]);
  timestamp = 0;
  pressedCounter = 0;
}

void RadioSwitch::setup()
{
  Log::trace("RadioSwitch::setup");
  rcSwitch.enableReceive(port);
}

void RadioSwitch::loop()
{
  if (!rcSwitch.available()) {
    return;
  }
  if (rcSwitch.getReceivedValue() == code &&
      rcSwitch.getReceivedBitlength() == bits &&
      rcSwitch.getReceivedProtocol() == protocol) {

    unsigned long now = millis();
    if (now > timestamp && (now - timestamp) < TRIGGER_DELAY) {
      return;
    }
    timestamp = now;

    Log::debug("Radio switch pressed.");
    if (targetAction != 0) {
      targetAction->exec();
    }
    ++pressedCounter;
    MessagePublisher::publishDeviceState(deviceName, 0);

    rcSwitch.resetAvailable();
  }
}

