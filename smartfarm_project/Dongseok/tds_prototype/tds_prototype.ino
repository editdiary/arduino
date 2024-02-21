#include <EEPROM.h>
#include "GravityTDS.h"

#define TdsSensorPin A1
#define LedPin 12 // LED를 연결한 핀 번호

GravityTDS gravityTds;

float temperature = 25, tdsValue = 0;
unsigned long previousMillis = 0; // 마지막으로 TDS 값을 읽은 시간을 저장
const long interval = 1000; // TDS 값을 읽는 간격 (1000ms = 1초)

void setup() {
    Serial.begin(115200);
    pinMode(LedPin, OUTPUT); // LED 핀을 출력으로 설정
    gravityTds.setPin(TdsSensorPin);
    gravityTds.setAref(5.0); // ADC의 참조 전압 설정, 아두이노 UNO에서 기본값은 5.0V
    gravityTds.setAdcRange(1024); // 10비트 ADC의 경우 1024, 12비트 ADC의 경우 4096
    gravityTds.begin(); // 초기화
}

void loop() {
    unsigned long currentMillis = millis();
    
    if (currentMillis - previousMillis >= interval) { // 마지막 측정 시간으로부터 1초가 경과했는지 확인
        previousMillis = currentMillis; // 마지막 측정 시간 업데이트

        //temperature = readTemperature(); // 온도 센서를 추가하고 온도를 읽습니다.
        gravityTds.setTemperature(temperature); // 온도 설정 및 온도 보정 실행
        gravityTds.update(); // 샘플링 및 계산
        tdsValue = gravityTds.getTdsValue(); // TDS 값 가져오기
        
        Serial.print(tdsValue, 0);
        Serial.println("ppm");
        
        // TDS 값이 200ppm 이상일 경우 LED 켜기
        if (tdsValue >= 200) {
            digitalWrite(YLedPin, HIGH); // LED 켜기
        } else {
            digitalWrite(LedPin, LOW); // LED 끄기
        }
    }
}