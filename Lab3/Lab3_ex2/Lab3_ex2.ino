/*
  Lab3 Example 2
  Let led increasing brightness
  
  The circuit:
  * led attached to digital pin 9

  created 2/20/2016  Lab3 Example 3
*/
const int pinLed = 9; // attached to digital pin 9
void setup() {
}

void loop() {
  for (int i=128;i<=1023;i++){
    analogWrite(pinLed, i);
    delay(15);
  }
}



