#define LED 11
#define VR  A0

void setup() {
  Serial.begin(9600);
}

void loop() {
  int analogValue = analogRead(VR);
  analogWrite(LED, analogValue / 4);
}
