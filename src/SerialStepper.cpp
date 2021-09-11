#include "SerialStepper.h"

using loopClock::microsNow;

namespace {
  constexpr byte full_step[] = {0b1001, 0b0011, 0b0110, 0b1100};
}

Stepper::Stepper(uint32_t steps_pr_turn)
  : steps_pr_turn_(steps_pr_turn),
    clock_(microsNow()) {}

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
  clock_ = microsNow();
}

float Stepper::speed() const {
  return micros_pr_step_ == 0 ? 0.0f :
    60.0e+6f / (micros_pr_step_ * steps_pr_turn_);
}

void Stepper::direction(Direction dir) {
  direction_ = dir;
}

void Stepper::steps(uint32_t nsteps) {
  steps_ = nsteps;
  clock_ = microsNow();
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
    if (loopClock::Micros(microsNow() - clock_) >= micros_pr_step_) {
      clock_ += micros_pr_step_;
      advance(direction_);
      if (steps_ != infinite_) {
        --steps_;
      }
    }
  }
  return steps_ == 0 ? 0 : full_step[pos_];
}

void Stepper::advance(Direction direction) {
  pos_ = (pos_ + (direction == FORWARD ? 1 : 3)) % 4;
}


bool Stepper::running() const {
  return steps_ != 0 && micros_pr_step_ != 0;
}
