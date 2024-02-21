#include <RtcDS1302.h>

#define DS1302_CLK 5
#define DS1302_DAT 4
#define DS1302_RST 2

#define LED_RED 13
#define LED_GREEN 12
#define LED_BLUE 11

unsigned long currTime = 0;
unsigned long prevTime = 0;

// RTC 라이브러리 생성(객체 생성)
ThreeWire myWire(DS1302_DAT, DS1302_CLK, DS1302_RST);
RtcDS1302<ThreeWire> Rtc(myWire);


void setup() {
  Serial.begin(9600);

	pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  Rtc.Begin();

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

  // RTC 모듈 초기 세팅
  if (!Rtc.IsDateTimeValid()) Rtc.SetDateTime(compiled);
  if (Rtc.GetIsWriteProtected()) Rtc.SetIsWriteProtected(false);
  if (!Rtc.GetIsRunning()) Rtc.SetIsRunning(true);

  // RTC 모듈 시간 갱신
  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled) Rtc.SetDateTime(compiled);
}


void loop() {
  currTime = millis();
  if (currTime - prevTime >= 2000)
  {
    prevTime = currTime;
    RtcDateTime now = Rtc.GetDateTime();  // RTC 모듈의 현재 시간 얻기

    // 시간 표시
    char temp[26];
    sprintf(temp, "%04u/%02u/%02u %02u:%02u:%02u", now.Year(), now.Month(), now.Day(), now.Hour(), now.Minute(), now.Second());
    Serial.println(temp);

    // LED 제어
    if (now.Hour() >= 9 && now.Hour() < 18)  // 9:00 ~ 18:00까지는 LED 켜기
    {
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_BLUE, LOW);
    }
    else  // 이외의 시간은 LED 모두 끄기
    {
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_BLUE, LOW);
    }
  }
}