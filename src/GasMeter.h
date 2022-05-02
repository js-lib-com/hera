#ifndef __HERA_GAS_METER
#define __HERA_GAS_METER

#include "Device.h"

class GasMeter : public Device {
  public:
    GasMeter(const char* deviceName, const byte pin);

    void setup();
    void loop();

    String setValue(const String& parameter);
    String getValue(const String& parameter);

  public:
    void onPulse();

  private:
    const byte pin;
    volatile float value;
    volatile bool dirty;

  private:
    static const char* deviceClass;
    static Action metaActions[];
};

inline String GasMeter::setValue(const String& parameter) {
  value = parameter.toFloat();
  return String(value);
}

inline String GasMeter::getValue(const String& parameter) {
  return String(value);
}

#endif // __HERA_GAS_METER

