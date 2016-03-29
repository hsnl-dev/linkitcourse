/*
  Lab3 Example 1
  Show the analog value from light sensor.
  
  The circuit:
  * light sensor attached to A0 (base shield A0 socket)

  Created 2/4/2016
*/
const int pinLight = A0;         // connect to analog pin 0

void setup()
{
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

void loop()
{
  // read the input value from light sensor
  int lightValue = analogRead(pinLight);
  // show the analog value of light sensor on the serial monitor
  Serial.println("the analog read data is ");
  Serial.println(lightValue);
  
  delay(100);
}


