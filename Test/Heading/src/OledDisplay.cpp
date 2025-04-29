#include "OledDisplay.h"

OledDisplay::OledDisplay() : u8g2(U8G2_R0, U8X8_PIN_NONE) {}

void OledDisplay::begin() {
    u8g2.begin();
}

void OledDisplay::drawDisplay(int angle, float desired_heading, float distance) {
    u8g2.clearBuffer();

    // 🎯 Vẽ vòng tròn la bàn
    u8g2.drawCircle(96, 32, 15);

    // 🔄 Tính toán vị trí kim la bàn
    int size = 5;
    float radians = angle * DEG_TO_RAD;
    int tipX = 96 + (int)(size * sin(radians));
    int tipY = 32 - (int)(size * cos(radians));
    int baseX1 = 96 + (int)(size * sin(radians + 2.1));
    int baseY1 = 32 - (int)(size * cos(radians + 2.1));
    int baseX2 = 96 + (int)(size * sin(radians - 2.1));
    int baseY2 = 32 - (int)(size * cos(radians - 2.1));

    // 🏹 Vẽ tam giác kim la bàn
    u8g2.drawLine(tipX, tipY, baseX1, baseY1);
    u8g2.drawLine(tipX, tipY, baseX2, baseY2);

    // 🌎 Hiển thị ký tự hướng
    u8g2.setFont(u8g2_font_5x8_tr);
    u8g2.drawStr(76, 32, "W"); 
    u8g2.drawStr(95, 55, "S");
    u8g2.drawStr(113, 32, "E");
    u8g2.drawStr(95, 16, "N");
    // 🔄 Hiển thị góc phương vị hiện tại
    char angleStr[10];
    sprintf(angleStr, "H:%03d", angle);
    u8g2.setFont(u8g2_font_profont12_tf);
    u8g2.drawStr(10, 40, angleStr);
    // 🎯 Hiển thị góc mong muốn
    char desiredStr[10];
    sprintf(desiredStr, "A:%03d", (int)desired_heading);
    u8g2.setFont(u8g2_font_profont12_tf);
    u8g2.drawStr(10, 10, desiredStr);
    
    // 📏 Hiển thị khoảng cách đến mục tiêu
    char distanceStr[10];
    sprintf(distanceStr, "D:%.1f", distance);
    u8g2.setFont(u8g2_font_profont12_tf);
    u8g2.drawStr(10, 25, distanceStr);
    
    u8g2.sendBuffer();
}
