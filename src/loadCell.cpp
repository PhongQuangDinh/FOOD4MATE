#include "loadCell.h"
// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 16;
const int LOADCELL_SCK_PIN = 4;
const float plate_weight = 1.73;

HX711 scale;

void setup_loadCell() {
  Serial.begin(115200);
  //rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
}

void loop_loadCell() {

  if (scale.is_ready()) 
  {
    scale.set_scale(plate_weight);    // minus the weight that we already know
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

//calibration factor will be the (reading)/(known weight)