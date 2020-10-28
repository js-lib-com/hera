#ifndef __HERA_DEVICE_ACTION
#define __HERA_DEVICE_ACTION

#include <Arduino.h>
#include <ESP8266HTTPClient.h>

class DeviceAction {
  public:
    DeviceAction(const char* deviceName, const char* action);

    bool exec();
    bool exec(int argument);
    bool exec(float argument);
    bool exec(const String& argument);

    const String& value();
    int statusCode();
    bool error();
    void retry(int retries);

  private:
    bool invoke();
    bool _invoke();

  private:
    HTTPClient client;

    const char* deviceName;
    const char* action;

    IPAddress address;
    String argument;
    String resultValue;
    int statusCodeValue;

    int retries;
};

inline void DeviceAction::retry(int _retries) {
  retries = _retries;
}

inline bool DeviceAction::exec() {
  argument.remove(0);
  return invoke();
}

inline bool DeviceAction::exec(int _argument) {
  argument = String(_argument);
  return invoke();
}

inline bool DeviceAction::exec(float _argument) {
  argument = String(_argument);
  return invoke();
}

inline bool DeviceAction::exec(const String& _argument) {
  argument = _argument;
  return invoke();
}

inline const String& DeviceAction::value() {
  return resultValue;
}

inline int DeviceAction::statusCode() {
  return statusCodeValue;
}

inline bool DeviceAction::error() {
  return statusCodeValue < 200 || statusCodeValue >= 300;
}

#endif // __HERA_DEVICE_ACTION

