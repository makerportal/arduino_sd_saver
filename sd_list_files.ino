#include <SPI.h>
#include <SD.h>
File root;
void setup() {
  Serial.begin(9600);
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  root = SD.open("/");
  while(true){
    File entry = root.openNextFile();
    if (!entry){
      break;
    }
    Serial.println(entry.name());
  }
}
void loop() {
  // nothing happens after setup finishes.
}
