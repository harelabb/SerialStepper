#ifndef PCF8574STEPPER_H
#define PCF8574STEPPER_H

#include "SerialStepperControl.h"

// This i2c controller can drive up to 2 steppers.
// Up to 8 i2c addresse gives a (teorethical) total of 16 steppers
class Pcf8574StepperControl : public SerialStepperControl<uint8_t> {
public:
  explicit Pcf8574StepperControl(int i2caddr = 0x38);
  void begin() override;
private:
  void doMove(Status status) override;
  int i2caddr_;
};
#endif
