#include <SoftwareSerial.h>  //소프트웨어 시리얼 라이브러리 추가
#include <DHT.h>    //온습도 센서 라이브러리 추가

#define RED_LED 7   // 온습도 경고등(적색 LED) - 온습도가 지나치게 높을 때 점등

// 온습도 센서
#define DHTPIN 8  // 온습도 센서 핀으로 설정
#define DHTTYPE DHT11  //온습도 센서 타입 설정

#define DC_MOTOR 9  // DC모터를 제어할 릴레이 모듈 핀 번호

DHT dht(DHTPIN, DHTTYPE);  // DHT 센서 초기화

unsigned long currTime = 0;
unsigned long sensorTime = 0;     // sensor print 제어

void setup() {
  Serial.begin(115200);

	pinMode(RED_LED, OUTPUT);  // 붉은 LED 핀을 출력으로 설정
  pinMode(DC_MOTOR, OUTPUT); // DC 모터 제어 핀 출력용으로 설정
	dht.begin();  // 온습도 모듈 시작 선언
}

void loop() {
  currTime = millis();  // millis() 시작

	float humidity = dht.readHumidity();        // 습도
  float temperature = dht.readTemperature();  // 온도
  if (isnan(humidity) || isnan(temperature)) {
    return;
  }

  if (currTime - sensorTime >= 5000) {  // 5초 간격으로 센서값 Serial에 표시
    sensorTime = currTime;

	Serial.print("온도: ");
		Serial.print(temperature);  // 온도
    Serial.print("C");
    Serial.print("  습도: ");
		Serial.print(humidity);  // 습도
    Serial.println("%");
  }

	// 온습도에 따른 환기팬 동작
  if (temperature >= 26 || humidity >= 60) {
    digitalWrite(DC_MOTOR, HIGH);  // 릴레이 모듈을 사용하여 DC모터(환기팬) 작동
    digitalWrite(RED_LED, HIGH);   // 경고등 켜기
  } else if (temperature <= 25 && humidity <= 40) {
    digitalWrite(DC_MOTOR, LOW);  // DC모터(환기팬) 작동 중지
    digitalWrite(RED_LED, LOW);   // 경고등 끄기
  }

}