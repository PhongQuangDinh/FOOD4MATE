#include <Arduino.h>

// Khai báo chân kết nối đến module điều khiển động cơ DC (L298N)
const int enablePin = 9;  // Chân enable để điều khiển tốc độ động cơ
const int motorPin1 = 13; // Chân IN1 điều khiển chiều quay động cơ
const int motorPin2 = 12; // Chân IN2 điều khiển chiều quay động cơ

void setupDCMotor();
void loopDCMotor();
void openFoodTray();
void closeFoodTray();
void turnOnLed();