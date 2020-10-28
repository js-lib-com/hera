#ifndef __HERA_PUSH_BUTTON
#define __HERA_PUSH_BUTTON

#include "Device.h"
#include "InPort.h"

class PushButton: public Device {
  public:
    PushButton(const char* deviceName, byte port);

    void setup();
    void loop();

  private:
    InPort switchPort;

  private:
    static const char* deviceClass;
};

#endif // HERA_PUSH_BUTTON

