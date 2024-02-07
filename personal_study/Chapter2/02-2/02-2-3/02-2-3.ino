#define BUTTON  4

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON,INPUT_PULLUP);
}

void loop() {
  int buttonValue = !digitalRead(BUTTON);
  Serial.println(buttonValue);
  delay(10);
}
