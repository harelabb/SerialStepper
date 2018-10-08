#ifndef SHIFTREGSTEPPER_H
#define SHIFTREGSTEPPER_H

#include "SerialStepper.h"

// This controller assumes 74HC595 shift registers each with 2 steppers
// The controller can handle up to 8 cascaded shift registers,
// up to 16 steppers.
class ShiftregStepperControl : public SerialStepperControl<uint64_t> {
public:
  ShiftregStepperControl(byte ser_pin, byte ltc_pin, byte clk_pin);
  void begin() override;
private:
  void doTick(Status status) override;
  const byte ser_pin_;
  const byte ltc_pin_;
  const byte clk_pin_;
};
#endif
