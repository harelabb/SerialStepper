#ifndef SPI595STEPPER_H
#define SPI595STEPPER_H

#include "SerialStepperControl.h"

/// @file

/// A SerialStepperControl for the 74HC595 shift register using SPI.
/// \n The controller can handle up to 8 cascaded shift registers,
/// each with 2 Stepper motors, i.e. a total of 16.
class Spi595StepperControl : public SerialStepperControl<uint64_t> {
public:
#ifdef ESP8266
  /// Constructor.
  /// @param ltc_pin specifies which pin to use for the latch signal.
  explicit Spi595StepperControl(byte ltc_pin = 15);
#else
  explicit Spi595StepperControl(byte ltc_pin = 10);
#endif
private:
  void doBegin() override;
  void doMove(Status status) override;
  const byte ltc_pin_;
};
#endif
