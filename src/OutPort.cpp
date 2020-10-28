#include "OutPort.h"
#include "Log.h"

OutPort::OutPort(byte _port, OutMode _outMode) {
  port = _port;
  mask = _outMode == DIRECT ? 0 : 1;
}

void OutPort::setup() {
  Log::trace("OutPort::setup()");
  pinMode(port, OUTPUT);
  // for DIRECT port mask is 0 and for INVERSE mask is 1
  digitalWrite(port, mask);
}

void OutPort::setState(byte state) {
  digitalWrite(port, state ^ mask);
}

