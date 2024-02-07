#include "DHT.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define LED_RED   11
#define LED_GREEN 10
#define LED_BLUE  9

#define DHTPIN 3

#define DHTTYPE DHT11  // 센서 종류 설정(DHT11 or DHT22)

DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27,20,4);  //0x27이 안 되면 0x3F로 해볼 

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.init();
  lcd.backlight();
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
}

void loop() {
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();

  Serial.print(temp);Serial.print("C");
  Serial.print("    ");
  Serial.print(humi);Serial.println("%");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TEMP: ");
  lcd.print(temp);
  lcd.setCursor(0, 1);
  lcd.print("HUMI: ");
  lcd.print(humi);
  
  float di = (1.8 * temp) - (0.55 * (1 - humi / 100.0) * (1.8 * temp - 26)) + 32;
  Serial.print("discomfort index: ");
  Serial.println(di);
  
  if(di <= 69)
  {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_BLUE, LOW);
  }
  else if(di >= 70 && di <= 75)
  {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_BLUE, LOW);
  }
  else if(di >= 76)
  {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, LOW);
  }
  delay(1500);
}
