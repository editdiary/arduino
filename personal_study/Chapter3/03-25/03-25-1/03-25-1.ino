#include <Servo.h>
#include "DHT.h"

#define DHTPIN 3
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

#define SERVO_PIN 9
Servo myservo;

unsigned long prevTime = 0;
unsigned long currTime = 0;

void setup()
{
  Serial.begin(9600);
  dht.begin();
  myservo.attach(SERVO_PIN);
}

void loop()
{
  currTime = millis();
  if (currTime - prevTime >= 2000)
  {
    prevTime = currTime;
    int temperature = dht.readTemperature();
    Serial.print(temperature);
    if (temperature >= 0 && temperature <= 40)
    {
      int servoOut = map(temperature, 0, 40, 180, 0);
      Serial.print(",");
      Serial.println(servoOut);
      myservo.write(servoOut);
    }
  }
}
