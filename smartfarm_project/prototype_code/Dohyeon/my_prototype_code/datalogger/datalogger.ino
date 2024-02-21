#include <SPI.h>
#include <SD.h>
#include "DHT.h"
#include <ThreeWire.h>
#include <RtcDS1302.h>

const int chipSelect = 10;

unsigned long currTime = 0;
unsigned long prevTime = 0;

#define DHTPIN 9

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

#define DS1302_CLK  5
#define DS1302_DAT  4
#define DS1302_RST  2

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

  //SD카드에 column name 적기(뺄지 말지 고민)
  dataFile = SD.open("logdata.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println("Date Time Temperature Humidity");
    dataFile.close();
  }
}

void loop() {
  currTime = millis();
  if (currTime - prevTime >= 2000)
  {
    prevTime = currTime;
    RtcDateTime now = Rtc.GetDateTime();

    // 시간 표시
    char tt[26];
    sprintf(tt, "%04u/%02u/%02u %02u:%02u:%02u", now.Year(), now.Month(), now.Day(), now.Hour(), now.Minute(), now.Second());
    Serial.print(tt);
    Serial.print(" ");

    float humi = dht.readHumidity();
    float temp = dht.readTemperature();
    Serial.print(temp);
    Serial.print(" ");
    Serial.println(humi);

    if (temp >= 0 && temp <= 60)
    {
      // 각종 데이터 기록
      dataFile = SD.open("logdata.txt", FILE_WRITE);
      if (dataFile) {
        dataFile.print(tt);
        dataFile.print(" ");
        dataFile.print(temp);
        dataFile.print(" ");
        dataFile.println(humi);
        dataFile.close();
      }
      else Serial.println("error opening file");
    }
  }
}