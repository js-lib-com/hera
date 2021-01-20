#include "RollerBlinds.h"
#include "Log.h"

const char* RollerBlinds::deviceClass = "js.hera.dev.RollerBlinds";
Action RollerBlinds::metaActions[] = {
  ACTION("open", &RollerBlinds::open),
  ACTION("close", &RollerBlinds::close),
  ACTION("position", &RollerBlinds::position),
  ACTION("move", &RollerBlinds::move),
  ACTION("stop", &RollerBlinds::stop),
  ACTION("updateUpPosition", &RollerBlinds::updateUpPosition),
  ACTION("updateDownPosition", &RollerBlinds::updateDownPosition)
};

RollerBlinds::RollerBlinds(const char* deviceName, byte pin1, byte pin2, byte pin3, byte pin4):
  Device(deviceClass, deviceName),
  stepper(4, pin1, pin2, pin3, pin4),
  movingSteps(0)
{
  actions = metaActions;
  actionsCount = sizeof(metaActions) / sizeof(metaActions[0]);
}

void RollerBlinds::setup() {
  Log::trace("RollerBlinds::setup");
  stepper.setMaxSpeed(500);
  stepper.setAcceleration(50);
}

void RollerBlinds::loop() {
  if (movingSteps != 0) {
    // movingSteps are non zero only on roller calibration when stepper is moved manually to maximum up and down positions
    stepper.move(movingSteps);
    stepper.runSpeedToPosition();
  }
  else {
    stepper.run();
  }
}

