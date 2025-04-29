#include "Navigation.h"

Navigation::Navigation() {}

float Navigation::calculateBearing(float x1, float y1, float x2, float y2) {
    float angle_rad = atan2(y2 - y1, x2 - x1);
    float angle_deg = angle_rad * 180 / PI;
    return (angle_deg < 0) ? angle_deg + 360 : angle_deg;
}

bool Navigation::checkTargetReached(float current_x, float current_y, float target_x, float target_y) {
    return getDistanceToTarget(current_x, current_y, target_x, target_y) < 0.5;
}

// Hàm mới: Tính khoảng cách đến mục tiêu
float Navigation::getDistanceToTarget(float current_x, float current_y, float target_x, float target_y) {
    return sqrt(pow(target_x - current_x, 2) + pow(target_y - current_y, 2));
}
