#include <SPI.h>
#include <SD.h>
#include "DHT.h"

const int chipSelect = 10;

unsigned long currTime = 0;
unsigned long prevTime = 0;

int cnt = 0;

#define DHTPIN 3

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

File dataFile;

void setup() {
  Serial.begin(9600);
  if (!SD.begin(chipSelect)) while (1);
  Serial.println("init sd card");
  dht.begin();
}

void loop() {
  currTime = millis();
  if (currTime - prevTime >= 1000)
  {
    prevTime = currTime;
    float humi = dht.readHumidity();
    float temp = dht.readTemperature();
    Serial.print(temp);
    Serial.print(",");
    Serial.println(humi);
    if (temp >= 0 && temp <= 60)
    {
      dataFile = SD.open("temphumi.txt", FILE_WRITE);
      if (dataFile) {
        dataFile.print(temp);
        dataFile.print(",");
        dataFile.println(humi);
        dataFile.close();
        Serial.println("file write ok");
      }
      else Serial.println("error opening file");
    }
  }
}
