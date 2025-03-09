#include <Arduino.h>
#define ENCODER_1A 16 // Pin for Encoder A yellow
#define ENCODER_1B 17 // Pin for Encoder B green

int pos = 0;
void readEncoder()
{
  int b = digitalRead(ENCODER_1B);
  if (b > 0)
    
  {
    pos++;
  }
  else
  {
    pos--;
  }
}
void setup()
{
  Serial.begin(115200);
  pinMode(ENCODER_1A, INPUT_PULLUP);
  pinMode(ENCODER_1B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCODER_1A), readEncoder, RISING);
}

void loop()
{
  Serial.println("Encoder 2 value: " + String(pos) + "\tSo vong quay: " + String(pos/680) + "\tGoc quay: " + String(((pos % 680) * 360) / 680) );
}
