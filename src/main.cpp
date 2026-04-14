#include <Arduino.h>
#include "HX711_ADC.h"
#include <EEPROM.h>

const int HX711_dout = 46; // HX711 d out pin
const int HX711_sck = 16; // HX711 sck pin
HX711_ADC LoadCell(HX711_dout, HX711_sck);
const int calVal_eepromAdress = 0;
unsigned long t = 0;
const int lowerBound = 62; // this is oh the water dish is empty weight

void HX711Setup(); // function declaration

void setup(){
  Serial.begin(57600);
  HX711Setup();
  
}

void loop(){
  delay(2000);
  LoadCell.powerUp();
  if(LoadCell.update() < lowerBound){
    Serial.println("\n\n\nWater Dish is Empty!\n");
  }
  LoadCell.powerDown();
}


// 62 grams
void HX711Setup() {
  Serial.println("\n\n\n");
  Serial.println("Starting HX711...");
  LoadCell.begin();
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