const int AA = 10;
const int AB = 6;

unsigned long previousMillis = 0; // 마지막 작동 시작 시간
unsigned long pumpStartMillis = 0; // 펌프가 시작된 시간
const long interval = 3600000; // 이벤트 간의 시간 (1시간 = 3600000 밀리초)
const long pumpTime = 10000; // 펌프가 작동하는 시간 (10초)
bool pumpRunning = false; // 펌프가 현재 작동 중인지 여부

void setup() {
  pinMode(AA, OUTPUT);
  pinMode(AB, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();

  // 펌프가 작동하지 않고, 다음 작동 시간에 도달했을 때
  if (!pumpRunning && currentMillis - previousMillis >= interval) {
    pumpRunning = true; // 펌프 작동 시작
    previousMillis = currentMillis; // 다음 이벤트를 위한 시간 업데이트
    pumpStartMillis = currentMillis; // 펌프 시작 시간 기록
    digitalWrite(AA, HIGH);
    digitalWrite(AB, LOW);
  }

  // 펌프가 작동 중이고, 10초가 경과했을 때
  if (pumpRunning && currentMillis - pumpStartMillis >= pumpTime) {
    digitalWrite(AA, LOW); // 펌프 작동 정지
    digitalWrite(AB, LOW);
    pumpRunning = false; // 펌프 작동 상태 업데이트
  }
}