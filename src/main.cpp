#include <Arduino.h>
#include "soc/rtc.h"
#include "HX711.h"

// 1. HX711 circuit wiring // these are the GPIOs on my esp32 
const int LOADCELL_DOUT_PIN = 46;
const int LOADCELL_SCK_PIN = 16;

// // 2. Adjustment settings
// const long LOADCELL_OFFSET = 50682624;
// const long LOADCEL_DIVDER = 5895655;

// 3. Known Weight and known weight reading
// const double knownWeight = 70; // 70 grams
// const long knownReading = 0; // needs to be set once I have the reading.
// const long calibrationFactor = knownReading / knownWeight;

HX711 scale;

/**
 * Setup will be the same in final product
 * 
 * the second loop is the one we will use indefinetly for weighing the water dish
 * 
 * calibration factor = reading / known weight (grams)
 */

void setup() {
    Serial.begin(115200);
    Serial.println("hello 1");
    rtc_cpu_freq_config_t config;
    rtc_clk_cpu_freq_get_config(&config);
    rtc_clk_cpu_freq_mhz_to_config(RTC_CPU_FREQ_80M, &config);
    rtc_clk_cpu_freq_set_config_fast(&config);
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
}

/**
 * Calibration Loop needed for intial setup
 */
void loop() {
  Serial.println("hello");
  if (scale.is_ready()) {
    scale.set_scale();    
    Serial.println("Tare... remove any weights from the scale.");
    delay(5000);
    scale.tare();
    Serial.println("Tare done...");
    Serial.print("Place a known weight on the scale...");
    delay(5000);
    long reading = scale.get_units(10);
    Serial.print("Result: ");
    Serial.println(reading);
  } 
  else {
    Serial.println("HX711 not found.");
  }
  delay(1000);
}

/**
 * Test loop
 */
// void loop() {
//     delay(3600000); // delay 1 hour
// }