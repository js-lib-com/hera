#ifndef __HERA_OUT_PORT
#define __HERA_OUT_PORT

#include <Arduino.h>
#include "MessagePublisher.h"
#include "Port.h"

class OutPort {
  public:
    OutPort(byte port, PortMode mode);

    void setup();
    void setState(byte state);
    byte getState();
    void toggle();
    const char* toString();

  private:
    const byte port;
    const byte mask;
};

inline byte OutPort::getState() {
  return digitalRead(port) ^ mask;
}

inline void OutPort::toggle() {
  setState(!getState());
}

inline const char* OutPort::toString() {
  return getState() ? "true" : "false";
}

#endif


