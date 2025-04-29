#include "MotorControl.h"

MotorControl::MotorControl(int leftFwd, int leftBwd, int rightFwd, int rightBwd) 
    : motorPinLeftForward(leftFwd), motorPinLeftBackward(leftBwd), 
      motorPinRightForward(rightFwd), motorPinRightBackward(rightBwd) {}

void MotorControl::begin() {
    pinMode(motorPinLeftForward, OUTPUT);
    pinMode(motorPinLeftBackward, OUTPUT);
    pinMode(motorPinRightForward, OUTPUT);
    pinMode(motorPinRightBackward, OUTPUT);
}

void MotorControl::controlMotors(float error) {
    if (error > 5.0) {
        digitalWrite(motorPinLeftForward, HIGH);
        digitalWrite(motorPinLeftBackward, LOW);
        digitalWrite(motorPinRightForward, LOW);
        digitalWrite(motorPinRightBackward, LOW);
    } else if (error < -5.0) {
        digitalWrite(motorPinLeftForward, LOW);
        digitalWrite(motorPinLeftBackward, LOW);
        digitalWrite(motorPinRightForward, HIGH);
        digitalWrite(motorPinRightBackward, LOW);
    } else {
        digitalWrite(motorPinLeftForward, HIGH);
        digitalWrite(motorPinLeftBackward, LOW);
        digitalWrite(motorPinRightForward, HIGH);
        digitalWrite(motorPinRightBackward, LOW);
    }
}
void MotorControl::stopMotors() {
    digitalWrite(motorPinLeftForward, LOW);
    digitalWrite(motorPinLeftBackward, LOW);
    digitalWrite(motorPinRightForward, LOW);
    digitalWrite(motorPinRightBackward, LOW);
}
