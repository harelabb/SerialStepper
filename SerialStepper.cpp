#include "SerialStepper.h"

using loopClock::now;

Stepper::Stepper(StepperControl& control)
  : clock_(now()) {
  control.addStepper(this);
}

void Stepper::start() {
  remaining_ = forever_;
}

void Stepper::stop() {
  remaining_ = 0;
}

void Stepper::speed(float rpm) {
  if (rpm > 0.0f) {
    micros_pr_step_ = 60.0e+6f / (rpm * steps_pr_turn_);
  }
  else {
    micros_pr_step_ = 0;
  }
  clock_ = now();
}

void Stepper::direction(Direction dir) {
  direction_ = dir;
}

void Stepper::reverse() {
  reverse_ = true;
}

void Stepper::step(uint32_t steps) {
  remaining_ = steps;
}

Stepper::Direction Stepper::direction() const {
  return direction_;
}

uint32_t Stepper::remaining() const {
  return remaining_;
}

void Stepper::turn(float turns) {
  if (turns > 0) {
    step(uint32_t(float(steps_pr_turn_) * turns));
  }
  else {
    step(0);
  }
}

void Stepper::move(StepperControl& control, byte unit) {
  if (micros_pr_step_ != 0 && remaining_ > 0) {
    if (now() - clock_ >= micros_pr_step_) {
      clock_ += micros_pr_step_;
      if (reverse_) {
        reverse_ = false;
        direction_ = !direction_;
      }
      advance();
      control.step(fullStep(), unit);
      if (remaining_ != forever_) {
        --remaining_;
      }
    }
  }
  else {
    control.step(0, unit);
    clock_ = now();
  }
}

void Stepper::advance() {
   current_ = (current_ + 4 + 1 - 2 * int(direction_)) % 4;
}

byte Stepper::fullStep() const {
  static constexpr int32_t fsteps = 0b1001001101101100;
  return (fsteps >> (4 * current_)) & 0x0F;
}

bool Stepper::running() const {
  return remaining_ > 0;
}

void StepperControl::run() {
  doMoveSteppers();
  doRun();
}


void StepperControl::step(byte step, byte unit) {
  doStep(step, unit);
}


void StepperControl::addStepper(Stepper* stepper) {
  if (stepper) {
    doAddStepper(stepper);
  }
}


StepperControl::~StepperControl() {}
