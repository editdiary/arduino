#include <SPI.h>
#include <SD.h>
#include "DHT.h"
#include <ThreeWire.h>
#include <RtcDS1302.h>

const int chipSelect = 10;

unsigned long currTime = 0;
unsigned long prevTime = 0;

#define DHTPIN 3

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

#define DS1302_CLK  6
#define DS1302_DAT  5
#define DS1302_RST  4

ThreeWire myWire(DS1302_DAT, DS1302_CLK, DS1302_RST);
RtcDS1302<ThreeWire> Rtc(myWire);

File dataFile;

void setup() {
  Serial.begin(9600);
  if (!SD.begin(chipSelect)) while (1);
  Serial.println("init sd card");
  dht.begin();

  Rtc.Begin();

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

  if (!Rtc.IsDateTimeValid()) Rtc.SetDateTime(compiled);
  if (Rtc.GetIsWriteProtected()) Rtc.SetIsWriteProtected(false);
  if (!Rtc.GetIsRunning()) Rtc.SetIsRunning(true);

  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled) Rtc.SetDateTime(compiled);
}

void loop() {
  currTime = millis();
  if (currTime - prevTime >= 1000)
  {
    prevTime = currTime;
    RtcDateTime now = Rtc.GetDateTime();
    float humi = dht.readHumidity();
    float temp = dht.readTemperature();
    Serial.print(temp);
    Serial.print(",");
    Serial.println(humi);
    if (temp >= 0 && temp <= 60)
    {
      dataFile = SD.open("Logger.txt", FILE_WRITE);
      if (dataFile) {
        dataFile.print(now.Year());
        dataFile.print("/");
        dataFile.print(now.Month());
        dataFile.print("/");
        dataFile.print(now.Day());
        dataFile.print(",");
        dataFile.print(now.Hour());
        dataFile.print(":");
        dataFile.print(now.Minute());
        dataFile.print(":");
        dataFile.print(now.Second());
        dataFile.print(",");
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
