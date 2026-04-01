#include <Arduino.h>
#include <Wifi.h>
#include <WiFiClientSecure.h>
#include "../secretFolder/passwordFile.cpp"

#define ENABLE_SMTP
#define ENABLE_DEBUG
#include <ReadyMail.h>

WiFiClientSecure ssl_client;
SMTPClient smtp(ssl_client);


void setup() {
  Serial.begin(115200);
  WiFi.begin("MY_SSID", "SSID_PASSWORD");
  while (WiFi.status() != WL_CONNECTED) delay(500);

  ssl_client.setInsecure();

  auto statusCallback = [](SMTPStatus status) {
    Serial.println(status.text);
  };

  smtp.connect("smtp.gmail.com", 465, statusCallback);

  if(smtp.isConnected()) {
    smtp.authenticate("wholecelerywaterdishemail@gmail.com", getPassword(), readymail_auth_password);

    SMTPMessage msg_Jacob;
    msg_Jacob.headers.add(rfc822_from, "DogDish wholecelerywaterdishemail@gmail.com");
    msg_Jacob.headers.add(rfc822_to, "Jacob 7159659801@tmomail.net");
    msg_Jacob.headers.add(rfc822_subject, "Olive in Drought");
    msg_Jacob.text.body("Olive needs water! She is dying *Palpatine Voice*");
    msg_Jacob.html.body("<html><body><h1>Hello!</h1></body></html>");

    SMTPMessage msg_Sophia;
    msg_Sophia.headers.add(rfc822_from, "DogDish wholecelerywaterdishemail@gmail.com");
    msg_Sophia.headers.add(rfc822_to, "Sophia 9125737@tmomail.net");
    msg_Sophia.headers.add(rfc822_subject, "Olive in Drought");
    msg_Sophia.text.body("Olive needs water! She is dying *Palpatine Voice*");
    msg_Sophia.html.body("<html><body><h1>Hello!</h1></body></html>");

    configTime(0, 0, "pool.ntp.org");
    while(time(nullptr) < 100000) delay(100);
    msg_Jacob.timestamp = time(nullptr);
    msg_Sophia.timestamp= time(nullptr);

    smtp.send(msg_Jacob);
    smtp.send(msg_Sophia);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}