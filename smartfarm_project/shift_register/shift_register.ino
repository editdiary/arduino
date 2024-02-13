#include <Shifty.h>

Shifty shift;  // shifty 객체 생성

void setup() {
  shift.setBitCount(8)  // 사용하는 Shift Register 개수에 8의 배수를 입
  shift.srtPins(11, 12, 8)  // DS, RCLK, SRCLK의 핀번호 세
}

void loop() {
  // 아래와 같이 마치 digitalWrite처럼 원하는 핀에 신호를 보낼 수 있다.
  shift.writeBit(1, HIGH);
  delay(500);
  shift.writeBit(3, HIGH);
  delay(500);
  shift.writeBit(1, LOW);
  delay(500);
  shift.writeBit(3, LOW);
  delay(500);
}