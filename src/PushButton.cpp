#include "PushButton.h"
#include "MessagePublisher.h"
#include "Log.h"

const char* PushButton::deviceClass = "js.hera.dev.PushButton"; 

PushButton::PushButton(const char* deviceName, byte port):
  Device(deviceName), switchPort(port)
{
}

void PushButton::setup() {
  Log::trace("PushButton::setup");
  switchPort.setup();
}

void PushButton::loop() {
  if (switchPort.pressed()) {
    Log::debug("Push button pressed");
    MessagePublisher::publishDeviceState(deviceName, 0);
  }
}

