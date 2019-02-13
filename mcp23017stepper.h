#ifndef MCP23017STEPPER_H
#define MCP23017STEPPER_H

#include "SerialStepperControl.h"

// This i2c controller can drive up to 4 stepper.
// Up to 8 i2c addresse gives a (teorethical) total of 32 steppers
class Mcp23017StepperControl : public SerialStepperControl<uint16_t> {
public:
  explicit Mcp23017StepperControl(int i2caddr = 0x20);
  void begin() override;
private:
  void doMove(Status status) override;
  int i2caddr_;
};
#endif
