#include "Device.h"

Device::Device(const char* deviceName):
  deviceName(deviceName)
{
  local = true;
}

Device::Device(const char* deviceClass, const char* deviceName):
  deviceName(deviceName)
{
  local = false;

  // {"deviceClass":"js.hera.dev.ContactSwitch","name":"front-door"}

  json = "{\"deviceClass\":\"";
  json += deviceClass;
  json += "\",\"name\":\"";
  json += deviceName;
  json += "\"}";
}

