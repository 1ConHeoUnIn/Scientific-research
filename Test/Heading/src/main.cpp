#include "OledDisplay.h"
#include "Compass.h"
#include "MotorControl.h"
#include "Navigation.h"
#include "StepTracker.h"
#include "WiFiConfig.h"

// Khởi tạo các đối tượng
OledDisplay oledDisplay;
Compass compass;
MotorControl motorControl(16, 17, 25, 26);
Navigation navigation;
StepTracker stepTracker;
WiFiConfig wifiConfig;

// Đặt tọa độ mục tiêu cố định
float target_x = 0.0;  // Định nghĩa biến, cấp phát bộ nhớ
float target_y = 0.0;

// Biến trạng thái và vị trí
float current_x = 0.0, current_y = 0.0;
bool target_reached = false;

void setup() {
    Serial.begin(115200);
    
    // Cấu hình WiFi (Nhập SSID/Password từ Web)
    wifiConfig.begin();
    
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
    // Xử lý yêu cầu từ WebServer để nhập tọa độ
    wifiConfig.handleClient();

    // Nếu chưa nhận được tọa độ mới, động cơ sẽ không hoạt động
    if (target_x == 0.0 && target_y == 0.0) {
        Serial.println("⏳ Chưa nhận tọa độ mới, động cơ sẽ không hoạt động.");
        motorControl.stopMotors();
        return; // Không tiếp tục xử lý
    }

    // Nếu đã đạt mục tiêu, dừng hệ thống và gửi tọa độ 0,0 lên server
    if (target_reached) {
        Serial.println("✅ Đã đến mục tiêu! Reset tọa độ...");

        // Dừng động cơ
        motorControl.stopMotors();

        // Reset vị trí hiện tại về 0 để chờ mục tiêu mới
        current_x = 0.0;
        current_y = 0.0;
        target_reached = false; // Đặt lại trạng thái để tiếp tục chạy

        Serial.println("📡 Đang chờ tọa độ mới...");
        delay(2000); // Để tránh xử lý quá nhanh trước khi nhận dữ liệu mới

        // Gửi tọa độ (0,0) lên máy chủ web
        WiFiClient client;
        if (client.connect("192.168.4.1", 80)) {  // Địa chỉ ESP32 ở chế độ AP
            client.println("POST /updateTarget HTTP/1.1");
            client.println("Host: 192.168.4.1");
            client.println("Content-Type: application/x-www-form-urlencoded");
            client.println("Content-Length: 16");
            client.println();
            client.println("target_x=0&target_y=0");
            Serial.println("📡 Đã gửi tọa độ (0,0) lên server!");
        } else {
            Serial.println("❌ Không thể kết nối với server!");
        }
        
        client.stop(); // Đóng kết nối sau khi gửi
        return; // Dừng xử lý tiếp theo
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
    Serial.printf("🔄 Góc hiện tại: %d | 🎯 Góc mong muốn: %.2f | ⚠ Sai số: %.2f\n",
                  heading, desired_heading, error);

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
    Serial.printf("📍 Vị trí hiện tại: (%.2f, %.2f) | 📏 Khoảng cách đến mục tiêu: %.1f\n",
                  current_x, current_y, distance);

    // Hiển thị dữ liệu lên màn hình OLED
    oledDisplay.drawDisplay(heading, desired_heading, distance);

    // Cập nhật số bước chân
    stepTracker.updateStepCount(navigation, current_x, current_y, target_x, target_y);

    delay(500);
}
