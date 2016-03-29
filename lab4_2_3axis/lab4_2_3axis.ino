// test for seeed graphical programming project
// loovee @ 2015-8-4

#include <Wire.h>
#include <ADXL345.h>
#include <Grove_LED_Bar.h>

Grove_LED_Bar bar(9, 8, 0);  // Clock pin, Data pin, Orientation

ADXL345 adxl;
int i;

void setup()
{
    i=0;
    bar.begin();
    adxl.init();
    Serial.begin(115200);
    Serial.println("test for Seeed Graphical programming project");
    Serial.println("3-axis digital accelerator");
}

void loop()
{
    //Boring accelerometer stuff
    int x,y,z;
    adxl.readXYZ(&x, &y, &z); //read the accelerometer values and store them in variables  x,y,z
    // Output x,y,z values
    Serial.print("values of X , Y , Z: ");
    Serial.print(x);
    Serial.print(" , ");
    Serial.print(y);
    Serial.print(" , ");
    Serial.println(z);

    if(z>290)
      i++;
    bar.setLevel(i);
/*
    double xyz[3];
    double ax,ay,az;
    adxl.getAcceleration(xyz);
    ax = x*0.0039;
    ay = y*0.0039;
    az = z*0.0039;
    Serial.print("X=");
    Serial.print(ax);
    Serial.println(" g");
    Serial.print("Y=");
    Serial.print(ay);
    Serial.println(" g");
    Serial.print("Z=");
    Serial.print(az);
    Serial.println(" g");
    Serial.println("**********************");
    */
    delay(300);
}

