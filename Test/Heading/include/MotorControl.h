#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include <Arduino.h>

class MotorControl {
public:
    MotorControl(int leftFwd, int leftBwd, int rightFwd, int rightBwd);
    void begin();
    void controlMotors(float error);
    void stopMotors();

private:
    int motorPinLeftForward, motorPinLeftBackward, motorPinRightForward, motorPinRightBackward;
};

#endif