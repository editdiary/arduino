// Rtc 시간 모듈
#include <RtcDS1302.h>
#include <ThreeWire.h>

// tds 라이브러리
#include <EEPROM.h>
#include "GravityTDS.h"

#include <SoftwareSerial.h> //소프트웨어 시리얼 라이브러리 추가
#include <DHT.h> //온습도 센서 라이브러리 추가 
#include <U8glib.h> //U8glib 라이브러리 추가 (OLED)
//OLED 라이브러리 추가
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h> //서보 모터 라이브러리 추가
//---[핀 번호 정의] -----------------------------------------

#define LIGHT_CDS A0  // CDS 조도센서 핀 지정(아날로그)
#define TdsSensorPin A1  // (아날로그) tds 모듈

// Rtc 모듈 핀 지정
#define DS1302_CLK 2
#define DS1302_DAT 3
#define DS1302_RST 4

// 워터 펌프 핀 지정
#define AA 5
#define AB 6

#define RED_LED 7  // 붉은 LED를 연결한 핀 번호 -> 경고등
#define YEL_LED 12 // tds 값 높을시 밫나는 LED를 연결한 핀 번호

// 온습도 모듈
#define DHTPIN 8 // 온습도 센서 out 8번 핀으로 설정
#define DHTTYPE DHT11 //온습도 센서 타입 설정

// 모터 모듈
#define DC_MOTOR 9   // PWM을 사용할 핀 (DC)
#define SERVO 11    // 서보모터를 연결한 핀 번호

// 생장 LED(임시)
#define GROWTH_LED 10
//----[객체 생성]-----------------------------------------------
Adafruit_SSD1306 display(-1); //4개의 핀을 가진 OLED 모듈은 RESET 핀이 없으므로 -1로 처리

// RTC 라이브러리 생성(객체 생성)
ThreeWire myWire(DS1302_DAT, DS1302_CLK, DS1302_RST);
RtcDS1302<ThreeWire> Rtc(myWire);

// 0.96인치 128x64 OLED
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);  //SSD1306 128X64 I2C 규격 선택 

DHT dht(DHTPIN, DHTTYPE); //DHT 센서 초기화

Servo servoMotor;

GravityTDS gravityTds;
//-----[변수 설정]-----------------------------------------
// millis()를 위한 시간 변수 설정
unsigned long currTime = 0;
unsigned long sensorTime = 0;
unsigned long growthTime = 0; // 생장 LED control 변수
unsigned long tdsTime = 0; // 마지막으로 TDS 값을 읽은 시간을 저장
unsigned long lastPumpTime = 0;  // 마지막 작동 시작 시간
unsigned long startPumpTime = 0; // 펌프가 시작된 시간

unsigned long lastServoTime = 0; // 서보모터 시간

int rotateState = 0;  //서보 모터(창문) 회전 여부를 결정하는 변수

//펌프 작동을 위한 시간수
const long pumpInterval = 3600000; // 이벤트 간의 시간 (1시간 = 3600000 밀리초)
const long pumpTime = 10000; // 펌프가 작동하는 시간 (10초)
bool pumpRunning = false; // 펌프가 현재 작동 중인지 여부

//tds 작동을 위한 시간수
float temperature = 25, tdsValue = 0;
const long tdsInterval = 1000; // TDS 값을 읽는 간격 (1000ms = 1초)
//-----[setup]---------------------------------------------------
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

  pinMode(SERVO, OUTPUT);
  pinMode(RED_LED, OUTPUT);  // 붉은 LED 핀을 출력으로 설정
  pinMode(DC_MOTOR, OUTPUT); //DC 모터 제어 핀 출력용으로 설정
  servoMotor.attach(SERVO);
  dht.begin();
	u8g.begin();
	
	//tds
  pinMode(YEL_LED, OUTPUT); // LED 핀을 출력으로 설정
  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0); // ADC의 참조 전압 설정, 아두이노 UNO에서 기본값은 5.0V
  gravityTds.setAdcRange(1024); // 10비트 ADC의 경우 1024, 12비트 ADC의 경우 4096
  gravityTds.begin(); // 초기화
	// 워터펌프
	pinMode(AA, OUTPUT);
  pinMode(AB, OUTPUT);

	// 생장LED(임시)
	pinMode(GROWTH_LED, OUTPUT);
}	

