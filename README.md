# ESP32-WaterDish-Alert
This is a project I created and developed for my WorkStation and Server Final. I am using an ESP32-S3-Devkitc, HX711 Amplifier, and a 3kg load cell. I combined this hardware with two libraries that provide me with the abilty to send an email and one that adds functions for my HX711/Load Sensor.

# Ready Mail
This is the library I found that allows me to connect to an SMTP service and send an email. In the code I am using the email to text gateway provided by T-Mobile
https://github.com/mobizt/ReadyMail/tree/main

# HX711_ADC
This library gives me the functions I need to use my HX711 Amplifier and load cell
https://github.com/olkal/HX711_ADC

# Next Steps
I need to find a new way to convert my emails to text better, the service provided by T-Mobile only allows about one email-to-text to pass through about once every four hours or so. Or I drop the email to text thing and have it just send emails. Another option would be building an app that notifies me or something.

# Currently Working On
I am currently designing a structure in AutoCad that will house my setup all in one. I am hoping to include a place to mount the esp32/hx711/load cell as well as battery powered setup for the system that way I don't have to have it plugging into the wall.
