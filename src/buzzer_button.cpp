#include "buzzer_button.h"

const int buzzerPin = 14;
const int high_frequency = 2300; 
const int low_frequency = 1800; //frequencies of high/low sounds

void playSound() //function used to play the sound via buzzer
{
  ledcWriteNote(0, NOTE_D, 7);
}
void stopSound()
{
  // noTone(buzzerPin);
  ledcDetachPin(buzzerPin);
}
void setup_buzzer() // call once
{
  pinMode(buzzerPin, OUTPUT);
  ledcAttachPin(buzzerPin, 0);
}