#ifndef __HERA_CONTACT_SWITCH
#define __HERA_CONTACT_SWITCH

#include "Device.h"
#include "InPort.h"

class ContactSwitch: public Device {
  public:
    ContactSwitch(const char* deviceName, byte port);

    void setup();
    void loop();

  protected:
    String isOpened(const String& parameter);

  private:
    // input port that reads the status of the wall switch
    byte port;

    // last known state
    byte state;

  private:
    static const char* deviceClass;
    static Action metaActions[];
};

inline String ContactSwitch::isOpened(const String& parameter) {
  return digitalRead(port) ? "true" : "false";
}

#endif


