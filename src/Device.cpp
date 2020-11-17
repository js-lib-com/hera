#include "Device.h"
#include "Log.h"

Device::Device(const char* deviceName):
  deviceName(deviceName)
{
  local = true;
}

Device::Device(const char* deviceClass, const char* deviceName):
  deviceName(deviceName)
{
  local = false;
}

String Device::invoke(const String& action, const String& parameter) {
  Log::trace("Device::invoke(const String&,const String&)");

  for (int i = 0; i < actionsCount; ++i) {
    if (action == actions[i].name) {
      Log::debug("Execute ", action);
      return (this->*actions[i].handler)(parameter);
    }
  }

  if (action == "getActions") {
    return getActions(parameter);
  }

  return String("Action not found: ") + action;
}

String Device::getActions(const String& parameter) {
  String json;
  json += '[';
  for (int i = 0; i < actionsCount; ++i) {
    if (i > 0) {
      json += ',';
    }
    json += '"';
    json += actions[i].name;
    json += '"';
  }
  json += ']';
  return json;
}

