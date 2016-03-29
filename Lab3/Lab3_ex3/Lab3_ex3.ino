/*
  Lab3 Example 3
  Let servo rotate back and forth.

  The circuit:
  * servo attached to digital pin 3 (base shield D3 socket)

  created 2/20/2016
*/

#include <Servo.h>

Servo myservo;  // create servo object

void setup()
{
  myservo.attach(3);  // attached to digital pin 3
  myservo.write(90);
}

void loop()
{

    for(int i=0; i<180; i++)
    {
        myservo.write(i);
        delay(15);
    }
    for(int i=179; i>=0; i--)
    {
        myservo.write(i);
        delay(15);
    }
}
