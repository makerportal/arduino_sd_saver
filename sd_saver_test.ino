#include <SPI.h>
#include <SD.h>

File testfile;
String fileName = "test.csv";
volatile int int_iter = 0;

void setup() {
  Serial.begin(9600); // start serial port
  // wait for SD module to start
  if (!SD.begin(4)) {
    Serial.println("No SD Module Detected");
    while (1);
  }
  // see if test file exists, delete it if it does
  // then prints headers and starts new one
  if (SD.exists(fileName)){
    Serial.println(fileName+" exists, deleting and starting new");
    SD.remove(fileName);
  }
  testfile = SD.open(fileName, FILE_WRITE);
  if (testfile) {
    // save headers to file
    testfile.println("Timestamp,Data");
    testfile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening file");
  }
}

void loop() {
  // save new integer every loop and then wait 1s
  testfile = SD.open(fileName, FILE_WRITE);
  if (testfile) {
    // save a different number each loop
    testfile.println(String(millis())+","+String(int_iter));
    testfile.close();
    Serial.println("Saving "+String(int_iter));
  } else {
    Serial.println("error opening file");
  }
  int_iter+=1;
  delay(1000);
}
