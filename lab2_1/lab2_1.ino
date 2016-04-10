#include <Grove_LED_Bar.h>

const int TouchPin=2;
const int ledPin=13;
const int ledBar=8;
Grove_LED_Bar bar(9, 8, 0);  // Clock pin, Data pin, Orientation

void setup() {
  bar.begin();
  pinMode(TouchPin, INPUT);
  pinMode(ledPin,OUTPUT);
} 
 
void loop() {
  int sensorValue = digitalRead(TouchPin);
  
  if(sensorValue==0){
    digitalWrite(ledPin,HIGH);
    bar.setLevel(10);
  }
  else{
    digitalWrite(ledPin,LOW);
    // Walk through the levels
    for (int i = 0; i <= 10; i++){
      if (digitalRead(TouchPin)==0){         // sensor detect
        break;
      }
      bar.setLevel(i);
      delay(100);
    } 
  }
}
