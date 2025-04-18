#include <Arduino.h>
#include <QMC5883LCompass.h>
#include "U8g2lib.h"
#include <Wire.h>
#include <cmath> // Thêm thư viện toán học

#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

// Khai báo các đối tượng cảm biến
QMC5883LCompass compass;
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Tọa độ hiện tại của tàu
float current_x = 0;
float current_y = 0;

// Tọa độ mục tiêu (sẽ được nhập từ Serial)
float target_x = 0;
float target_y = 0;
bool target_reached = true; // Biến trạng thái để kiểm tra đã đến mục tiêu chưa

// Hướng mong muốn
float desired_heading = 0;

// Chân điều khiển động cơ
const int motorPinLeftForward = 16;
const int motorPinLeftBackward = 17;
const int motorPinRightForward = 25;
const int motorPinRightBackward = 26;

// Ngưỡng sai số góc để bắt đầu di chuyển thẳng (độ)
const float HEADING_THRESHOLD = 5.0;

// Khoảng cách tối thiểu đến mục tiêu để coi như đã đến (đơn vị tọa độ)
const float DISTANCE_THRESHOLD = 0.5;

// Bước di chuyển ước tính mỗi lần lặp khi đi thẳng (đơn vị tọa độ)
const float MOVEMENT_STEP = 0.1;

void controlMotors(float error) {
  if (error > HEADING_THRESHOLD) {
    // Xoay sang phải
    digitalWrite(motorPinLeftForward, HIGH);
    digitalWrite(motorPinLeftBackward, LOW);
    digitalWrite(motorPinRightForward, LOW);
    digitalWrite(motorPinRightBackward, LOW);
  } else if (error < -HEADING_THRESHOLD) {
    // Xoay sang trái
    digitalWrite(motorPinLeftForward, LOW);
    digitalWrite(motorPinLeftBackward, LOW);
    digitalWrite(motorPinRightForward, HIGH);
    digitalWrite(motorPinRightBackward, LOW);
  } else {
    // Tiến thẳng
    digitalWrite(motorPinLeftForward, HIGH);
    digitalWrite(motorPinLeftBackward, LOW);
    digitalWrite(motorPinRightForward, HIGH);
    digitalWrite(motorPinRightBackward, LOW);
  }
}

float calculateBearing(float x1, float y1, float x2, float y2) {
  float angle_rad = atan2(y2 - y1, x2 - x1);
  float angle_deg = angle_rad * 180 / PI;
  if (angle_deg < 0) {
    angle_deg += 360;
  }
  return angle_deg;
}

void drawCompass(int angle, int x, int y, int z) {
  u8g2.clearBuffer();

  u8g2.drawCircle(96, 32, 30, U8G2_DRAW_ALL);

  float radians = angle * DEG_TO_RAD;
  int arrowX = 96 + (int)(25 * sin(radians));
  int arrowY = 32 - (int)(25 * cos(radians));

  int headSize = 5;
  int headX1 = 96 + (int)((25 - headSize) * sin(radians - 0.3));
  int headY1 = 32 - (int)((25 - headSize) * cos(radians - 0.3));
  int headX2 = 96 + (int)((25 - headSize) * sin(radians + 0.3));
  int headY2 = 32 - (int)((25 - headSize) * cos(radians + 0.3));

  u8g2.drawLine(96, 32, arrowX, arrowY);
  u8g2.drawLine(arrowX, arrowY, headX1, headY1);
  u8g2.drawLine(arrowX, arrowY, headX2, headY2);

  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(92, 10, "N");
  u8g2.drawStr(92, 64, "S");
  u8g2.drawStr(120, 35, "E");
  u8g2.drawStr(64, 35, "W");

  char angleStr[4];
  sprintf(angleStr, "%03d", angle);
  u8g2.setFont(u8g2_font_profont12_tf);
  u8g2.drawStr(90, 45, angleStr);

  u8g2.setFont(u8g2_font_5x8_tr);
  char xStr[10];
  sprintf(xStr, "X:%d", x);
  u8g2.drawStr(0, 15, xStr);

  char yStr[10];
  sprintf(yStr, "Y:%d", y);
  u8g2.drawStr(0, 30, yStr);

  char zStr[10];
  sprintf(zStr, "Z:%d", z);
  u8g2.drawStr(0, 45, zStr);

  u8g2.sendBuffer();
}

