#ifndef MCP23016STEPPER_H
#define MCP23016STEPPER_H

#include "SerialStepper.h"

// This i2c controller can drive up to 4 stepper.
// Up to 8 i2c addresse gives a (teorethical) total of 32 steppers
class Mcp23016StepperControl : public SerialStepperControl<uint16_t> {
public:
  explicit Mcp23016StepperControl(int i2caddr = 0x20);
  void begin() override;
private:
  void doTick(Status status) override;
  int i2caddr_;
};
#endif
