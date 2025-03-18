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
void drawCompass(int angle) {
  u8g2.clearBuffer(); // Clear the buffer

  // Draw the compass circle
  u8g2.drawCircle(64, 32, 30, U8G2_DRAW_ALL); // coordinate x,y and radius

  // Calculate the arrow end point based on the angle
  float radians = angle * DEG_TO_RAD;   //degree * (pi/180)
  int arrowX = 64 + (int)(25 * sin(radians));
  int arrowY = 32 - (int)(25 * cos(radians));

    // Draw the arrow head
  int headSize = 5; // Size of the arrow head
  int headX1 = 64 + (int)( (25 - headSize) * sin(radians - 0.3));
  int headY1 = 32 - (int)( (25-headSize) * cos(radians - 0.3));
  int headX2 = 64 + (int)( (25 - headSize) * sin(radians + 0.3));
  int headY2 = 32 - (int)( (25-headSize) * cos(radians + 0.3));

  u8g2.drawLine(64, 32, arrowX, arrowY);
  u8g2.drawLine(arrowX, arrowY, headX1, headY1);
  u8g2.drawLine(arrowX, arrowY, headX2, headY2);
  // Draw cardinal directions
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(60, 10, "N"); // North
  u8g2.drawStr(60, 64, "S"); // South
  u8g2.drawStr(120, 35, "E"); // East
  u8g2.drawStr(0, 35, "W");  // West
  
  // Display the angle
    char angleStr[4];
    sprintf(angleStr, "%03d", angle);
    u8g2.drawStr(55, 50,angleStr);
    u8g2.drawStr(85, 50, "°");

  u8g2.sendBuffer(); // Send to display
}

void setup() {
  Serial.begin(115200);
  compass.init();
  u8g2.begin();
}

void loop() {
  int angle;
  compass.read();
  angle = compass.getAzimuth();
  
  drawCompass(angle);
  delay(100);
}
