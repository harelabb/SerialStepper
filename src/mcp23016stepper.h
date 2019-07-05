#ifndef MCP23016STEPPER_H
#define MCP23016STEPPER_H

#include "SerialStepperControl.h"

/// @file

/// A SerialStepperControl using the MCP23016 i2c port expander.
/// This controller can drive up to 4 steppers.
/// \n Up to 8 i2c addresse gives a (theoretical) total of 32 steppers.
class Mcp23016StepperControl
  : public SerialStepperControl<uint16_t> {
public:
  /// Constructor.
  /// @param i2caddr specifies the i2c address.
  explicit Mcp23016StepperControl(int i2caddr = 0x20);

private:
  void doBegin() override;
  void doMove(Status status) override;
  int i2caddr_;
};
#endif
