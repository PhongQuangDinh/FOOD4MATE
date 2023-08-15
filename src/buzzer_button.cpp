#include "buzzer_button.h"
#include "distanceSensor.h"

const int buzzerPin = 2;
const int buttonPin = 4;
const int high_frequency = 2300; 
const int low_frequency = 1800; //frequencies of high/low sounds

void playSound() //function used to play the sound via buzzer
{
  tone(buzzerPin, high_frequency, 500);
  tone(buzzerPin, low_frequency, 500);
}

void buzzer_button_setup()
{
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void buzzer_button_loop() 
{
  // put your main code here, to run repeatedly:
  int btnState = digitalRead(buttonPin);
  if (btnState == HIGH) //user press button on app maybe?
  {
    playSound();
    int delayed = 15; //seconds (waiting time since the button is pressed)
    while(delayed) //wait and check if the pet comes to the machine
    {
      int distance = getDistance(); //update distance
      if (distance <= 50)
      {
        Serial.println("Food delivered");
        break;
      }
      delayed--;
      delay(1000);//countdown
    }
  }
  delay(500); //short delay to make sure button functions properly
}