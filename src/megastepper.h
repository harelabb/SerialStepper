#ifndef MEGASTEPPER_H
#define MEGASTEPPER_H

#include "SerialStepperControl.h"
/// @file

/// A StepperControl using 6 Arduino Mega ports.
/// PORTA, PORTC, PORTL, PORTB, PORTF, PORTK
/// Each controller can handle 12 steppers.
class MegaStepperControl : public SerialStepperControl<uint64_t, 12> {
public:
  /// Constructor.
  MegaStepperControl();
private:
  void doBegin() override;
  void doMove(Status status) override;
};
#endif
