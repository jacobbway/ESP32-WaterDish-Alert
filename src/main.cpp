#include <Arduino.h>
#include <Wifi.h>
#include <WiFiClientSecure.h>
#include "HX711_ADC.h"
#include "../secretFolder/password.cpp"

#define ENABLE_SMTP
#define ENABLE_DEBUG
#include <ReadyMail.h>

/**************************************
 *  WIFI and READY Mail Declarations
 **************************************/
WiFiClientSecure ssl_client;
SMTPClient smtp(ssl_client);
const char* ssid = "34_Felonies";
const char* wifiPassword = "moderncanoe313";
const char* emailAddress = "jacobbway1997@gmail.com"; // wholecelerywaterdishemail@gmail.com
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
    HX711Setup();
    initWiFi();
}

void loop() {
    LoadCell.powerUp();
    float value = LoadCell.getData();
    if(value < lowerBound){
        Serial.println(value);
        //sendEmail();
    }
    LoadCell.powerDown();
    delay(15000); // 15 seconds for test going to make it an hour in use // 3600000 is an hour
}

void HX711Setup() {
  Serial.println("\n");
  Serial.println("Starting HX711...");
  LoadCell.begin();
  float calibrationValue = 763.17; // 763.17 = calibration factor without plate on it

  unsigned long stabilizingTime = 2000; // lets the precision improve by allowing more time for stabilizing
  boolean _tare = false;
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

void sendEmail() {
  ssl_client.setInsecure();

  auto statusCallback = [](SMTPStatus status) {
    Serial.println(status.text);
  };
  
  smtp.connect("smtp.gmail.com", 465, statusCallback);

  if(smtp.isConnected()) {
    smtp.authenticate(emailAddress, getPassword(), readymail_auth_password);

    SMTPMessage msg_Jacob;
    msg_Jacob.headers.add(rfc822_from, emailAddress);
    msg_Jacob.headers.add(rfc822_to, "7159659201@tmomail.net");
    msg_Jacob.headers.add(rfc822_subject, "Olive in Drought");
    msg_Jacob.text.body("Olive needs water!");
    // msg_Jacob.html.body("<html><body><h1>Olive needs water!</h1></body></html>");

    // SMTPMessage msg_Sophia;
    // msg_Sophia.headers.add(rfc822_from, emailAddress);
    // msg_Sophia.headers.add(rfc822_to, "9124095737@tmomail.net");
    // msg_Sophia.headers.add(rfc822_subject, "Olive in Drought");
    // msg_Sophia.text.body("Olive needs water!");
    // msg_Sophia.html.body("<html><body><h1>Olive needs water!</h1></body></html>");

    configTime(0, 0, "pool.ntp.org");
    while(time(nullptr) < 100000) delay(100);
    msg_Jacob.timestamp = time(nullptr);
    // msg_Sophia.timestamp= time(nullptr);

    smtp.send(msg_Jacob);
    // smtp.send(msg_Sophia);
  }
}