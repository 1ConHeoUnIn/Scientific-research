#include <Arduino.h>
// Define motor pins
#define IN1 2  // Motor A input 1
#define IN2 4  // Motor A input 2
#define IN3 16 // Motor B input 1
#define IN4 17 // Motor B input 2
#define ENA 5  // Motor A enable (PWM)
#define ENB 18 // Motor B enable (PWM)

// PWM Configuration
const int freq = 5000;    // PWM frequency (adjust as needed)
const int resolution = 8; // 8-bit resolution (0-255)
const int channelA = 0;   // PWM channel for Motor A
const int channelB = 1;   // PWM channel for Motor B

// matrix
const int n = 5;
int dutycycle = 140;
int max_speed = 200;

// set position of the boat is the center of the matrix
int currentRow = n / 2;
int currentCol = n / 2;

// target position
int x, y;

void go_forward(int t) // t=s/v
{
  printf("dutycycle: %d\n", dutycycle);
  Serial.println("Go Forward");
  // Motor A Forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  // Motor B Forward
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  delay(t);
}
void stop()
{
  printf("dutycycle: %d\n", dutycycle);
  // Motor A Stop
  Serial.println("Stop");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  // Motor B Stop
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void go_back(int t)
{
  // Motor A back
  Serial.println("Go Back");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  // Motor B  back

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(t);
}
void spin_left()
{
  printf("dutycycle: %d\n", dutycycle);
  Serial.println("Go Forward");
  // Motor A Forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  // Motor B Stop
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  
}

void spin_right()
{
  // Motor A Stop
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  // Motor B Forward
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void speed_control()
{

  for (dutycycle; dutycycle < max_speed; dutycycle++)
  {
    printf("dutycycle: %d\n", dutycycle);
    ledcWrite(channelA, dutycycle);
    ledcWrite(channelB, dutycycle);
    delay(200);
  }
}

void lenght_index(int a, int b)
{
 Serial.print("lenght of indexis : ( , )");
  if (Serial.available() > 0)
  {
    a = Serial.parseInt();
    b = Serial.parseInt();
}

}

void create_map()
{
  int map[n][n];

  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      map[i][j] = 0;
    }
  }
  map[currentRow][currentCol] = 'x';
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      if (map[i][j] == 'x')
      {
        Serial.print('x'); // Print 'x'
        Serial.print(" ");
      }
      else
      {
        Serial.print(map[i][j]); // Print other values
        Serial.print(" ");
      }
    }
    Serial.println();
  }
}

void choose_target()
{
  Serial.print("choose target: ( , )");
  if (Serial.available() > 0)
  {
    x = Serial.parseInt();
    y = Serial.parseInt();
    Serial.print("target was chosen is: (");
    Serial.print(x);
    Serial.print(",");
    Serial.print(y);
    Serial.println(")");
  }
}

void update_position(int newRow, int newCol)
{
if (newRow >= 0 && newRow < n && newCol >= 0 && newCol < n)
{
  currentRow = newRow;
  currentCol = newCol;
  create_map();
}
}



void setup()
{
  // Set motor control pins as outputs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Configure PWM for Motor A
  ledcSetup(channelA, freq, resolution);
  ledcAttachPin(ENA, channelA);
  ledcWrite(channelA, dutycycle); // Set PWM duty cycle (0-255)
  // Configure PWM for Motor B
  ledcSetup(channelB, freq, resolution);
  ledcAttachPin(ENB, channelB);
  ledcWrite(channelB, dutycycle); // Set PWM duty cycle (0-255)

  Serial.begin(115200);
  Serial.println("Motor test!");

  create_map();
}

void loop()
{
  choose_target();
}
