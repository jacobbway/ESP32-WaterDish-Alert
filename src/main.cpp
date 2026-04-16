#include <Arduino.h>
#include <Wifi.h>
#include <WiFiClientSecure.h>
#include "HX711.h"
#include "soc/rtc.h"
#include "../secretFolder/password.cpp"

#define ENABLE_SMTP
#define ENABLE_DEBUG
#include <ReadyMail.h>

WiFiClientSecure ssl_client;
SMTPClient smtp(ssl_client);

// // 1. HX711 circuit wiring
// const int LOADCELL_DOUT_PIN = 2;
// const int LOADCELL_SCK_PIN = 3;

// // 2. Adjustment settings
// const long LOADCELL_OFFSET = 50682624;
// const long LOADCEL_DIVDER = 5895655;

const char* ssid = "34_Felonies";
const char* wifiPassword = "moderncanoe313";
const char* emailAddress = "jacobbway1997@gmail.com"; // wholecelerywaterdishemail@gmail.com

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


void setup() {
  Serial.begin(115200);
  Serial.println("hello!");
  initWiFi();
}

/**
 * this loop should run when the water dish reaches its threshold. then you have to hit the reset button to start again.
 */
void loop() {

}