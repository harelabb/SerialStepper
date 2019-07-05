#ifndef SHIFTOUT595STEPPER_H
#define SHIFTOUT595STEPPER_H

#include "SerialStepperControl.h"

/// @file

/// A SerialStepperControl for the 74HC595 shift register using ShiftOut().
/// \n The controller can handle up to 8 cascaded shift registers,
/// each with 2 Stepper motors, i.e. a total of 16.
class Shiftout595StepperControl
  : public SerialStepperControl<uint64_t> {

public:
  /// Constructor.
  explicit Shiftout595StepperControl(byte ser_pin,
                                     byte ltc_pin,
                                     byte clk_pin);
private:
  void doBegin() override;
  void doMove(Status status) override;
  const byte ser_pin_;
  const byte ltc_pin_;
  const byte clk_pin_;
};
#endif