void setup() {
  Serial.begin(115200);
  compass.init();
  u8g2.begin();

  pinMode(motorPinLeftForward, OUTPUT);
  pinMode(motorPinLeftBackward, OUTPUT);
  pinMode(motorPinRightForward, OUTPUT);
  pinMode(motorPinRightBackward, OUTPUT);

  Serial.println("Nhap toa do muc tieu (x,y) qua Serial Monitor:");
  Serial.println("Vi du: 5,2");
}

void loop() {
  compass.read();
  int current_heading = compass.getAzimuth();
  int x = compass.getX();
  int y = compass.getY();
  int z = compass.getZ();

  if (target_reached) {
    // Chờ nhập tọa độ mục tiêu mới từ Serial Monitor
    if (Serial.available() > 0) {
      String input = Serial.readStringUntil('\n');
      input.trim();
      int commaIndex = input.indexOf(',');
      if (commaIndex != -1) {
        String xStr = input.substring(0, commaIndex);
        String yStr = input.substring(commaIndex + 1);
        target_x = xStr.toFloat();
        target_y = yStr.toFloat();
        Serial.print("Da nhan toa do muc tieu: (");
        Serial.print(target_x);
        Serial.print(", ");
        Serial.print(target_y);
        Serial.println(")");
        target_reached = false;
      } else {
        Serial.println("Sai dinh dang. Vui long nhap theo dinh dang 'x,y'.");
      }
    }
    digitalWrite(motorPinLeftForward, LOW);
    digitalWrite(motorPinLeftBackward, LOW);
    digitalWrite(motorPinRightForward, LOW);
    digitalWrite(motorPinRightBackward, LOW);
    delay(100);
    return;
  }

  // Tính toán góc phương vị đến mục tiêu
  desired_heading = calculateBearing(current_x, current_y, target_x, target_y);

  // Tính sai số giữa hướng hiện tại và hướng mong muốn
  float error = desired_heading - current_heading;
  if (error > 180) {
    error -= 360;
  } else if (error < -180) {
    error += 360;
  }

  // Điều khiển động cơ dựa trên sai số
  controlMotors(error);

  // Cập nhật vị trí hiện tại của tàu (ước tính dựa trên hướng đi nếu gần đúng)
  if (abs(error) < HEADING_THRESHOLD) {
    float rad = current_heading * DEG_TO_RAD;
    current_x += MOVEMENT_STEP * cos(rad);
    current_y += MOVEMENT_STEP * sin(rad);
  }

  // Kiểm tra xem đã đến mục tiêu chưa
  float distanceToTarget = sqrt(pow(target_x - current_x, 2) + pow(target_y - current_y, 2));
  if (distanceToTarget < DISTANCE_THRESHOLD) {
    digitalWrite(motorPinLeftForward, LOW);
    digitalWrite(motorPinLeftBackward, LOW);
    digitalWrite(motorPinRightForward, LOW);
    digitalWrite(motorPinRightBackward, LOW);
    Serial.println("Da den muc tieu!");
    current_x = 0;
    current_y = 0;
    target_reached = true;
    Serial.println("Da reset toa do hien tai ve (0,0).");
    Serial.println("Nhap toa do muc tieu moi (x,y) qua Serial Monitor:");
    Serial.println("Vi du: 10,-3");
  }

  // Vẽ la bàn
  drawCompass(current_heading, x, y, z);

  Serial.print("Current Heading: ");
  Serial.print(current_heading);
  Serial.print(", Desired Heading: ");
  Serial.print(desired_heading);
  Serial.print(", Error: ");
  Serial.print(error);
  Serial.print(", Current X: ");
  Serial.print(current_x);
  Serial.print(", Current Y: ");
  Serial.print(current_y);
  Serial.print(", Distance to Target: ");
  Serial.println(distanceToTarget);

  delay(100);
}