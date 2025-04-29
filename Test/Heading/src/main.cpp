#include "OledDisplay.h"
#include "Compass.h"
#include "MotorControl.h"
#include "Navigation.h"
#include "StepTracker.h"

// Khởi tạo các đối tượng
OledDisplay oledDisplay;
Compass compass;
MotorControl motorControl(16, 17, 25, 26);
Navigation navigation;
StepTracker stepTracker;

// Đặt tọa độ mục tiêu cố định
const float target_x = 3.0;
const float target_y = 3.0;

// Biến trạng thái và vị trí
float current_x = 0.0, current_y = 0.0;
bool target_reached = false;

void setup() {
    Serial.begin(115200);
    
    // Khởi tạo các module
    oledDisplay.begin();
    compass.begin();
    motorControl.begin();
    stepTracker.begin();

    Serial.println("🚀 Hệ thống khởi động!");
    Serial.print("🎯 Mục tiêu cố định: (");
    Serial.print(target_x);
    Serial.print(", ");
    Serial.print(target_y);
    Serial.println(")");
}

void loop() {
    // Nếu đã đạt mục tiêu, dừng hệ thống
    if (target_reached) {
        Serial.println("✅ Đã đến mục tiêu! Dừng động cơ.");
        motorControl.stopMotors(); // Dừng động cơ
        return;
    }

    // Đọc góc từ la bàn
    int heading = compass.getHeading();
    

    // Tính toán góc mong muốn và sai số
    float desired_heading = navigation.calculateBearing(current_x, current_y, target_x, target_y);
    float error = desired_heading - heading;

    // Điều chỉnh sai số hướng
    if (error > 180) error -= 360;
    else if (error < -180) error += 360;

    // Hiển thị thông tin góc trên Serial Monitor
    Serial.print("🔄 Góc hiện tại: ");
    Serial.print(heading);
    Serial.print(" | 🎯 Góc mong muốn: ");
    Serial.print(desired_heading);
    Serial.print(" | ⚠ Sai số: ");
    Serial.println(error);

    // Điều khiển động cơ
    motorControl.controlMotors(error);

    // Cập nhật vị trí nếu hướng đi đúng
    if (abs(error) < 5.0) {
        float rad = heading * DEG_TO_RAD;
        current_x += 0.1 * cos(rad);
        current_y += 0.1 * sin(rad);
    }

    // Kiểm tra trạng thái đạt mục tiêu
    target_reached = navigation.checkTargetReached(current_x, current_y, target_x, target_y);

    // Tính khoảng cách đến mục tiêu
    float distance = navigation.getDistanceToTarget(current_x, current_y, target_x, target_y);

    // Hiển thị thông tin vị trí và khoảng cách trên Serial Monitor
    Serial.print("📍 Vị trí hiện tại: (");
    Serial.print(current_x);
    Serial.print(", ");
    Serial.print(current_y);
    Serial.println(")");
    
    Serial.print("📏 Khoảng cách đến mục tiêu: ");
    Serial.println(distance);

    // Hiển thị dữ liệu lên màn hình OLED
    
    oledDisplay.drawDisplay(heading, desired_heading, distance);


    // Cập nhật số bước chân
    stepTracker.updateStepCount(navigation, current_x, current_y, target_x, target_y);

    delay(500);
}
