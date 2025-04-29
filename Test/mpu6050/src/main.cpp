#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 mpu;
int stepCount = 0;
float stepLength = 0.7;
float distanceTravelled = 0;
float accelPrev = 0;
const float targetDistance = 10.0;

void setup() {
    Serial.begin(115200);
    Wire.begin();
    mpu.initialize();

    if (!mpu.testConnection()) {
        Serial.println("Không tìm thấy MPU6050, kiểm tra kết nối!");
        while (1);
    }
    
    Serial.println("MPU6050 đã khởi động thành công!");
}

void loop() {
    int16_t ax, ay, az;
    mpu.getAcceleration(&ax, &ay, &az);

    float accelXY = sqrt(pow(ax / 16384.0, 2) + pow(ay / 16384.0, 2));

    if (abs(accelXY - accelPrev) > 0.1) {
        stepCount++;
        distanceTravelled = stepCount * stepLength;

        Serial.print("Bước chân: ");
        Serial.print(stepCount);
        Serial.print(" - Quãng đường: ");
        Serial.print(distanceTravelled);
        Serial.println(" m");

        // Kiểm tra nếu đạt 10m và reset lại
        if (distanceTravelled >= targetDistance) {
            Serial.println("🎉 Đã đạt 10m! Reset lại...");
            
            // Reset giá trị
            stepCount = 0;
            distanceTravelled = 0;

            // Hiển thị giá trị reset ngay lập tức
            Serial.println("Bước chân: 0 - Quãng đường: 0 m");
        }
    }

    accelPrev = accelXY;
    delay(100);
}
