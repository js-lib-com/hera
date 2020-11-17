#ifndef __HERA_DEVICE
#define __HERA_DEVICE

#include <Arduino.h>

class Device;
typedef String(Device::*ActionHandler)(const String&);

typedef struct {
  const char* name;
  ActionHandler handler;
} Action;

#define ACTION(name,handler) {name,(ActionHandler)handler}

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

    virtual void setup();
    virtual void loop();
    virtual String invoke(const String& action, const String& parameter = "");

  private:
    String getActions(const String& parameter);

  protected:
    const char* deviceName;
    Action* actions;
    int actionsCount;
    bool local;
};

inline const bool Device::isLocal() const {
  return local;
}

inline void Device::setup() {
}

inline void Device::loop() {
}

inline const char* Device::getName() const {
  return deviceName;
}

#endif

