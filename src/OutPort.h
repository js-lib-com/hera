#ifndef __HERA_OUT_PORT
#define __HERA_OUT_PORT

#include <Arduino.h>
#include "MessagePublisher.h"

enum OutMode {
  DIRECT, INVERSE
};

class OutPort {
  public:
    OutPort(byte port, OutMode outMode);

    void setup();
    void setState(byte state);
    byte getState();
    void toggle();

  private:
    byte port;
    byte mask;
};

inline byte OutPort::getState() {
  return digitalRead(port) ^ mask;
}

inline void OutPort::toggle() {
  setState(!getState());
}

#endif


