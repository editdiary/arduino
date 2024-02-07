#define LIGHT_CDS A0
#define LED 11

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
}

void loop() {
  int lightValue = analogRead(LIGHT_CDS);
  Serial.println(lightValue);
  if(lightValue <= 400)
  {
    digitalWrite(LED, HIGH);
  }
  else
  {
    digitalWrite(LED, LOW);
  }
}
