#ifndef UNOSTEPPER_H
#define UNOSTEPPER_H

#include "SerialStepperControl.h"
/// @file

/// A StepperControl using 2 half Arduino UNO ports.
/// Each controller can handle 2 steppers.
class UnoStepperControl : public SerialStepperControl<uint8_t, 2> {
public:
  /// Constructor.
  UnoStepperControl();
private:
  void doBegin() override;
  void doMove(Status status) override;
};
#endif
