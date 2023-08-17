#include "dcMotor.h"

const int motorPin1 = 15;
const int motorPin2 = 5;
const int motorPin3 = 22;
const int motorPin4 = 2;
const int funnelPin1 = 18;
const int funnelPin2 = 12;

bool isOpenTray = false;
bool isOpenFunnel = false;
int openFunnelTimer = 10;
int openTrayTimer = 12;
int FunnelCounter = openFunnelTimer;
int TrayCounter = openTrayTimer;

void setupDCMotor() 
{
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  pinMode(funnelPin1, OUTPUT);
  pinMode(funnelPin2, OUTPUT);

  // Tắt động cơ ban đầu
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  digitalWrite(funnelPin1, LOW);
  digitalWrite(funnelPin2, LOW); 
}
void openFoodTray()
{
  // Chiều quay động cơ để mở khay
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);

  delay(100);
}

void closeFoodTray()
{
  // Chiều quay động cơ để đóng khay
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);

  delay(100);
}

// Hàm mở phễu
void openFunnel()
{
  // Chiều quay động cơ để mở khay
  digitalWrite(funnelPin1, HIGH);
  digitalWrite(funnelPin2, LOW);

  delay(100);
}

// Hàm đóng phễu
void closeFunnel()
{
  // Chiều quay động cơ để đóng khay
  digitalWrite(funnelPin1, LOW);
  digitalWrite(funnelPin2, HIGH);

  delay(100);
}

//Hàm tắt động cơ phễu
void shutDownFunnel()
{
  digitalWrite(funnelPin1, LOW);
  digitalWrite(funnelPin2, LOW);
}

void shutDownFoodTray()
{
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}
void loopDCMotor()
{
  // Funnel looping
  if (isOpenFunnel == true && FunnelCounter > 0) // give signal but still not fully open
  {
    openFunnel();
    FunnelCounter -= 1;
  }
  else if (isOpenFunnel == false && FunnelCounter < openFunnelTimer)
  {
    closeFunnel();
    FunnelCounter += 1;
  }
  else
    shutDownFunnel();

  // Tray looping
  if (isOpenTray == true && TrayCounter > 0) // give signal but still not fully open
  {
    openFoodTray();
    TrayCounter -= 1;
  }
  else if (isOpenTray == false && TrayCounter < openFunnelTimer)
  {
    closeFoodTray();
    TrayCounter += 1;
  }
  else
    shutDownFoodTray();
}