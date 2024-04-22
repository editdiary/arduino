//-----[라이브러리 import]------------------------------------
// Rtc 시간 모듈
#include <RtcDS1302.h>
#include <ThreeWire.h>

// tds 라이브러리
#include <EEPROM.h>
#include "GravityTDS.h"

#include <SoftwareSerial.h>  //소프트웨어 시리얼 라이브러리 추가
#include <DHT.h>    //온습도 센서 라이브러리 추가

//OLED 라이브러리 추가
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>


//-----[핀 번호 정의] ----------------------------------------
#define LIGHT_CDS A0  // CDS 조도센서(아날로그)
#define TDS_PIN A1    // tds 모듈(아날로그)

// Rtc 모듈 핀 지정
#define DS1302_CLK 2
#define DS1302_DAT 3
#define DS1302_RST 4

#define RED_LED 7   // 온습도 경고등(적색 LED) - 온습도가 지나치게 높을 때 점등
#define YEL_LED 6   // tds 경고등(황색 LED) - tds가 지나치게 높을 때 점등

// 온습도 센서
#define DHTPIN 8       // 온습도 센서 핀으로 설정
#define DHTTYPE DHT11  //온습도 센서 타입 설정

// 모터 모듈
#define DC_MOTOR 9     // DC모터를 제어할 릴레이 모듈 핀 번호

// 생장 LED
#define GROWTH_LED 10


//-----[객체 생성]--------------------------------------------
// 0.96인치 128x64 OLED
Adafruit_SSD1306 display(-1);  //4개의 핀을 가진 OLED 모듈은 RESET 핀이 없으므로 -1로 처리

// RTC 라이브러리 생성(객체 생성)
ThreeWire myWire(DS1302_DAT, DS1302_CLK, DS1302_RST);
RtcDS1302<ThreeWire> Rtc(myWire);

DHT dht(DHTPIN, DHTTYPE);  // DHT 센서 초기화

GravityTDS gravityTds;     // tds 센서 객체 선언


//-----[변수 설정]--------------------------------------------
// millis()를 위한 시간 변수 설정
unsigned long currTime = 0;
unsigned long sensorTime = 0;     // sensor print 제어
unsigned long growthTime = 0;     // 생장 LED 제어
unsigned long tdsTime = 0;        // TDS 센서 제어


//-----[setup]------------------------------------------------
void setup() {
  Serial.begin(115200);

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

	pinMode(RED_LED, OUTPUT);     // 붉은 LED 핀을 출력으로 설정
  pinMode(DC_MOTOR, OUTPUT);    // DC 모터 제어 핀 출력용으로 설정
  pinMode(GROWTH_LED, OUTPUT);  // 생장 LED 핀 출력용으로 설정
  dht.begin();                  // 온습도 모듈 시작 선언

	//tds
  pinMode(YEL_LED, OUTPUT);      // tds 경고등 출력으로 설정
  gravityTds.setPin(TDS_PIN);
  gravityTds.setAref(5.0);       // ADC의 참조 전압 설정, 아두이노 UNO에서 기본값은 5.0V
  gravityTds.setAdcRange(1024);  // 10비트 ADC의 경우 1024, 12비트 ADC의 경우 4096
  gravityTds.begin();            // 초기화

  // 0x3C주소로 디스플레이 장치를 초기화
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
}


//-----[loop문]-----------------------------------------------
void loop() {
  currTime = millis();  // millis() 시작

	// 각종 센서값 읽기
  RtcDateTime now = Rtc.GetDateTime();        // RTC 모듈로부터 현재 시간 얻기
  int lightValue = analogRead(LIGHT_CDS);     // CDS 조도값
  float tdsValue = gravityTds.getTdsValue();  // TDS 값 가져오기
  float humidity = dht.readHumidity();        // 습도
  float temperature = dht.readTemperature();  // 온도
  // 온습도를 읽지 못하는 경우 반복문 건너뛰기
  if (isnan(humidity) || isnan(temperature)) {
    return;
  }

  // 5초 간격으로 OLED 및 Serial에 센서값 표시
	if (currTime - sensorTime >= 5000) {
    sensorTime = currTime;

    // OLED
    display.clearDisplay();  // 화면 초기화
    display.setTextSize(2);  // 폰트 크기 설정
    display.setTextColor(WHITE);
    display.setCursor(0, 0);       // 출력 위치 설정
    display.print(temperature,1);  // 온도 데이터(소수점 첫째 자리) 출력
    display.println("C");          // 온도 기호(C) 출력 
    display.print(humidity,1);     // 습도 데이터(소수점 첫째 자리) 출력
    display.println("%");          // 습도 기호(%) 출력
    display.print(tdsValue,1);     // tds 데이터(소수점 첫째 자리) 출력
		display.println("ppm");        // tds 기호(ppm) 출력
    display.print(now.Hour());
    display.print("h ");
    display.print(now.Minute());
    display.println("m");
    display.display(); 

    // Serial
    char dt[26];
    sprintf(dt, "%02u/%02u %02u:%02u:%02u", now.Month(), now.Day(), now.Hour(), now.Minute(), now.Second());
    Serial.print(dt);
    Serial.print("  온도: ");
		Serial.print(temperature);  // 온도
    Serial.print("C");
    Serial.print("  습도: ");
		Serial.print(humidity);  // 습도
    Serial.print("%");
    Serial.print("  조도: ");
    Serial.print(lightValue);  // 조도
    Serial.print("  TDS: ");
		Serial.print(tdsValue, 0);  // tds
    Serial.println("ppm");
  }

	// 온습도에 따른 환기팬 제어
  if (temperature >= 27.0 || humidity >= 60) {
    digitalWrite(DC_MOTOR, HIGH);  // 릴레이 모듈을 사용하여 DC모터(환기팬) 작동
    digitalWrite(RED_LED, HIGH);   // 경고등 켜기
  } else if (temperature <= 26.3 && humidity <= 40) {
    digitalWrite(DC_MOTOR, LOW);   // DC모터(환기팬) 작동 중지
    digitalWrite(RED_LED, LOW);    // 경고등 끄기
  }

	// 10초마다 tds 보정 및 경고등 제어
  if (currTime - tdsTime >= 10000) {  // 마지막 측정 시간으로부터 10초가 경과했는지 확인
    tdsTime = currTime;  // 마지막 측정 시간 업데이트

    gravityTds.setTemperature(temperature);  // 온도 설정 및 온도 보정 실행
    gravityTds.update();  // 샘플링 및 계산

    // TDS 값이 600ppm 이상일 경우 경고등(황색) 켜기
    if (tdsValue >= 800) {
        digitalWrite(YEL_LED, HIGH);  // 경고등(황색) 켜기
    } else {
        digitalWrite(YEL_LED, LOW);   // 경고등(황색) 끄기
    }
	}

	// 식물 생장 LED 제어
  if (currTime - growthTime >= 5000)
  {
    growthTime = currTime;
    // 6:00 ~ 21:00 사이 조도값에 따른 LED 제어
	  if (now.Hour() >= 6 && now.Hour() < 21 && lightValue <= 800) {
	    digitalWrite(GROWTH_LED, HIGH);  // 낮 동안 조도값이 낮으면 LED 켜기
	  } else if ((now.Hour() >= 21 || now.Hour() < 6) && lightValue > 800) {
	    digitalWrite(GROWTH_LED, LOW);   // 밤 동안 조도값이 높으면 LED 끄기
	  }
  }

}
