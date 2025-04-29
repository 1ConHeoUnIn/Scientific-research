#ifndef COMPASS_H
#define COMPASS_H

#include <Arduino.h>
#include <QMC5883LCompass.h>

class Compass {
public:
    Compass();
    void begin();
    int getHeading();

private:
    QMC5883LCompass compass;
};

#endif
