#ifndef __HERA_DEVICE
#define __HERA_DEVICE

#include <Arduino.h>

class Device {
  public:
    /**
       Create a local device. A local device is not advertiseable and has no device class nor JSON representation.
       It is visible only on local host.

       @param deviceName device name.
    */
    Device(const char* deviceName);
    Device(const char* deviceClass, const char* deviceName);

    const char* getName() const;
    const bool isLocal() const;
    const char* JSON() const;

    virtual void setup();
    virtual void loop();
    virtual String invoke(const String& action, const String& parameter = "");

  protected:
    const char* deviceName;

    bool local;

    // JSON representation of this device instance
    String json;
};

inline const bool Device::isLocal() const {
  return local;
}

inline void Device::setup() {
}

inline void Device::loop() {
}

inline String Device::invoke(const String& action, const String& parameter) {
}

inline const char* Device::getName() const {
  return deviceName;
}

inline const char* Device::JSON() const {
  return json.c_str();
}

#endif

