#include <ESP8266mDNS.h>

#include "DeviceAction.h"
#include "Log.h"

const String& INVOKE_PATH = "/js/hera/dev/HostSystem/invoke.rmi";

DeviceAction::DeviceAction(const char* _deviceName, const char* _action):
  deviceName(_deviceName),
  action(_action)
{
  client.setTimeout(2000);
  retries = 0;
  statusCode = 0;
}

bool DeviceAction::invoke() {
  for (++retries; retries > 0; --retries) {
    if (_invoke()) {
      return true;
    }
  }
  return false;
}

bool DeviceAction::_invoke() {
  Log::trace("DeviceAction::_invoke");

  if (!address) {
    Log::debug("Query service: ", deviceName);
    int n = MDNS.queryService(deviceName, "tcp");
    switch (n) {
      case 0:
        Log::debug("Device not found: ", deviceName);
        return false;

      case 1:
        address = MDNS.IP(0);
        Log::debug("Device IP: ", address.toString());
        break;

      default:
        Log::debug("Too many devices: ", deviceName);
        return false;
    }
  }

  String url("http://");
  url += address.toString();
  url += INVOKE_PATH;
  Log::debug("Device URL: " + url);

  String arguments("[\"");
  arguments += deviceName;
  arguments += "\",\"";
  arguments += action;
  arguments += "\"";
  if (argument.length()) {
    arguments += ",\"";
    arguments += argument;
    arguments += "\"";
  }
  arguments += "]";
  Log::debug("Device arguments: " + arguments);

  ESP.wdtDisable();
  if (!client.begin(url)) {
    Log::debug("Fail to init HTTP client. Force query service.");
    address = (uint32_t)0;
    ESP.wdtEnable(0);
    return false;
  }
  client.addHeader("Content-Type", "application/json", true);
  statusCode = client.POST(arguments);
  resultValue = client.getString();
  client.end();

  Log::debug("Status code: ", String(statusCode));
  ESP.wdtEnable(0);

  if (statusCode == 200) {
    return true;
  }
  if (statusCode == 204) {
    resultValue.remove(0);
    return true;
  }

  // reset address if bad status code in order to force query service
  // just in case device host IP address was changed
  Log::debug("Bad status code. Force query service.");
  address = (uint32_t)0;
  return false;
}

