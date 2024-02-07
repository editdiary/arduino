#include "DHT.h"

#define DHTPIN 3

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();

  Serial.print(temp);Serial.print("C");
  Serial.print("    ");
  Serial.print(humi);Serial.println("%");

  delay(1500);
}
