#ifndef __HERA_NET_SWITCH
#define __HERA_NET_SWITCH

#include "Device.h"
#include "InPort.h"
#include "DeviceAction.h"

class NetSwitch: public Device {
  public:
    NetSwitch(const char* deviceName, const char* targetDeviceName, byte port);

    void setup();
    void loop();
    String invoke(const String& action, const String& parameter = "");

  private:
    DeviceAction targetAction; 
    InPort switchPort;
    int pressedCounter;

  private:
    static const char* deviceClass;
};

#endif

