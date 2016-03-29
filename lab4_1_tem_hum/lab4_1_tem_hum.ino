#include <LDHT.h>
#include <Grove_LED_Bar.h>

Grove_LED_Bar bar(9, 8, 0);  // Clock pin, Data pin, Orientation

#define DHTPIN 2          // what pin we're connected to
#define DHTTYPE DHT22     // using DHT22 sensor
LDHT dht(DHTPIN,DHTTYPE);


float humidity;
float temperature;
int i;

void setup() {
  bar.begin();
  dht.begin();
  Serial.begin(115200);
}


void loop() {    
      Serial.println("===================================");
      if(dht.read())
      {
        temperature = dht.readTemperature();

        Serial.print("Temperature Celcius = ");
        Serial.print(temperature);
        Serial.println("C");
    
        Serial.println("===================================");
        i=map(temperature,25,30,1,10);
        Serial.println(i);
        bar.setLevel(i);
      }
      delay(1000);
}




