#include <Arduino.h>
#include <QMC5883LCompass.h>
#include "U8g2lib.h"
#include <Wire.h>

#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

QMC5883LCompass compass;
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Function to draw a compass circle and arrow
void drawCompass(int angle, int x, int y, int z) {
  u8g2.clearBuffer(); // Clear the buffer

  // Draw the compass circle - Moved to the right side
  u8g2.drawCircle(96, 32, 30, U8G2_DRAW_ALL); // coordinate x,y and radius

  // Calculate the arrow end point based on the angle
  float radians = angle * DEG_TO_RAD;   //degree * (pi/180)
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

  // Draw cardinal directions - Adjusted positions
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(92, 10, "N"); // North
  u8g2.drawStr(92, 64, "S"); // South
  u8g2.drawStr(120, 35, "E"); // East
  u8g2.drawStr(64, 35, "W");  // West

  // Display the angle - Below the center of the circle
  char angleStr[4];
  sprintf(angleStr, "%03d", angle);
  u8g2.setFont(u8g2_font_profont12_tf); // smaller font
  u8g2.drawStr(90,45, angleStr); // Changed Y position to 45


  // Display x, y, z - Left side, horizontally
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
}

void loop() {
  int angle;
  int x, y, z;
  compass.read();
  angle = compass.getAzimuth();
  x = compass.getX();
  y = compass.getY();
  z = compass.getZ();

  drawCompass(angle, x, y, z);
  delay(100);
}
