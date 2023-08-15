#include <Arduino.h>

// Khai báo chân kết nối đến module điều khiển động cơ DC (L298N)
const int motorPin1 = 13; // Chân IN1 điều khiển chiều quay động cơ khay
const int motorPin2 = 12; // Chân IN2 điều khiển chiều quay động cơ khay
const int motorPin3 = 10; // Chân IN3 điều khiển chiều quay động cơ khay
const int motorPin4 = 8;  // Chân IN4 điều khiển chiều quay động cơ khay
const int funnelPin1 = 6; // Chân IN5 điều khiển chiều quay động cơ phễu
const int funnelPin2 = 4; // // Chân IN4 điều khiển chiều quay động cơ phễu

void setupDCMotor();
void loopDCMotor();
void openFoodTray();
void closeFoodTray();
void openFunnel();
void closeFunnel();
void turnOnLed();