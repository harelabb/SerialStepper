#include "SerialStepper.h"
#include "SerialStepperControl.h"

using loopClock::now;

namespace {
  byte fullStep(byte pos) {
    static constexpr uint16_t fsteps = 0b1001001101101100;
    return (fsteps >> (4 * pos)) & 0x0F;
  }
}

Stepper::Stepper(StepperControl& control)
  : clock_(now()) {
  control.addStepper(this);
}

void Stepper::start() {
  steps(infinite_);
}

void Stepper::stop() {
  steps(0);
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

void Stepper::steps(uint32_t nsteps) {
  steps_ = nsteps;
}

Stepper::Direction Stepper::direction() const {
  return direction_;
}

uint32_t Stepper::steps() const {
  return steps_;
}

void Stepper::turn(float turns) {
  if (turns > 0) {
    steps(uint32_t(float(steps_pr_turn_) * turns));
  }
  else {
    steps(0);
  }
}

byte Stepper::tick() {
  if (running()) {
    if (now() - clock_ >= micros_pr_step_) {
      clock_ += micros_pr_step_;
      if (reverse_) {
        reverse_ = false;
        direction_ = !direction_;
      }
      advance(direction_);
      if (steps_ != infinite_) {
        --steps_;
      }
    }
  }
  else  {
    clock_ = now();
  }
  return steps_ == 0 ? 0 : status_;
}


void Stepper::advance(Direction direction) {
  pos_ = (pos_ + 4 + 1 - 2 * int(direction)) % 4;
  status_ = fullStep(pos_);
}


bool Stepper::running() const {
  return steps_ != 0 && micros_pr_step_ != 0;
}
