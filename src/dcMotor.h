#include <Arduino.h>

extern bool isOpenTray;
extern bool isOpenFunnel;

void setupDCMotor();
void loopDCMotor();
void openFoodTray();
void closeFoodTray();
void openFunnel();
void closeFunnel();