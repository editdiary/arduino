#define LIGHT_CDS A0
#define LED 11

int cnt = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
}

void loop() {
  int lightValue = analogRead(LIGHT_CDS);
  Serial.println(lightValue);
  if(lightValue <= 400)
  {
    if(cnt >=3) digitalWrite(LED, HIGH);
    else cnt++;
    delay(1000);
  }
  else
  {
    cnt = 0;
    digitalWrite(LED, LOW);
  }
}
