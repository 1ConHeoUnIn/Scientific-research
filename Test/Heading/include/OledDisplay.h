#ifndef OLEDDISPLAY_H
#define OLEDDISPLAY_H

#include <Arduino.h>
#include "U8g2lib.h"

class OledDisplay {
public:
    OledDisplay();
    void begin();
    void drawDisplay(int angle, float desired_heading, float distance);
    
private:
    U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
};

#endif
