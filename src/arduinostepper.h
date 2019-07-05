#ifndef ARDUINOSTEPPER_H
#define ARDUINOSTEPPER_H

#include "SerialStepperControl.h"
/// @file

/// A StepperControl using the Arduino pins directly.
/// Each controller can handle 1 stepper.
class ArduinoStepperControl : public SerialStepperControl<byte, 1> {
public:
  /// Constructor.
  /// @param pin1, pin2, pin3, pin4 : the output pins to use.
  ArduinoStepperControl(byte pin1, byte pin2, byte pin3, byte pin4);
private:
  void doBegin() override;
  void doMove(Status status) override;
  const byte pin1_;
  const byte pin2_;
  const byte pin3_;
  const byte pin4_;
};
#endif
