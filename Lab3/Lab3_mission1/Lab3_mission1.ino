/*
  Lab3 Mission 1
  Use LED bar to show sound volume.

  The circuit:
  * sound sensor attached to analog pin 0 (base shield A0 socket)
  * LED bar attached to digital pin 8 (base shield D8 socket)

  created 2/20/2016
*/
#include <Grove_LED_Bar.h>

Grove_LED_Bar bar(9, 8, 0); // Clock pin, Data pin, Orientation

const int soundPin = A0; // connect to analog pin A0
const int ledBar = 8; // connect to digital pin 8

void setup()
{
  bar.begin();
}

void loop()
{
  // read the input value from sound sensor
  int soundValue = analogRead(soundPin);
  // calculate the level of sound
  int soundLevel = map(soundValue, 0, 80, 0, 10);

  // show the sound level on led bar
  for (int i = 0; i <= soundLevel; i++){
    bar.setLevel(i);
  }

  delay(10);

}
