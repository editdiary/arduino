#define LIGHT_CDS A0  // CDS 조도센서 핀 지정(아날로그)

unsigned long currTime = 0;
unsigned long sensorTime = 0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  currTime = millis();  // millis() 시작
  int lightValue = analogRead(LIGHT_CDS);  // CDS 조도값

  if (currTime - sensorTime >= 2000) {
    sensorTime = currTime;
    Serial.print("조도: ")
    Serial.println(lightValue)
  }
}