#include <Arduino.h>
#include "HX711.h"
#include "soc/rtc.h"

void setup_loadCellCalibration();
void loop_loadCellCalibration();
void setup_loadCell();
void loop_loadCell();
float getCurWeight();