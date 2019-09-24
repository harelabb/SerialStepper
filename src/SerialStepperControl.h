#ifndef SERIALSTEPPERCONTROL_H
#define SERIALSTEPPERCONTROL_H

#include <Arduino.h>

/// @file

class Stepper;

/// Abstract interface class for the stepper control.
/// This is the base class for the control to be
/// implemented for specific port expanding hardware
class StepperControl {

public:
  /// Initialize the port expander used.
  /// Should be called in setup()
  void begin();

  /// Let the StepperControl drive the stepper.
  /// Should be called in setup()
  void addStepper(Stepper& stepper);

  /// Update the steppers drivers output.
  /// Should be called in loop()
  bool run();

  /// Cannot copy StepperControl.
  StepperControl(const StepperControl&) = delete;
  /// Can move StepperControl.
  StepperControl(StepperControl&&) = default;

  /// Cannot assign to StepperControl.
  StepperControl& operator=(const StepperControl&) = delete;
  /// Can move assign to StepperControl.
  StepperControl& operator=(StepperControl&&) = default;

  virtual ~StepperControl() = default;

protected:
  StepperControl() = default;
  byte tickStepper(Stepper& stepper);

private:
  virtual void doBegin() = 0;
  virtual bool doRun() = 0;
  virtual void doAddStepper(Stepper& stepper) = 0;
};


/// Intermediate abstract base class for StepperControl.
/// This class takes care of the communication between the
/// Stepper and the StepperControl, leaving only two funtions
/// to be implemented at the leaf class;
/// \n doBegin(), that initializes the hardware, and
/// \n doMove() that does the real physical output to the stepper motors.
/// @param UINT: unsigned integral type to store all ouput values.
/// One byte is needed for every 2 Stepper.
/// @param MAX_STEPPER: The maxomum number of Stepper motors the
/// StepperControl can handle. Defaults to two times the number of bytes in UINT.
template <typename UINT, byte MAX_STEPPERS = 2 * sizeof(UINT)>
class SerialStepperControl : public StepperControl {
public:
  /// Returns the maxium number of steppers this controller type can handle.
  static constexpr int maxSteppers() {
    return MAX_STEPPERS;
  }

protected:
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
  /// Initialize hardware.
  /// To be implemented in final class.
  virtual void doBegin() = 0;

  /// Generate the output pin values.
  /// <p> To be implemented in final class.
  /// <p> The status parameter holds the pin value patterns for all
  /// the steppers that this contoller handles.
  /// <p> This function should set the output pins LOW/HIGH accoringly.
  virtual void doMove(Status status) = 0;

  void doAddStepper(Stepper& stepper) override final {
    if (n_steppers_ < MAX_STEPPERS) {
      steppers_[n_steppers_++] = &stepper;
    }
  }

  bool doRun() override final {
    for (byte unit = 0; unit < nSteppers(); ++unit) {
      byte stat = tickStepper(*steppers_[unit]);
      updateStatus(stat, unit);
    }
    if (status_ != old_status_) {
      old_status_ = status_;
      doMove(status_);
      return true;
    }
    return false;
  }

  void updateStatus(byte stat, byte unit) {
    Status unit_stat = Status(stat) << (4 * unit);
    Status unit_mask = Status(0x0F) << (4 * unit);
    status_ = (status_ & ~unit_mask) | unit_stat;
  }

  Status status_ = 0;
  Status old_status_ = ~Status(0);
  byte n_steppers_ = 0;
  Stepper* steppers_[MAX_STEPPERS];
};
#endif
