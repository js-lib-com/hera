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
    int status();
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
    int statusCode;

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

inline int DeviceAction::status() {
  return statusCode;
}

inline bool DeviceAction::error() {
  return statusCode != 0 && (statusCode < 200 || statusCode >= 300);
}

#endif // __HERA_DEVICE_ACTION

