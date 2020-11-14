#include "RadioSwitch.h"
#include "Log.h"

#define TRIGGER_DELAY 2000

const char* RadioSwitch::deviceClass = "js.hera.dev.RadioSwitch";

RadioSwitch::RadioSwitch(const char* deviceName, byte port, long unsigned int code, byte bits, byte protocol):
  Device(deviceClass, deviceName),
  port(port),
  code(code),
  bits(bits),
  protocol(protocol)
{
  timestamp = 0;
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

    rcSwitch.resetAvailable();
    MessagePublisher::publishDeviceState(deviceName, 0);
  }
}


