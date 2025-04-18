#include <Arduino.h>
#include <QMC5883LCompass.h>
#include "U8g2lib.h"
#include <Wire.h>

#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

// Khai báo các đối tượng cảm biến
QMC5883LCompass compass;
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

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

// Function to draw a compass circle and arrow
void drawCompass(int angle, int x, int y, int z) {
  u8g2.clearBuffer(); // Clear the buffer

  // Draw the compass circle
  u8g2.drawCircle(96, 32, 30, U8G2_DRAW_ALL); // coordinate x,y and radius

  // Calculate the arrow end point based on the angle
  float radians = angle * DEG_TO_RAD;   // degree * (pi/180)
  int arrowX = 96 + (int)(25 * sin(radians));
  int arrowY = 32 - (int)(25 * cos(radians));

  // Draw the arrow head
  int headSize = 5; // Size of the arrow head
  int headX1 = 96 + (int)((25 - headSize) * sin(radians - 0.3));
  int headY1 = 32 - (int)((25 - headSize) * cos(radians - 0.3));
  int headX2 = 96 + (int)((25 - headSize) * sin(radians + 0.3));
  int headY2 = 32 - (int)((25 - headSize) * cos(radians + 0.3));

  u8g2.drawLine(96, 32, arrowX, arrowY);
  u8g2.drawLine(arrowX, arrowY, headX1, headY1);
  u8g2.drawLine(arrowX, arrowY, headX2, headY2);

  // Draw cardinal directions
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(92, 10, "N"); // North
  u8g2.drawStr(92, 64, "S"); // South
  u8g2.drawStr(120, 35, "E"); // East
  u8g2.drawStr(64, 35, "W");  // West

  // Display the angle
  char angleStr[4];
  sprintf(angleStr, "%03d", angle);
  u8g2.setFont(u8g2_font_profont12_tf); // smaller font
  u8g2.drawStr(90, 45, angleStr); // Changed Y position to 45

  // Display x, y, z
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

  u8g2.sendBuffer(); // Send to display
}

void setup() {
  Serial.begin(115200);
  compass.init();
  u8g2.begin();
  
  pinMode(motorPinLeftForward, OUTPUT);
  pinMode(motorPinLeftBackward, OUTPUT);
  pinMode(motorPinRightForward, OUTPUT);
  pinMode(motorPinRightBackward, OUTPUT);
}

void loop() {
  compass.read();
  int angle = compass.getAzimuth();
  int x = compass.getX();
  int y = compass.getY();
  int z = compass.getZ();

  // Tính sai số
  float error = desired_heading - angle;

  // Đảm bảo sai số trong khoảng -180 đến 180
  if (error > 180) {
    error -= 360;
  } else if (error < -180) {
    error += 360;
  }

  // Điều khiển động cơ dựa trên sai số
  controlMotors(error);

  // Vẽ la bàn
  drawCompass(angle, x, y, z);
  
  delay(100); // Thời gian chờ giữa các lần lặp
}