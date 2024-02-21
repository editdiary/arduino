#include <SoftwareSerial.h> //소프트웨어 시리얼 라이브러리 추가 
#include <DHT.h> //온습도 센서 라이브러리 추가 
#include <U8glib.h> //U8glib 라이브러리 추가 (OLED)
//OLED 라이브러리 추가
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(-1); //4개의 핀을 가진 OLED 모듈은 RESET 핀이 없으므로 -1로 처리

// 블루투스 모듈
#define TXD 2 //TXD를 2번 핀으로 설정
#define RXD 3 //RXD를 3번 핀으로 설정

// 온습도 모듈
#define DHTPIN 8 // 온습도 센서 out 8번 핀으로 설정
#define DHTTYPE DHT11 //온습도 센서 타입 설정

#define RED_LED 7  // 붉은 LED를 연결한 핀 번호
#define DC_MOTOR 9   // PWM을 사용할 핀 (DC)
#define SERVO 11    // 서보모터를 연결한 핀 번호

// 0.96인치 128x64 OLED
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);  //SSD1306 128X64 I2C 규격 선택 

SoftwareSerial mySerial(TXD, RXD); //소프트웨어 시리얼 mySerial 객체 선언
DHT dht(DHTPIN, DHTTYPE); //DHT 센서 초기화

int rotateState = 0;  //서보 모터(창문) 회전 여부를 결정하는 변수 

unsigned long lastServoTime = 0;

Servo servoMotor;

void setup() 
  pinMode(SERVO, OUTPUT);
  pinMode(RED_LED, OUTPUT);  // 붉은 LED 핀을 출력으로 설정
  pinMode(DC_MOTOR, OUTPUT); //DC 모터 제어 핀 출력용으로 설정
  servoMotor.attach(SERVO);
  dht.begin();
	u8g.begin();
  Serial.begin(9600);
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    return;
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
  if (millis() - lastServoTime >= 3600000) {  // 1시간(60분 * 60초 * 1000밀리초)
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

}