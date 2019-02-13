#include "SerialStepperControl.h"
#include "SerialStepper.h"

void StepperControl::run() {
  doMoveSteppers();
  doRun();
}


void StepperControl::addStepper(Stepper* stepper) {
  if (stepper) {
    doAddStepper(stepper);
  }
}

byte StepperControl::move(Stepper& stepper) const {
  return stepper.tick();
}

StepperControl::~StepperControl() {}
