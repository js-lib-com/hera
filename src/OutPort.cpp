#include "OutPort.h"
#include "Log.h"

OutPort::OutPort(byte port, PortMode mode):
  port(port),
  mask(mode == DIRECT ? 0 : 1)
{
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

