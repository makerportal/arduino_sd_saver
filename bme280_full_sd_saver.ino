#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

File main_folder; // initialize folder for saving
File dataFile; // initialize sd file
const int chipSelect = 4; // CS pin on sd card module
int prev_file_indx = 0; // used for file naming
String fileName = "000";

// measurements taken at r_0 (sea level)
const float station_elev = 11.0; // station elevation (KNYC used here)
const float p_0 = 1017.3*100.0;

Adafruit_BME280 bme; // start BME sensor

void setup() {
//    Serial.begin(9600); // use serial port if printout is desired
    // verify BME280 is working
    bool status;
    status = bme.begin();  
    if (!status) {
        while (1);
    }
    // verify SD card is working
    if (!SD.begin(chipSelect)) {
      return;
    }
    main_folder = SD.open("/");
    fileName = sd_saver(main_folder);
}

void loop() {  
  // SD save section
  String data_array = "";
  data_array += String(millis()); // save milliseconds since start of program
  data_array += ",";
  data_array += String(bme.readTemperature()); // save temp
  data_array += ",";
  data_array += String(bme.readHumidity()); // save humidity
  data_array += ",";
  data_array += String(bme.readPressure()); // save pressure in Pa
  data_array += ",";
  data_array += String(bme.readAltitude(p_0/100.0)+station_elev); // save altitude from Adafruit routine

  // SD Card writing and saving  
  dataFile = SD.open("DATA"+fileName+".csv",FILE_WRITE);
  // if the file is valid, write to it:
  if(dataFile){
    dataFile.println(data_array);
    dataFile.close();
  }
  delay(100);
}

String sd_saver(File dir){
  while(true){
    // iterate all files to ensure no overwrites
    File entry = dir.openNextFile();
    if (!entry){
      break;
    }
    // naming routine
    String entry_name = entry.name();
    if ((entry_name.substring(4,7)).toInt()>=prev_file_indx){
      prev_file_indx = (entry_name.substring(4,7)).toInt()+1;
      if (prev_file_indx>=100){
        fileName = String(prev_file_indx);
      } else if (prev_file_indx>=10){
        fileName = "0"+String(prev_file_indx);
      } else{
        fileName = "00"+String(prev_file_indx);
      }
    }
    entry.close();
  }
  return fileName;
}
