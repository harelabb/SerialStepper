#ifndef MCP23S17STEPPER_H
#define MCP23S17STEPPER_H

#include <SerialStepperControl.h>

/// @file

/// A SerialStepperControl using the MCP23S17 SPI port expander.
/// This controller can drive up to 4 steppers.
/// \n Up to 8 addresse gives a (theoretical) total of 32 steppers
/// per cs pin.

class Mcp23S17StepperControl
  : public SerialStepperControl<uint16_t> {
public:
  /// Constructor.
  /// @param cs_pin specifies the chip select pin.
  /// @param addr specifies the chip hw address (0-7 inclusive).

  explicit Mcp23S17StepperControl(uint8_t cs_pin, uint8_t addr = 0);

private:
  void doBegin() override;
  void doMove(Status status) override;
  const uint8_t cs_pin_;
  const uint8_t addr_;
};
#endif
