#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <Arduino.h>
#include <cmath>

class Navigation {
public:
    Navigation();
    float calculateBearing(float x1, float y1, float x2, float y2);
    bool checkTargetReached(float current_x, float current_y, float target_x, float target_y);
    float getDistanceToTarget(float current_x, float current_y, float target_x, float target_y); // Thêm hàm mới
};

#endif
