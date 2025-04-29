#include "Compass.h"

Compass::Compass() {}

void Compass::begin() {
    compass.init();
}

int Compass::getHeading() {
    compass.read();
    return compass.getAzimuth();
}
