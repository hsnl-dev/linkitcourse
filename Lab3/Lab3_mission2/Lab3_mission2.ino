/*
  Lab3 mission 2
  Use Servo to show the light intensity level.

  The circuit:
  * light sensor attached to A0 (socket A0 on base shield)
  * servo attached to D3 (socket D3 on base shield)

  Created 2/4/2016
*/
#include <Servo.h>
Servo myservo; // create servo object

const int lightPin = A0; // connect to analog pin 0
const int servoPin = 3; // connect to digital pin 8
const int LIGHT_MAX = 500; // max value of light sensor is 420

void setup()
{
  myservo.attach(3); // attache to digital pin 3
  myservo.write(90);
  Serial.begin(9600);
}

void loop()
{
  // read the input value from light sensor
  int lightValue = analogRead(lightPin);

  // calculate the angle
  int angle = map(lightValue, 0, LIGHT_MAX, 0, 179);

  // show the light level on led bar
  myservo.write(angle);
  Serial.println(angle);
  delay(10);
}
