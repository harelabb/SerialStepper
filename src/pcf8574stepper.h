#ifndef PCF8574STEPPER_H
#define PCF8574STEPPER_H

#include "SerialStepperControl.h"

/// @file

/// A SerialStepperControl using the  PCF8574A or PCF8574 i2c port expander.
/// This controller can drive up to 2 steppers.
/// \n Up to 8 i2c addresse gives a (theoretical) total of 16 steppers.
/// \n Note that the pcf8574 cannot source enough current to drive
/// the ULN2003. A 47kΩ pull-up resistor on each output pin will help.
class Pcf8574StepperControl : public SerialStepperControl<uint8_t> {
public:
  /// Constructor.
  /// @param i2caddr specifies the i2c address.
  explicit Pcf8574StepperControl(int i2caddr = 0x38);

private:
  void doBegin() override;
  void doMove(Status status) override;
  int i2caddr_;
};
#endif
