#ifndef __HERA_MESSAGE_PUBLISHER
#define __HERA_MESSAGE_PUBLISHER

#include <Arduino.h>

class MessagePublisher {
  public:
    static void init();
    static void subscribe(const String& messageBrokerURL);
    
    static void publishDeviceState(const char* deviceName, float value);
    static void publishLogRecord(const char* level, const char* message);

  private:
    static void publish(const String& message);

  private:
    static bool serialEnabled;
    static String messageBrokerURL;
};

#endif

