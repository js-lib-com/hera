#ifndef __HERA_RADIO_SWITCH
#define __HERA_RADIO_SWITCH

#include <RCSwitch.h>
#include "Device.h"
#include "DeviceAction.h"

class RadioSwitch: public Device {
  public:
    RadioSwitch(const char* deviceName, byte port, long unsigned int code, byte bits = 24, byte protocol = 1);
    RadioSwitch(const char* deviceName, const char* targetDeviceName, const char* targetActionName, byte port, long unsigned int code, byte bits = 24, byte protocol = 1);
    void ctor();

    void setup();
    void loop();

  protected:
    String getState(const String& parameter);

  private:
    DeviceAction* targetAction;
    const byte port;
    const long unsigned int code;
    const byte bits;
    const byte protocol;

    RCSwitch rcSwitch;
    unsigned long timestamp;

    int pressedCounter;

  private:
    static const char* deviceClass;
    static Action metaActions[];
};

inline String RadioSwitch::getState(const String& parameter) {
  return String(pressedCounter);
}

#endif

