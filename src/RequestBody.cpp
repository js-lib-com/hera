#include "RequestBody.h"
#include "Log.h"

RequestBody::RequestBody(const String& body)
{
  Log::debug("Request body: ", body);

  // ["device-name","action-name","parameter"]

  State state = WAIT_DEVICE_NAME;
  bool done = false;

  for (int i = 0; i < body.length() && !done; ++i) {
    char c  = body.charAt(i);

    switch (state) {
      case WAIT_DEVICE_NAME:
        if (c == '"') {
          state = DEVICE_NAME;
        }
        break;

      case DEVICE_NAME:
        if (c == '"') {
          state = WAIT_ACTION_NAME;
        }
        else {
          deviceName += c;
        }
        break;

      case WAIT_ACTION_NAME:
        if (c == '"') {
          state = ACTION_NAME;
        }
        break;

      case ACTION_NAME:
        if (c == '"') {
          state = WAIT_PARAM;
        }
        else {
          actionName += c;
        }
        break;

      case WAIT_PARAM:
        if (c == '"') {
          state = PARAMETER;
        }
        break;

      case PARAMETER:
        if (c == '"') {
          done = true;
        }
        else {
          parameter += c;
        }
        break;
    }
  }
}


