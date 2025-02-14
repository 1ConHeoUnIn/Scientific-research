
/*check pwm pin
#include <Arduino.h>

// Array of PWM-capable pins (consult your ESP32's datasheet)
const int pwmPins[] = {2, 4, 5, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33};
const int numPwmPins = sizeof(pwmPins) / sizeof(pwmPins[0]);

void setup() {
  Serial.begin(115200);
  Serial.println("PWM-capable pins:");

  for (int i = 0; i < numPwmPins; i++) {
    Serial.print("Pin ");
    Serial.println(pwmPins[i]);
  }
}

void loop() {

}
*/