#define LED 11
#define VR  A0

void setup() {
  Serial.begin(9600);
}

void loop() {
  analogWrite(LED, 0);
  delay(500);
  analogWrite(LED, 50);
  delay(500);
  analogWrite(LED, 150);
  delay(500);
  analogWrite(LED, 255);
  delay(500);
}
