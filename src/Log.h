#ifndef __HERA_LOG
#define __HERA_LOG

#include "MessagePublisher.h"

#define TRACE "TRACE"
#define ERROR "ERROR"
#define DEBUG "DEBUG"

class Log {
  public:
    static void trace(const String& message);
    static void error(const String& message);
    static void debug(const String& message);
    static void debug(const char* message, const String& parameter);
    static void debug(const char* message, float parameter);
};

inline void Log::trace(const String& message) {
  MessagePublisher::publishLogRecord(TRACE, message.c_str());
}

inline void Log::error(const String& message) {
  MessagePublisher::publishLogRecord(ERROR, message.c_str());
}

inline void Log::debug(const String& message) {
  MessagePublisher::publishLogRecord(DEBUG, message.c_str());
}

inline void Log::debug(const char* message, const String& parameter) {
  String formattedMessage = message;
  formattedMessage += parameter;
  MessagePublisher::publishLogRecord(DEBUG, formattedMessage.c_str());
}

inline void Log::debug(const char* message, float parameter) {
  Log::debug(message, String(parameter));
}

#endif


