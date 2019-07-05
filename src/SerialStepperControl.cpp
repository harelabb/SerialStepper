#include "SerialStepperControl.h"
#include "SerialStepper.h"

byte StepperControl::tickStepper(Stepper& stepper) {
  return stepper.tick();
}

bool StepperControl::run() {
  return doRun();
}

void StepperControl::begin() {
  doBegin();
}

void StepperControl::addStepper(Stepper& stepper) {
  doAddStepper(stepper);
}
