#include <Arduino.h>
#include "HX711_ADC.h"

#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

const int HX711_dout = 46; // HX711 d out pin
const int HX711_sck = 16; // HX711 sck pin
HX711_ADC LoadCell(HX711_dout, HX711_sck);
const int calVal_eepromAdress = 0;
unsigned long t = 0;
const int lowerBound = 62; // this is oh the water dish is empty weight

void HX711Setup(); // function declaration

void setup(){
  Serial.begin(57600);
  LoadCell.begin();
  HX711Setup();
}

void loop(){
  static boolean newDataReady = 0;
  const int serialPrintInterval = 5000; // wait 5 seconds

  // check for new data/start next conversion:
  if (LoadCell.update()) newDataReady = true;

  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      float i = LoadCell.getData();
      Serial.print("Load_cell output val: ");
      Serial.println(i);
      if(i < lowerBound){
        Serial.println("Water Dog");
      }
      newDataReady = 0;
      t = millis();
    }
  }
}


// 62 grams
void HX711Setup() {
  Serial.println("\n");
  Serial.println("Starting HX711...");

  float calibrationValue = 763.17; // 763.17 = calibration factor without plate on it

  unsigned long stabilizingTime = 2000; // lets the precision improve by allowing more time for stabilizing
  boolean _tare = true;
  LoadCell.start(stabilizingTime, _tare);

  if(LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check wiring");
    while(1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue); // sets the calibration factor ( reading / known weight )
    Serial.println("HX711 Setup complete!");
  }
}