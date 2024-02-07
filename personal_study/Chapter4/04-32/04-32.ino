#include <SoftwareSerial.h>
#include "DHT.h"

SoftwareSerial btSerial = SoftwareSerial(9, 10);

#define DHTPIN 3
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

unsigned long currTime = 0;
unsigned long prevTime = 0;

void setup() {
  Serial.begin(115200);
  btSerial.begin(115200);
  dht.begin();
}

void loop() {
  currTime = millis();
  if (currTime - prevTime >= 5000)
  {
    prevTime = currTime;
    float humi = dht.readHumidity();
    float temp = dht.readTemperature();
    if (temp >= 0 && temp <= 60)
    {
      Serial.print(temp);
      Serial.print(",");
      Serial.println(humi);
      btSerial.print(temp);
      btSerial.print(",");
      btSerial.println(humi);
    }
  }
}
