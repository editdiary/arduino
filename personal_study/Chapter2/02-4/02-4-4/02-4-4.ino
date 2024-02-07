#define LED 11
#define VR  A0

void setup() {
  Serial.begin(9600);
}

void loop() {
  int analogValue = analogRead(VR);
  int analogMapping = map(analogValue,0,1023,0,255);
  analogWrite(LED, analogMapping);
}
