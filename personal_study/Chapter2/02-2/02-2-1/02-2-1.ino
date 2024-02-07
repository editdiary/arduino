#define LED_BLUE 9
#define LED_RED 8
#define DELAY_TIME 80

void setup() {
  pinMode(LED_BLUE,OUTPUT);
  pinMode(LED_RED,OUTPUT);
}

void loop() {
  digitalWrite(LED_BLUE,HIGH);
  digitalWrite(LED_RED,LOW);
  delay(DELAY_TIME);
  digitalWrite(LED_BLUE,LOW);
  digitalWrite(LED_RED,HIGH);
  delay(DELAY_TIME);
}
