#include "loadCell.h"
// HX711 circuit wiring
const int DOUT_PIN = 16;
const int SCK_PIN = 4;
const float CalibrationFactor = 0.46f; // 2280.f
const float plateWeight = 500.f; // 0.5 kg
float curWeight = 0;

HX711 scale;
void setup_loadCellCalibration() 
{
  scale.begin(DOUT_PIN, SCK_PIN);
}

void loop_loadCellCalibration() 
{
  scale.set_scale();    
  Serial.println("Reset scale to 0... remove any weights from scale.");
  delay(5000);
  scale.tare();
  Serial.println("Successfully reset scale");
  Serial.print("Place the plate on the scale...");
  delay(5000);
  long reading = scale.get_units(10);
  Serial.print("Result: ");
  Serial.println(reading);
  Serial.println("Actual calibration would be: "); 
  Serial.println(reading / plateWeight);
  delay(1000);
}
void setup_loadCell() 
{
  Serial.println("Initializing the scale");
  scale.begin(DOUT_PIN, SCK_PIN);

  Serial.println("Average of 5 readings testing before setting scale ");
  Serial.println(scale.get_units(5), 1);         
  scale.set_scale(CalibrationFactor); //calibration factor will be the (reading)/(known weight)
  scale.tare(); // reset the scale to 0

  Serial.println("Average of 5 readings after setting up the scale ");
  Serial.println(scale.get_units(5), 1);
  Serial.println("Now reading for real...");
}
float getCurWeight() {return curWeight;}
void loop_loadCell() 
{
  curWeight = scale.get_units(10);
  scale.power_down(); // put the ADC in sleep mode
  // delay(100);
  scale.power_up();
}