#ifndef STEP_TRACKER_H
#define STEP_TRACKER_H

#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include <SimpleKalmanFilter.h>
#include "Navigation.h"


class StepTracker {
public:
    StepTracker();
    void begin();
    void updateStepCount(Navigation &navigation, float current_x, float current_y, float target_x, float target_y);
    
private:
    MPU6050 mpu;
    int stepCount;
    float stepLength;
    float distanceTravelled;
    float accelPrev;
    SimpleKalmanFilter kalmanFilter;
};

#endif  
