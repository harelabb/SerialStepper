#ifndef SERIALSTEPPER_H
#define SERIALSTEPPER_H

#include <Arduino.h>
#include "loopclock.h"

class StepperControl;

class Stepper {
public:
  enum Direction:byte {BACKWARD = false, FORWARD = true};
  explicit Stepper(StepperControl& control);
  void start();
  void stop();
  void speed(float rpm);
  void direction(Direction direction);
  void reverse();
  void step(uint32_t steps);
  void turn(float turns);
  Direction direction() const;
  uint32_t remaining() const;
  bool running() const;

  void move(StepperControl& control, byte unit);

  Stepper& operator=(const Stepper&) = delete;
  Stepper(const Stepper&) = delete;
  Stepper(Stepper&&) = delete;

private:
  static constexpr uint32_t forever() {
    return ~uint32_t(0);
  }
  void advance();
  byte fullStep() const;
  LoopClock::Time now() const;

  byte current_ = 0;
  Direction direction_ = FORWARD;
  bool reverse_ = false;

  uint32_t micros_pr_step_ = 0;
  uint32_t remaining_ = 0;
  LoopClock::Time clock_;
  static constexpr uint32_t steps_pr_turn_ = 2048;
};

inline Stepper::Direction operator!(Stepper::Direction direction) {
  return direction == Stepper::FORWARD ?
    Stepper::BACKWARD : Stepper::FORWARD;
  }

class StepperControl {
public:
  StepperControl() {}
  void run();
  void step(byte step, byte unit);
  virtual void begin() {};
  virtual ~StepperControl();
  void addStepper(Stepper* stepper);
private:
  virtual void doStep(byte step, byte unit) = 0;
  virtual void doMoveSteppers() = 0;
  virtual void doRun() = 0;
  virtual void doAddStepper(Stepper* stepper) = 0;

  StepperControl& operator=(const StepperControl&) = delete;
  StepperControl& operator=(StepperControl&&) = delete;
  StepperControl(const StepperControl&) = delete;
  StepperControl(StepperControl&&) = delete;
};


template <typename UINT, byte MAX_STEPPERS = 2 * sizeof(UINT)>
class SerialStepperControl : public StepperControl {
public:
  using Status = UINT;

  SerialStepperControl() {
    for (auto& stepper : steppers_) {
      stepper = nullptr;
    }
  }

  byte nSteppers() const {
    return n_steppers_;
  }

private:
  virtual void doMove(Status status) = 0;

  void doAddStepper(Stepper* stepper) override {
    if (n_steppers_ < MAX_STEPPERS) {
      steppers_[n_steppers_++] = stepper;
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
    Status unit_mask = 0x0F << (4 * unit);
    status_ = (status_ & ~unit_mask) | unit_step;
  }

  void doRun() override {
    if (status_ != old_status_) {
      old_status_ = status_;
      doMove(status_);
    }
  }

  Status status_ = 0;
  Status old_status_ = 0;
  byte n_steppers_ = 0;
  Stepper* steppers_[MAX_STEPPERS];
};
#endif
