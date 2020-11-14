#include "ContactSwitch.h"
#include "MessagePublisher.h"
#include "Log.h"

const char* ContactSwitch::deviceClass = "js.hera.dev.ContactSwitch";

ContactSwitch::ContactSwitch(const char* deviceName, byte port):
  Device(deviceClass, deviceName),
  port(port)
{
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

String ContactSwitch::invoke(const String& action, const String& parameter)
{
  if (action == "isOpened") {
    byte state = digitalRead(port);
    return String(state ? "true" : "false");
  }
  
  return Device::invoke(action, parameter);
}

