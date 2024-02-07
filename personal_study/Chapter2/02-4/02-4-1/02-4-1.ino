#define LED 11
#define VR  A0

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println( analogRead(VR) );
  delay(1000);
}
