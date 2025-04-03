#include <Arduino.h>
#include <QMC5883LCompass.h>

// Khai báo các đối tượng cảm biến
QMC5883LCompass compass;

// Hướng mong muốn (setpoint)
float desired_heading = 90; // Độ (0-360)

// Chân điều khiển động cơ
const int motorPinLeftForward = 16;  // Chân tiến động cơ trái
const int motorPinLeftBackward = 17; // Chân lùi động cơ trái
const int motorPinRightForward = 25;  // Chân tiến động cơ phải
const int motorPinRightBackward = 26; // Chân lùi động cơ phải

void controlMotors(float error) {
  if (error > 10) {
    // Xoay sang phải
    digitalWrite(motorPinLeftForward, HIGH);  // Chạy động cơ trái
    digitalWrite(motorPinLeftBackward, LOW);   // Dừng lùi động cơ trái
    digitalWrite(motorPinRightForward, LOW);   // Dừng động cơ phải
    digitalWrite(motorPinRightBackward, LOW);   // Dừng lùi động cơ phải
  } else if (error < -10) {
    // Xoay sang trái
    digitalWrite(motorPinLeftForward, LOW);    // Dừng động cơ trái
    digitalWrite(motorPinLeftBackward, LOW);   // Dừng lùi động cơ trái
    digitalWrite(motorPinRightForward, HIGH);   // Chạy động cơ phải
    digitalWrite(motorPinRightBackward, LOW);   // Dừng lùi động cơ phải
  } else {
    // Tiến thẳng
    digitalWrite(motorPinLeftForward, HIGH);    // Chạy động cơ trái
    digitalWrite(motorPinLeftBackward, LOW);   // Dừng lùi động cơ trái
    digitalWrite(motorPinRightForward, HIGH);   // Chạy động cơ phải
    digitalWrite(motorPinRightBackward, LOW);   // Dừng lùi động cơ phải
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Khởi động cảm biến
  compass.init();
  
  pinMode(motorPinLeftForward, OUTPUT);
  pinMode(motorPinLeftBackward, OUTPUT);
  pinMode(motorPinRightForward, OUTPUT);
  pinMode(motorPinRightBackward, OUTPUT);
}

void loop() {
  compass.read();
  float azimuth = compass.getAzimuth(); // Lấy giá trị azimuth từ cảm biến

  // Tính sai số
  float error = desired_heading - azimuth;

  // Đảm bảo sai số trong khoảng -180 đến 180
  if (error > 180) {
    error -= 360;
  } else if (error < -180) {
    error += 360;
  }

  // Điều khiển động cơ dựa trên sai số
  controlMotors(error);

  // In ra dữ liệu để kiểm tra
  Serial.print("Azimuth: ");
  Serial.print(azimuth);
  Serial.print(" | Error: ");
  Serial.println(error);

  delay(100); // Thời gian chờ giữa các lần lặp
}