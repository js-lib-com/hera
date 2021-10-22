#ifndef __HERA_IN_PORT
#define __HERA_IN_PORT

#include <Arduino.h>
#include "Port.h"

class InPort {
  public:
    InPort(byte port = PORT_DISABLED);
    InPort(byte port, PortMode mode);

    void setup();
    bool pressed();

  private:
    const byte INACTIVE_LEVEL;
    const byte ACTIVE_LEVEL;

    const byte port;

    // keeps timestamp returned by millis() of the moment input was triggered
    unsigned long triggerTimestamp;

    // previous sample for input port value
    byte previousState;
};

#endif // __HERA_IN_PORT


