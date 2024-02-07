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
      int setTemperature = 0;
      int setServoOut = map(setTemperature, 0, 40, 180, 0);
      myservo.write(setServoOut);
    }
    else if (runState == 2) {
      Serial.println("10 degree");
      int setTemperature = 10;
      int setServoOut = map(setTemperature, 0, 40, 180, 0);
      myservo.write(setServoOut);
    }
    else if (runState == 3) {
      Serial.println("20 degree");
      int setTemperature = 20;
      int setServoOut = map(setTemperature, 0, 40, 180, 0);
      myservo.write(setServoOut);
    }
    else if (runState == 4) {
      Serial.println("30 degree");
      int setTemperature = 30;
      int setServoOut = map(setTemperature, 0, 40, 180, 0);
      myservo.write(setServoOut);
    }
    else if (runState == 5) {
      Serial.println("40 degree");
      int setTemperature = 40;
      int setServoOut = map(setTemperature, 0, 40, 180, 0);
      myservo.write(setServoOut);
    }
    else if (runState >= 6) runState = 0;
    delay(200);
  }

  if (runState == 0)
  {
    currTime = millis();
    if (currTime - prevTime >= 2000)
    {
      prevTime = currTime;
      int temperature = dht.readTemperature();
      Serial.print(temperature);
      if (temperature >= 0 && temperature <= 40)
      {
        int servoOut = map(temperature, 0, 40, 0, 180);
        Serial.print(",");
        Serial.println(servoOut);
        myservo.write(servoOut);
      }
    }
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