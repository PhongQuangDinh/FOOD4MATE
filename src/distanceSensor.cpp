#include "distanceSensor.h"

int trig_pin = 19;
int echo_pin = 21;
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
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
}