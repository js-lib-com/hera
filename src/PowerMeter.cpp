#include "MessagePublisher.h"
#include "PowerMeter.h"
#include "Log.h"

const char* PowerMeter::deviceClass = "js.hera.dev.PowerMeter";

PowerMeter* powerMeter;

ICACHE_RAM_ATTR void onPulseInterruptHandler() {
  static unsigned long last_interrupt_time = 0;

  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 100) {
    powerMeter->onPulse();
  }
  last_interrupt_time = interrupt_time;
}

PowerMeter::PowerMeter(const char* deviceName, const byte pin, const int ppwh):
  Device(deviceClass, deviceName),
  pin(pin),
  ppwh(ppwh)
{
  pulseCount = 0;
  pulseTime = 0;
  lastTime = 0;
}

void PowerMeter::setup()
{
  Log::trace("PowerMeter::setup");
  powerMeter = this;
  pinMode(pin, INPUT);
  attachInterrupt(digitalPinToInterrupt(pin), onPulseInterruptHandler, RISING);
}

void PowerMeter::loop()
{
  if (power != 0) {
    MessagePublisher::publishDeviceState(deviceName, power);
    power = 0;
  }
}

void PowerMeter::onPulse() {
  lastTime = pulseTime;
  pulseTime = micros();
  power = (3600000000.0 / (pulseTime - lastTime)) / ppwh;

  pulseCount++;
  energy = (1.0 * pulseCount / (ppwh * 1000));
}

