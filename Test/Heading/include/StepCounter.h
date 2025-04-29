#ifndef STEPCOUNTER_H
#define STEPCOUNTER_H

#include <Arduino.h>
#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"

class StepCounter {
public:
    StepCounter();
    void begin();
    void update();
    int getStepCount();
    float getDistanceTravelled();

private:
    MPU6050 mpu;
    int stepCount;
    float stepLength;
    float distanceTravelled;
    float accelPrev;
};

#endif
