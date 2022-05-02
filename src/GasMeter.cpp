#include "MessagePublisher.h"
#include "GasMeter.h"
#include "Log.h"

const char* GasMeter::deviceClass = "js.hera.dev.GasMeter";
Action GasMeter::metaActions[] = {
  ACTION("setValue", &GasMeter::setValue),
  ACTION("getValue", &GasMeter::getValue)
};

GasMeter* gasMeter;

ICACHE_RAM_ATTR void onGasInterruptHandler() {
  static unsigned long last_interrupt_time = 0;

  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 100) {
    gasMeter->onPulse();
  }
  last_interrupt_time = interrupt_time;
}

GasMeter::GasMeter(const char* deviceName, const byte pin):
  Device(deviceClass, deviceName),
  pin(pin),
  value(0.0),
  dirty(false)
{
  actions = metaActions;
  actionsCount = sizeof(metaActions) / sizeof(metaActions[0]);
}

void GasMeter::setup()
{
  Log::trace("GasMeter::setup");
  gasMeter = this;
  pinMode(pin, INPUT);
  attachInterrupt(digitalPinToInterrupt(pin), onGasInterruptHandler, RISING);
}

void GasMeter::loop()
{
  if (dirty) {
    MessagePublisher::publishDeviceState(deviceName, value);
    dirty = false;
  }
}

void GasMeter::onPulse() {
  value += 0.01;
  dirty = true;
}

