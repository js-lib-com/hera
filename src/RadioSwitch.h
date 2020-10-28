#ifndef __HERA_RADIO_SWITCH
#define __HERA_RADIO_SWITCH

#include <RCSwitch.h>
#include "Device.h"

class RadioSwitch: public Device {
  public:
    RadioSwitch(const char* deviceName, byte port, long unsigned int code, byte bits = 24, byte protocol = 1);

    void setup();
    void loop();
    String invoke(const String& action, const String& parameter = "");

  private:
    const byte port;
    const long unsigned int code;
    const byte bits;
    const byte protocol;

    RCSwitch rcSwitch;
    unsigned long timestamp;

  private:
    static const char* deviceClass;
};

#endif