//---[loop문]------------------------------------------------
void loop() {
  currTime = millis();  // millis() 시작

  // 각종 센서값 읽기
  RtcDateTime now = Rtc.GetDateTime();  // RTC 모듈로부터 현재 시간 얻기
  int lightValue = analogRead(LIGHT_CDS);  // CDS 조도값
  tdsValue = gravityTds.getTdsValue(); // TDS 값 가져오기
  float humidity = dht.readHumidity();  // 습도
  float temperature = dht.readTemperature();  // 온도
  if (isnan(humidity) || isnan(temperature)) {
    return;
  }

  if (currTime - sensorTime >= 5000)  // 5초 간격으로 센서값 Serial에 표시
  {
    sensorTime = currTime;

    // Serial 시간 표시
    char dt[26];
    sprintf(dt, "%02u/%02u %02u:%02u:%02u", now.Month(), now.Day(), now.Hour(), now.Minute(), now.Second());
    Serial.print(dt);
    Serial.print("  온: ");
		Serial.print(temperature);  // 온도
    Serial.print("  습: ");
		Serial.print(humidity);  // 습도
    Serial.print("  조: ");
    Serial.print(lightValue);  // 조도
    Serial.print("  T: ");
		Serial.println(tdsValue);  // tds
  }

  // OLED
  u8g.firstPage(); //picture loop의 시작
  do {
    u8g.setFont(u8g_font_fub14); //온도, 습도 폰트지정
    u8g.setPrintPos(5, 20); //온도 데이터 출력 커서 설정

    u8g.print(temperature,1); //온도 데이터(소수점 첫째 자리) 출력
    u8g.print("\xb0""C"); //온도 기호(°C) 출력 
    u8g.setPrintPos(70, 20); //습도 데이터 출력 커서 설정
    u8g.print(humidity,1); //습도 데이터(소수점 첫째 자리) 출력
    u8g.print("%"); //습도 기호(%) 출력
    u8g.setFont(u8g_font_fub20); //TDS 폰트지정
    u8g.setPrintPos(40, 55); //TDS 데이터 출력 커서 설정
    u8g.print(tdsValue,1); //tds 데이터(소수점 첫째 자리) 출력
		u8g.print("ppm"); // tds 기호(ppm) 출력
  } while(u8g.nextPage()); //picture loop의 끝  

  // 온습도에 따른 환기팬 동작
  if (temperature >= 26 || humidity >= 60) {
    analogWrite(DC_MOTOR, 128);  // PWM을 사용하여 모터를 반만 틀도록 설정 (0 ~ 255 중 128은 절반)
    digitalWrite(RED_LED, HIGH);  // 붉은 LED 켜기
  } else if (temperature <= 25 && humidity <= 40) {
    analogWrite(DC_MOTOR, 0);
    digitalWrite(RED_LED, LOW);  // 붉은 LED 끄기
  }

  // 1시간에 한 번씩 서보모터(창문 여닫이) 동작
  if (currTime - lastServoTime >= 3600000) {  // 1시간(60분 * 60초 * 1000밀리초)
    // 서보모터 열기 또는 닫기
    if (rotateState == 0) {
      // 서보모터를 90도로 열기
      servoMotor.write(90);
      rotateState = 1;
    } else {
      // 서보모터를 0도로 닫기
      servoMotor.write(0);
      rotateState = 0;
    }
    lastServoTime = millis();  // 서보모터가 동작한 시간 갱신
  }

  //워터펌프
  // 펌프가 작동하지 않고, 다음 작동 시간에 도달했을 때
  if (!pumpRunning && currTime - lastPumpTime >= pumpInterval) {
    pumpRunning = true; // 펌프 작동 시작
    lastPumpTime = currTime ; // 다음 이벤트를 위한 시간 업데이트
    startPumpTime = currTime; // 펌프 시작 시간 기록
    digitalWrite(AA, HIGH);
    digitalWrite(AB, LOW);
  }

  // 펌프가 작동 중이고, 10초가 경과했을 때
  if (pumpRunning && currTime - startPumpTime >= pumpTime) {
    digitalWrite(AA, LOW); // 펌프 작동 정지
    digitalWrite(AB, LOW);
    pumpRunning = false; // 펌프 작동 상태 업데이트
  }

  //tds
  if (currTime - tdsTime >= tdsInterval) { // 마지막 측정 시간으로부터 1초가 경과했는지 확인
      tdsTime = currTime; // 마지막 측정 시간 업데이트

      //temperature = readTemperature(); // 온도 센서를 추가하고 온도를 읽습니다.
      gravityTds.setTemperature(temperature); // 온도 설정 및 온도 보정 실행
      gravityTds.update(); // 샘플링 및 계산
        
      Serial.print(tdsValue, 0);
      Serial.println("ppm");
        
      // TDS 값이 200ppm 이상일 경우 LED 켜기
      if (tdsValue >= 200) {
          digitalWrite(YEL_LED, HIGH); // LED 켜기
      } else {
          digitalWrite(YEL_LED, LOW); // LED 끄기
      }
	  }

  // 생장 led 제어
  if (currTime - growthTime >= 5000)
  {
    growthTime = currTime;
    // 9:00 ~ 18:00 사이 조도값에 따른 LED 제어
	  if (now.Hour() >= 9 && now.Hour() < 18 && lightValue <= 500) {
	    digitalWrite(GROWTH_LED, HIGH); // 낮 동안 조도값이 낮으면 LED 켜기
	  } else if ((now.Hour() >= 18 || now.Hour() < 9) && lightValue > 500) {
	    digitalWrite(GROWTH_LED, LOW); // 저녁이거나 조도값이 높으면 LED 끄기
	  }
  }

}