#include "SerialStepper.h"

Stepper::Stepper(StepperControl& control, byte unit)
  : clock_(now()) {
  control.addStepper(this, unit);
}


void Stepper::forward(bool fwd) {
  forward_ = fwd;
}

bool Stepper::forward() const {
  return forward_;
}

void Stepper::reverse() {
  reverse_ = true;
}

void Stepper::speed(float rpm) {
  if (rpm > 0.0f) {
    micros_pr_step_ = 60.0e+6f / (rpm * steps_pr_turn);
    Serial.println(micros_pr_step_, DEC);
  }
  else {
    micros_pr_step_ = 0;
  }
  clock_ = now();
}

void Stepper::steps(uint32_t stp) {
  continuous_ = false;
  remaining_ = stp;
}

uint32_t Stepper::steps() const {
  return remaining_;
}

void Stepper::move(StepperControl& control, byte unit) {
  if (remaining_ > 0) {
    if (now() - clock_ >= micros_pr_step_) {
      clock_ = now();
      if (reverse_) {
          forward_ = !forward_;
          reverse_ = false;
      }
      control.step(half_step ? hstep() : fstep(), unit);
      if (!continuous_) {
        --remaining_;
      }
    }
  }
  else if (micros_pr_step_ != 0) {
    control.step(0, unit);
    clock_ = now();
  }
}

byte Stepper::hstep() {
  current_ = (current_ + 8 + 1 - 2 * int(forward_)) % 8;
  return (hsteps_ >> (4 * current_)) & 0xf;
}

byte Stepper::fstep() {
  current_ = (current_ + 4 + 1 - 2 * int(forward_)) % 4;
  return (fsteps_ >> (4 * current_)) & 0xf;
}

bool Stepper::running() const {
  return remaining_ > 0;
}

void Stepper::turn(float turns) {
  if (turns > 0) {
    steps(uint32_t(float(steps_pr_turn) * turns));
    if (half_step) {
      remaining_ *= 2;
    }
  }
  else {
    steps(0);
  }
}

void Stepper::run() {
  remaining_ = 1;
  continuous_ = true;
}

void Stepper::stop() {
  remaining_ = 0;
}

LoopClock::Time Stepper::now() const {
  return LoopClock::now();
}


void StepperControl::tick() {
  doMoveSteppers();
  doTick();
}


void StepperControl::step(byte step, byte unit) {
  doStep(step, unit);
}


void StepperControl::addStepper(Stepper* stepper, byte unit) {
  if (stepper) {
    doAddStepper(stepper, unit);
  }
}


StepperControl::~StepperControl() {}
