#include "ContactSwitch.h"
#include "MessagePublisher.h"
#include "Log.h"

const char* ContactSwitch::deviceClass = "js.hera.dev.ContactSwitch";
Action ContactSwitch::metaActions[] = {
  ACTION("isOpened", &ContactSwitch::isOpened),
};

ContactSwitch::ContactSwitch(const char* deviceName, byte port):
  Device(deviceClass, deviceName),
  port(port)
{
  actions = metaActions;
  actionsCount = sizeof(metaActions) / sizeof(metaActions[0]);
}

void ContactSwitch::setup()
{
  Log::trace("ContactSwitch::setup");
  pinMode(port, INPUT_PULLUP);
  state = 0;
}

void ContactSwitch::loop()
{
  byte currentState = digitalRead(port);
  if (state != currentState) {
    state = currentState;
    MessagePublisher::publishDeviceState(deviceName, state);
  }
}

