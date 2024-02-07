#define CAR_LED_RED     12
#define CAR_LED_YELLOW  11
#define CAR_LED_GREEN   10
#define HUMAN_LED_RED   3
#define HUMAN_LED_GREEN 2

void setup() {
  pinMode(CAR_LED_RED,OUTPUT);
  pinMode(CAR_LED_YELLOW,OUTPUT);
  pinMode(CAR_LED_GREEN,OUTPUT);
  pinMode(HUMAN_LED_RED,OUTPUT);
  pinMode(HUMAN_LED_GREEN,OUTPUT);
}

void loop() {
  digitalWrite(CAR_LED_RED,HIGH);
  digitalWrite(CAR_LED_YELLOW,HIGH);
  digitalWrite(CAR_LED_GREEN,HIGH);
  digitalWrite(HUMAN_LED_RED,HIGH);
  digitalWrite(HUMAN_LED_GREEN,HIGH);
  delay(1000);
  digitalWrite(CAR_LED_RED,LOW);
  digitalWrite(CAR_LED_YELLOW,LOW);
  digitalWrite(CAR_LED_GREEN,LOW);
  digitalWrite(HUMAN_LED_RED,LOW);
  digitalWrite(HUMAN_LED_GREEN,LOW);
  delay(1000);
}
