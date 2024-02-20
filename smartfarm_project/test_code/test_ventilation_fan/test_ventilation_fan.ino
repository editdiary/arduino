#include <DHT.h> //온습도 센서 라이브러리 추가 

// 온습도 모듈
#define DHTPIN 8 // 온습도 센서 out 8번 핀으로 설정
#define DHTTYPE DHT11 //온습도 센서 타입 설정

#define DC_MOTOR  5
#define MOTOR_A_1B  6

DHT dht(DHTPIN, DHTTYPE); //DHT 센서 초기화

// millis()를 위한 시간 변수 설정
unsigned long currTime = 0;
unsigned long sensorTime = 0;

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  currTime = millis();  // millis() 시작

  // 각종 센서값 읽기
  float humidity = dht.readHumidity();  // 습도
  float temperature = dht.readTemperature();  // 온도
  if (isnan(humidity) || isnan(temperature)) {
    return;
  }

  // 각종 센서값 표시
  if (currTime - sensorTime >= 5000) {
    sensorTime = currTime;
    Serial.print("  온: ");
		Serial.print(temperature);  // 온도
    Serial.print("  습: ");
		Serial.println(humidity);  // 습도
  }

  // 온습도에 따른 환기팬 동작
  if (temperature >= 26 || humidity >= 60) {
    analogWrite(DC_MOTOR,150);  // 세기는 0~255 사이 값
    analogWrite(MOTOR_A_1B,0);
  } else if (temperature <= 25 && humidity <= 40) {
    analogWrite(DC_MOTOR,0);
    analogWrite(MOTOR_A_1B,0);
  }

}