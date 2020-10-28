#include "InPort.h"

#define TRIGGER_DELAY 500

InPort::InPort(byte port): port(port) {
  triggerTimestamp = 0;
  previousState = HIGH;
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
  if (state == LOW && previousState == HIGH) {
    // on falling edge set return value to true
    returnValue = true;
  }
  // on rising edge trigger delay
  if (state == HIGH && previousState == LOW) {
    triggerTimestamp = now;
  }

  previousState = state;
  return returnValue;
}


