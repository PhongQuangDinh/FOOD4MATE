#include "distanceSensor.h"

int trig_pin = 2;
int echo_pin = 3;
long getDistance()
{
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
  
  long duration = pulseIn(echo_pin, HIGH);
  long distanceCm = duration * 0.034 / 2;
  return distanceCm;
}
void setup_distanceSensor()
{
  Serial.begin(9600);
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
}

void loopDistanceSensor()
{
  int distanceCm = getDistance();
   Serial.println(distanceCm);
}