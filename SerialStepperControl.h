#ifndef SERIALSTEPPERCONTROL_H
#define SERIALSTEPPERCONTROL_H

#include <Arduino.h>

class Stepper;

class StepperControl {
public:
  StepperControl() {}
  void run();
  virtual void begin() {};
  virtual ~StepperControl();
  void addStepper(Stepper* stepper);

protected:
  byte move(Stepper& stepper) const;

private:
  virtual void doStep(byte pos, byte unit) = 0;
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
        byte stat = move(*steppers_[unit]);
        doStep(stat, unit);
      }
    }
  }

  void doStep(byte stat, byte unit) override {
    if (unit < nSteppers()) {
      Status unit_stat = stat << (4 * unit);
      Status unit_mask = 0x0F << (4 * unit);
      status_ = (status_ & ~unit_mask) | unit_stat;
    }
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
