#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;

void setup() {
  Serial.begin(9600);
  if (!SD.begin(chipSelect)) while (1);
  Serial.println("init sd card");
  File dataFile = SD.open("test.txt", FILE_WRITE);

  if (dataFile) {
    dataFile.println("arduino sd test");
    dataFile.close();
  }
}

void loop() {
  
}
