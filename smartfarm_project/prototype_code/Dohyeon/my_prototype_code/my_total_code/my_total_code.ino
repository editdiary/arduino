// Rtc 모듈 라이브러리
#include <RtcDS1302.h>
#include <ThreeWire.h>

/*
// 데이터로거 라이브러리
//#inclue <SPI.h>
//#include <SD.h>
*/

// Rtc 모듈 핀 지정
#define DS1302_CLK 5
#define DS1302_DAT 4
#define DS1302_RST 2

#define LED_RED 13  // LED 핀 지정

#define LIGHT_CDS A0  // CDS 조도센서 핀 지정(아날로그)

// millis()를 위한 시간 변수 설정
unsigned long currTime = 0;
unsigned long prevTime = 0;
unsigned long ledTime = 0;
/*unsigned long sdTime = 0;*/

/*
const int chipSelect = 10;  // 데이터로거를 위한 chip 핀 선택
File dataFile  // 로거를 남길 file 변수 지정
*/

// RTC 라이브러리 생성(객체 생성)
ThreeWire myWire(DS1302_DAT, DS1302_CLK, DS1302_RST);
RtcDS1302<ThreeWire> Rtc(myWire);


void setup() {
  Serial.begin(9600);

  pinMode(LED_RED, OUTPUT);

  /*
  if (!SD.begin(chipSelect)) while(1);  // SD모듈 시작
  Serial.println("init sd card");
  */

  // Rtc 모듈 세팅
  Rtc.Begin();  //모듈 시작 선언
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);  //Rtc 시간 불러오기
  // 초기 세팅
  if (!Rtc.IsDateTimeValid()) Rtc.SetDateTime(compiled);
  if (Rtc.GetIsWriteProtected()) Rtc.SetIsWriteProtected(false);
  if (!Rtc.GetIsRunning()) Rtc.SetIsRunning(true);
  // 시간 세팅
  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled) Rtc.SetDateTime(compiled);
}

void loop() {
  currTime = millis();  // millis() 시작

  // 각종 센서값 읽기
  RtcDateTime now = Rtc.GetDateTime();  // RTC 모듈로부터 현재 시간 얻기
  int lightValue = analogRead(LIGHT_CDS);  // CDS 조도값
  float humi = dht.readHumidity();      //습도
  float temp = dht.readTemperature();   //온도

  if (currTime - prevTime >= 2000)  // 2초 간격으로 센서값 Serial에 표시
  {
    prevTime = currTime;

    // Serial 시간 표시
    char dt[26];
    sprintf(dt, "%04u/%02u/%02u %02u:%02u:%02u", now.Year(), now.Month(), now.Day(), now.Hour(), now.Minute(), now.Second());
    Serial.print(dt);
    Serial.print("  ");

    // Serial 조도값 표시
    Serial.println(lightValue);
  }

  // 5초마다 led 제어
  if (currTime - ledTime >= 5000)
  {
    ledTime = currTime;
    // 9:00 ~ 18:00 사이 조도값에 따른 LED 제어
    if (now.Hour() >= 9 && now.Hour() < 18)
    {
      if (lightValue <= 500)  // 조도값이 낮다면(혹시 LED가 꺼져있다면) LED 켜기
      {
        digitalWrite(LED_RED, HIGH);
      }
      else continue;  // 조도값이 높다면(LED가 이미 켜져있다면) 코드 건너뛰기
    }
    else  // 저녁 시간(LED를 끌 시간) 조도값에 따른 LED 제어
    {
      if (lightValue <= 500) continue;  // 조도값이 낮다면(이미 LED가 꺼져있다면) 코드 건너뛰기
      else  // 조도값이 높다면(혹시 LED가 켜져있다면) 끄기
      {
        digitalWrite(LED_RED, LOW);
      }
    }
  }

  /*
  // 일정 시간(임의 10초)마다 센서값 SD카드에 기록
  if (currTime - sdTime >= 10000)
  {
    sdTime = currTime;

    // 시간 형식에 맞춰 읽기
    char dt[26];
    sprintf(dt, "%04u/%02u/%02u %02u:%02u:%02u", now.Year(), now.Month(), now.Day(), now.Hour(), now.Minute(), now.Second());

    // 로거 남기기
    dataFile = SD.open("Logger.txt", FILE_WRITE);
    if (dataFile)
    {
      //입력할 센서값들 적기
      dataFile.print(dt);
      dataFile.print("  ")
      dataFile.print(temp);
      dataFile.print("  ");
      dataFile.println(humi);
      dataFile.close();
    }
    else Serial.println("error opening file");
  }
  */
}
