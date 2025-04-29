#include "StepTracker.h"
#include "Navigation.h"

StepTracker::StepTracker() 
    : stepCount(0), stepLength(0.7), distanceTravelled(0), kalmanFilter(0.01, 0.01, 0.1) {}

void StepTracker::begin() {
    Wire.begin();
    mpu.initialize();

    if (!mpu.testConnection()) {
        Serial.println("Không tìm thấy MPU6050, kiểm tra kết nối!");
        while (1);
    }

    Serial.println("MPU6050 đã khởi động thành công!");
}

void StepTracker::updateStepCount(Navigation &navigation, float current_x, float current_y, float target_x, float target_y) {
    int16_t ax, ay, az;
    mpu.getAcceleration(&ax, &ay, &az);

    float accelXY = sqrt(pow(ax / 16384.0, 2) + pow(ay / 16384.0, 2));
    float filteredAccel = kalmanFilter.updateEstimate(accelXY);  // Áp dụng bộ lọc Kalman

    // In giá trị để kiểm tra
    Serial.print("⏳ Gia tốc thô: ");
    Serial.print(accelXY);
    Serial.print(" | 📏 Gia tốc đã lọc: ");
    Serial.println(filteredAccel);

    if (abs(filteredAccel - accelXY) > 0.3) {  // Điều chỉnh ngưỡng nếu cần
        stepCount++;
        distanceTravelled = stepCount * stepLength;

        Serial.print("👣 Bước chân: ");
        Serial.print(stepCount);
        Serial.print(" - 📏 Quãng đường: ");
        Serial.print(distanceTravelled);
        Serial.println(" m");

        // Không cần tạo mới `Navigation`, chỉ cần truyền tham chiếu
        float targetDistance = navigation.getDistanceToTarget(current_x, current_y, target_x, target_y);
        Serial.print("🎯 Khoảng cách còn lại đến mục tiêu: ");
        Serial.println(targetDistance);
    }
}
