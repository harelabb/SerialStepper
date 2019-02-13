#ifndef ARDUINOSTEPPER_H
#define ARDUINOSTEPPER_H

#include "SerialStepperControl.h"

// This controller drives the stepper directly from the Arduino
// Each controller can handle 1 stepper.
class ArduinoStepperControl : public SerialStepperControl<byte, 1> {
public:
  ArduinoStepperControl(byte pin1, byte pin2, byte pin3, byte pin4);
  void begin() override;
private:
  void doMove(Status status) override;
  const byte pin1_;
  const byte pin2_;
  const byte pin3_;
  const byte pin4_;
};
#endif
