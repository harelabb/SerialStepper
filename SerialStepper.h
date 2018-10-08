#ifndef STEPPER_H
#define STEPPER_H

#include <Arduino.h>
#include "loopclock.h"

class StepperControl;

class Stepper {
public:
  explicit Stepper(StepperControl& control, byte unit);
  void forward(bool fwd);
  bool forward() const;
  void reverse();
  void speed(float rpm);
  void steps(uint32_t stp);
  uint32_t steps() const;
  void turn(float turns);
  void stop();
  void move(StepperControl& control, byte unit);
  bool running() const;
  void run();

  Stepper& operator=(const Stepper&) = delete;
  Stepper(const Stepper&) = delete;
  Stepper(Stepper&&) = delete;

  bool half_step = false;
  uint32_t steps_pr_turn = 2048;
private:
  byte hstep();
  byte fstep();
  LoopClock::Time now() const;

  static constexpr int32_t fsteps_ =
    0b1001001101101100;

  static constexpr int32_t hsteps_ =
    0b10010001001100100110010011001000;

  byte current_ = 0;
  bool forward_ = true;
  bool reverse_ = false;
  bool continuous_ = false;

  uint32_t micros_pr_step_ = 0;
  uint32_t remaining_ = 0;
  LoopClock::Time clock_;
};

class StepperControl {
public:
  StepperControl() {}
  void tick();
  void step(byte step, byte unit);
  virtual void begin() {};
  virtual ~StepperControl();
  void addStepper(Stepper* stepper, byte unit);
private:
  virtual void doStep(byte step, byte unit) = 0;
  virtual void doMoveSteppers() = 0;
  virtual void doTick() = 0;
  virtual void doAddStepper(Stepper* stepper, byte unit) = 0;

  StepperControl& operator=(const StepperControl&) = delete;
  StepperControl(const StepperControl&) = delete;
  StepperControl(StepperControl&&) = delete;
};


template <typename UINT>
class SerialStepperControl : public StepperControl {
public:
  using Status = UINT;
  SerialStepperControl() {
    for (auto& stepper : steppers_) {
      stepper = nullptr;
    }
  }
private:
  virtual void doTick(Status status) = 0;

  static constexpr byte nSteppers() {
    return 2 * sizeof(Status);
  }
  void doAddStepper(Stepper* stepper, byte unit) override {
    if (unit >= 0 && unit < nSteppers()) {
      steppers_[unit] = stepper;
    }
  }
  void doMoveSteppers() override {
    for (byte unit = 0; unit < nSteppers(); ++unit) {
      if (steppers_[unit]) {
        steppers_[unit]->move(*this, unit);
      }
    }
  }
  void doStep(byte step, byte unit) override {
    Status unit_step = step << (4 * unit);
    Status unit_mask = 0xf << (4 * unit);
    status_ = (status_ & ~unit_mask) | unit_step;
  }
  void doTick() override {
    if (status_ != old_status_) {
      old_status_ = status_;
      doTick(status_);
    }
  }
  Status status_ = 0;
  Status old_status_ = 0;
  Stepper* steppers_[nSteppers()];
};



#endif
