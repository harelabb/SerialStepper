# SerialStepper Library for Arduino and compatibles #

This is a drver for uniplar stepper motors driven by ULN2003/ULN2004
darlington arrays.

## Introduction ##

#### Why another stepper library ####

This stepper motor library is alternative to the
[Arduino Stepper Library](https://www.arduino.cc/en/Reference/Stepper).
One problem with this library is that the step() function to
move the stepper, is blocking.  
That is, everyting else will have to wait until the motor reaches its  
target. This also means that only one stapper can run at the time.

Another problem with multiple steppers, is that each stepper requires   
four output pins on the microcontroller.

#### Overview ####

The library consists of 2 parts:

  1. The **Stepper**, that represents the status of the stepper

  2. The **StepperControl**, that requests update of the stepper status   
     and generate the outout to the Darlinton arrays

In addition, there is the **loopClock**, that takes care of the time,
and the Steppers can check if they should move anoter step.

### No delay ###

The library requires that all steps in the loop are fast.
The total time spent in one loop iteration needs to be less than
about 2 millisconds, if the steppers should be able to run at full speed.

A typical loop function with steppers may look like:

    void loop() {
      loopclock::tick();
      
      if (some_condition) {
        stepper.turn(0.5);
      }

      if (some_other_codition) {
        do_other_stuff();
      }

      stepper_ctl.run();
    }

*some_condition* and *some_other_condition* are typically functions of
time or external events.
