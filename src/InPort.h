#ifndef __HERA_IN_PORT
#define __HERA_IN_PORT

#include <Arduino.h>

#define PORT_DISABLED -1

class InPort {
  public:
    InPort(byte port = PORT_DISABLED);

    void setup();
    bool pressed();

  private:
    byte port;

    // keeps timestamp returned by millis() of the moment input was triggered
    unsigned long triggerTimestamp;

    // previous sample for input port value
    byte previousState;
};

#endif

