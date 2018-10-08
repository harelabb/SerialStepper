#include "arduinostepper.h"
#include <Arduino.h>

ArduinoStepperControl::
ArduinoStepperControl(byte pin1, byte pin2, byte pin3, byte pin4)
  : pin1_(pin1), pin2_(pin2), pin3_(pin3), pin4_(pin4) {}

void
ArduinoStepperControl::begin() {
  pinMode(pin1_, OUTPUT);
  pinMode(pin2_, OUTPUT);
  pinMode(pin3_, OUTPUT);
  pinMode(pin4_, OUTPUT);
}


void
ArduinoStepperControl::doTick() {
  if (status_ != old_status_) {
    old_status_ = status_;
    digitalWrite(pin1_, status_ & 1 ? HIGH : LOW);
    digitalWrite(pin2_, status_ & 2 ? HIGH : LOW);
    digitalWrite(pin3_, status_ & 4 ? HIGH : LOW);
    digitalWrite(pin4_, status_ & 8 ? HIGH : LOW);
  }
}

void
ArduinoStepperControl::doStep(byte step, byte) {
  status_ = step;
}

void
ArduinoStepperControl::doMoveSteppers() {
  if (stepper_) {
    stepper_->move(*this, 0);
  }
}

void
ArduinoStepperControl::doAddStepper(Stepper* stepper, byte unit) {
  if (unit == 0 && !stepper_) {
    stepper_ = stepper;
  }
}
