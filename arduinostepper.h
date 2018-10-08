#ifndef ARDUINOSTEPPER_H
#define ARDUINOSTEPPER_H

#include "SerialStepper.h"

// This controller drives the stepper directly from the Arduino
// Each controller can handle 2 steppers.
class ArduinoStepperControl : public StepperControl {
public:
  ArduinoStepperControl(byte pin1, byte pin2, byte pin3, byte pin4);
  void begin() override;
private:
  void doStep(byte step, byte unit) override;
  void doMoveSteppers() override;
  virtual void doTick() override;
  virtual void doAddStepper(Stepper* stepper, byte unit) override;

private:
  const byte pin1_;
  const byte pin2_;
  const byte pin3_;
  const byte pin4_;
  Stepper* stepper_ = nullptr;
  byte status_;
  byte old_status_;

};
#endif
