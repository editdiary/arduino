#define LIGHT_CDS A0
#define LED 11

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
}

void loop() {
  Serial.println( analogRead(LIGHT_CDS) );
  delay(1000);
}
