#include <Servo.h>  //서보 모터 라이브러리 추가

#define SERVO 11    // 서보모터를 연결한 핀 번호

Servo servoMotor;  // 서보모터 객체 선언

unsigned long currTime = 0;
unsigned long lastServoTime = 0;  // 서보모터 제어

int rotateState = 0;  // 서보 모터(창문) 회전 여부 결정


void setup() {
  Serial.begin(115200);
  pinMode(SERVO, OUTPUT);
  servoMotor.attach(SERVO);
}

void loop() {
  currTime = millis();  // millis() 시작

// 10초에 한 번씩 서보모터(창문 여닫이) 동작
  if (currTime - lastServoTime >= 10000) {
    // 서보모터 열기 또는 닫기
    if (rotateState == 0) {
      servoMotor.write(90);  // 서보모터를 90도로 열기
      rotateState = 1;
    } else {
      servoMotor.write(0);   // 서보모터를 0도로 닫기
      rotateState = 0;
    }
    lastServoTime = millis();  // 서보모터가 동작한 시간 갱신
  }

}