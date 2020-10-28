#ifndef __HERA_REQUEST_BODY
#define __HERA_REQUEST_BODY

#include <Arduino.h>

class RequestBody {
  public:
    RequestBody(const String& body);

    const String& getDeviceName() const;
    const String& getActionName() const;
    const String& getParameter() const;

  private:
    String deviceName;
    String actionName;
    String parameter;

  private:
    enum State {
      WAIT_DEVICE_NAME, DEVICE_NAME, WAIT_ACTION_NAME, ACTION_NAME, WAIT_PARAM, PARAMETER
    };
};

inline const String& RequestBody::getDeviceName() const {
  return deviceName;
}

inline const String& RequestBody::getActionName() const {
  return actionName;
}

inline const String& RequestBody::getParameter() const {
  return parameter;
}

#endif


