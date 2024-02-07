#include <Servo.h>
#include "DHT.h"

#define DHTPIN 3
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

#define SERVO_PIN 9
Servo myservo;

unsigned long prevTime = 0;
unsigned long currTime = 0;

#define BUTTON_PIN  12

int runState = 0;

void setup()
{
  Serial.begin(9600);
  dht.begin();
  myservo.attach(SERVO_PIN);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop()
{
  if (button() == 1)
  {
    runState++;
    if (runState == 1) {
      Serial.println("0 degree");
    }
    else if (runState == 2) {
      Serial.println("10 degree");
    }
    else if (runState == 3) {
      Serial.println("20 degree");
    }
    else if (runState == 4) {
      Serial.println("30 degree");
    }
    else if (runState == 5) {
      Serial.println("40 degree");
    }
    else if (runState >= 6) runState = 0;
    delay(200);
  }

  if (runState == 0)
  {
    Serial.println("runState = 0");
  }
}

int button()
{
  static int oldSw = 1;
  static int newSw = 1;
  newSw = digitalRead(BUTTON_PIN);
  if (newSw != oldSw)
  {
    oldSw = newSw;
    if (newSw == 0) {
      return 1;
    }
  }
  return 0;
}
