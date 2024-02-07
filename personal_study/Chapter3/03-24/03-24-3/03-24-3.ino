#include <MsTimer2.h>

#define SW_PIN  12
#define MOTOR_A_1A  5
#define MOTOR_A_1B  6

boolean stopAndGo = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(SW_PIN, INPUT_PULLUP);
  randomSeed(analogRead(0));
}

void loop()
{
  if( digitalRead(SW_PIN) == 0)
  {
    stopAndGo = 1;
    analogWrite(MOTOR_A_1A,150);  // 세기는 0~255 사이 값
    analogWrite(MOTOR_A_1B,0);
    int randNumber = random(2000, 7000);
    Serial.println(randNumber);
    delay(randNumber);
    stopAndGo = 0;
    analogWrite(MOTOR_A_1A,0);
    analogWrite(MOTOR_A_1B,0);
  }
}
