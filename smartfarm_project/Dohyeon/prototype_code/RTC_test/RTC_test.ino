#include <RtcDS1302.h>

#define DS1302_CLK 5
#define DS1302_DAT 4
#define DS1302_RST 2

unsigned long currTime = 0;
unsigned long prevTime = 0;

// RTC 라이브러리 생성(객체 생성)
ThreeWire myWire(DS1302_DAT, DS1302_CLK, DS1302_RST);
RtcDS1302<ThreeWire> Rtc(myWire);


void setup() {
  Serial.begin(9600);
  Rtc.Begin();  // RTC 모듈 라이브러리 시작

  // RTCDateTime 클래스 생성(컴파일된 시간(__DATE__, __TIME__)으로 설정)
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

  // RTC 모듈 초기 세팅
  if (!Rtc.IsDateTimeValid()) 
  {
      Serial.println("RTC lost confidence in the DateTime!");
      Rtc.SetDateTime(compiled);
  }
  // 만일 RTC 모듈이 쓰기 금지 모드라면 쓰기 금지 모드 해제
  if (Rtc.GetIsWriteProtected())
  {
    Serial.println("RTC was write protected, enabling writing now");
    Rtc.SetIsWriteProtected(false);
  }
  // 만일 RTC 모듈이 동작중이 아니라면 시동
  if (!Rtc.GetIsRunning())
  {
    Serial.println("RTC was not actively running, starting now");
    Rtc.SetIsRunning(true);
  }

  // RTC 모듈 시간 세팅
  RtcDateTime now = Rtc.GetDateTime();  // RTC 모듈의 현재 시간 얻기

  if (now < compiled)  // 만일 RTC 모듈의 시간이 현재 시간(컴파일 시간)보다 늦다면 갱신
  {
    Serial.println("RTC is older than compile time!  (Updating DateTime)");
    Rtc.SetDateTime(compiled);
  }
  else if (now > compiled)  //RTC 모듈이 컴파일한 시간보다 빠르다면
  {
    //RTC가 컴파일 시간보다 새것이야(이래야지!)
    Serial.println("RTC is newer than compile time. (this is expected)");
  }
  else if (now == compiled)  //RTC 모듈이 컴파일한 시간과 같다면
  {
    //RTC 모듈과 컴파일 시간이 같아! (그럴리가... 그렇지만 괜찮아..!)
    Serial.println("RTC is the same as compile time! (not expected but all is fine)");
  }
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
  }
}

