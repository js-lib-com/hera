#include "InPort.h"

#define TRIGGER_DELAY 500

InPort::InPort(byte port):
  port(port),
  INACTIVE_LEVEL(LOW),
  ACTIVE_LEVEL(HIGH)
{
  triggerTimestamp = 0;
  previousState = ACTIVE_LEVEL;
}

InPort::InPort(byte port, PortMode mode):
  port(port),
  INACTIVE_LEVEL(mode == DIRECT ? LOW : HIGH),
  ACTIVE_LEVEL(mode == DIRECT ? HIGH : LOW)
{
  triggerTimestamp = 0;
  previousState = ACTIVE_LEVEL;
}

void InPort::setup() {
  pinMode(port, INPUT_PULLUP);
}

bool InPort::pressed() {
  if (port == PORT_DISABLED) {
    return false;
  }
  unsigned long now = millis();

  // ignore input port transition on trigger delay period
  if ((now - triggerTimestamp) < TRIGGER_DELAY) {
    return false;
  }

  // current implementation does not use debounce delay

  bool returnValue = false;
  byte state = digitalRead(port);
  if (state == INACTIVE_LEVEL && previousState == ACTIVE_LEVEL) {
    // on falling edge set return value to true
    returnValue = true;
  }
  if (state == ACTIVE_LEVEL && previousState == INACTIVE_LEVEL) {
    // on rising edge trigger delay
    triggerTimestamp = now;
  }

  previousState = state;
  return returnValue;
}

