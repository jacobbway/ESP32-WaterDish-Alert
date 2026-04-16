#include <Arduino.h>
#include <Wifi.h>
#include <WiFiClientSecure.h>
#include "HX711_ADC.h"
#include "../secretFolder/password.cpp"

#define ENABLE_SMTP
#define ENABLE_DEBUG
#include <ReadyMail.h>

#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

/**************************************
 *  WIFI and READY Mail Declarations
 **************************************/
WiFiClientSecure ssl_client;
SMTPClient smtp(ssl_client);
const char* ssid = "34_Felonies"; // set to your SSID
const char* wifiPassword = "moderncanoe313"; // set to your SSID password
const char* emailAddress = "jacobbway1997@gmail.com"; // your email
// const char* emailAddress1 = "wholecelerywaterdishemail@gmail.com";
void initWiFi();
void sendEmail();


/**************************************
 *  HX711 Declarations and Functions
 **************************************/
const int HX711_dout = 46; // HX711 d out pin
const int HX711_sck = 16; // HX711 sck pin
HX711_ADC LoadCell(HX711_dout, HX711_sck);
const int calVal_eepromAdress = 0;
unsigned long t = 0;
const float lowerBound = 62; // this is oh the water dish is empty weight
void HX711Setup(); // function declaration


void setup() {
    Serial.begin(57600);
    LoadCell.begin();
    HX711Setup();
    initWiFi();
}

void loop(){
  static boolean newDataReady = 0;
  const int serialPrintInterval = 5000; // wait 5 seconds

  // checks for new data. LoadCell.update() checks if the weight has changed
  if (LoadCell.update()) newDataReady = true;

  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      float i = LoadCell.getData();
      Serial.print("Load_cell output val: ");
      Serial.println(i);
      if(i < lowerBound){
        if(!WiFi.status()) initWiFi(); // ensure I am still connected to WiFi if not reconnect
        sendEmail();
      }
      newDataReady = 0;
      t = millis();
    }
  }

  delay(3600000); // delay loop for an hour
}

/**
 * this is the sets up the HX711 object named LoadCell
 * calibration factor is the raw reading from the scale divided by 
 * a known weight. 
 */
void HX711Setup() {
  Serial.println("\n");
  Serial.println("Starting HX711...");
  float calibrationValue = 763.17; // calibration factor 

  unsigned long stabilizingTime = 10000; // lets the precision improve by allowing more time for stabilizing
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

/**
 * this function connects me to my wifi
 * the WiFi library is a default library to Arduinio Library
 */
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, wifiPassword);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

/**
 * this function sends my email and is built from the Ready Mail library by mobizt
 * https://github.com/mobizt/ReadyMail/tree/main
 * 
 * the getPassword() function is returning my App Password from my google account
 * I have it in a file that isn't uploaded to my repo. because Security.
 * 
 * I have the ability to send two messages but the email to text service is extremely
 * rate limited due to spammer abuse. Thats the main reason behind the hour wait period
 * it only accepts about one email to text around every 3 hours
 * 
 */
void sendEmail() {
  ssl_client.setInsecure();

  auto statusCallback = [](SMTPStatus status) {
    Serial.println(status.text);
  };
  
  smtp.connect("smtp.gmail.com", 465, statusCallback); // validates the smtp server

  if(smtp.isConnected()) {
    smtp.authenticate(emailAddress, getPassword1(), readymail_auth_password); // replace getPassword() with your google app password

    SMTPMessage msg_Jacob;
    msg_Jacob.headers.add(rfc822_from, emailAddress);
    msg_Jacob.headers.add(rfc822_to, getPhoneNumber() + "@tmomail.net");
    msg_Jacob.headers.add(rfc822_subject, "Olive in Drought");
    msg_Jacob.text.body("Olive needs water!");


    configTime(0, 0, "pool.ntp.org");
    while(time(nullptr) < 100000) delay(100);
    msg_Jacob.timestamp = time(nullptr);
    // msg_Sophia.timestamp= time(nullptr);

    smtp.send(msg_Jacob);
    // smtp.send(msg_Sophia);
  }
}