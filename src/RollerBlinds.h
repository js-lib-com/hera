#ifndef HERA_ROLLER_BLINDS
#define HERA_ROLLER_BLINDS

#include <AccelStepper.h>
#include "Device.h"
#include "E2PROM.h"

class RollerBlinds: public Device {
  public:
    RollerBlinds(const char* deviceName, byte pin1, byte pin2, byte pin3, byte pin4, byte eepromAddr = 0);
    void setup();
    void loop();

  protected:
    String open(const String& parameter);
    String close(const String& parameter);
    String position(const String& parameter);
    String state(const String& parameter);

    String move(const String& parameter);
    String stop(const String& parameter);
    String inverse(const String& parameter);
    String updateUpPosition(const String& parameter);
    String updateDownPosition(const String& parameter);

    String dump(const String& parameter);

  private:
    AccelStepper stepper;

    /// 1: clockwise, -1: anti-clockwise
    byte rotationSens;

    bool calibrationRequired;

    /// Manual moving steps. As long this value is non zero stepper keep moving.
    int movingSteps;

    /// The number of steps to roll down to maximum position.
    long downPosition;

    long lastKnownPosition;

    /// Open moving pending. Set to true when open is invoked and reseted to false when target is reached.
    byte openPending;

    byte eepromAddr;

  private:
    static const char* deviceClass;
    static Action metaActions[];
};

inline String RollerBlinds::close(const String& parameter) {
  open("0.0");
  return String(stepper.currentPosition());
}

inline String RollerBlinds::position(const String& parameter) {
  return String(stepper.currentPosition());
}

inline String RollerBlinds::state(const String& parameter) {
  return downPosition ? String(abs((double)stepper.currentPosition() / (double)downPosition)) : "1.0";
}

inline String RollerBlinds::move(const String& parameter) {
  movingSteps = parameter.toInt();
  return String(stepper.currentPosition());
}

inline String RollerBlinds::stop(const String& parameter) {
  movingSteps = 0;
  return String(stepper.currentPosition());
}

inline String RollerBlinds::inverse(const String& parameter) {
  rotationSens = parameter.toInt();
  return String(stepper.currentPosition());
}

inline String RollerBlinds::updateUpPosition(const String& parameter) {
  stepper.setCurrentPosition(0);
  return String(stepper.currentPosition());
}

#endif // HERA_ROLLER_BLINDS

