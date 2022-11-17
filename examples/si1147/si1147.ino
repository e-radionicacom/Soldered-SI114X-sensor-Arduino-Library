/**
 **************************************************

   @file        si1147.ino
   @brief       Example for reading light and UV intensity


                Product used is www.solde.red/333041
   @authors     Goran Juric for soldered.com
 ***************************************************/

#include "SI114X-light-sensor-easyc-SOLDERED.h"

SI114X sensor;

void setup()
{
  Serial.begin(115200); //Begin serial communication with PC using 115200 baud rate
  if (! sensor.begin(SI1147)) //Initialize sensor and specify which sensor is connected
  {
    Serial.println("Didn't find Si1147");
    while (1);  //Loop forever if sensor is not found
  }
}

void loop()
{
  float light;
  light = sensor.readVisible(); // get light intensity
  Serial.print("Light intensity: ");
  Serial.print(light);
  Serial.println(" lux.");
  light = sensor.readIR(); // get light intensity
  Serial.print("IR light intensity: ");
  Serial.print(light);
  Serial.println(" lux.");
  int uv = sensor.readUV(); // If your sensor does not support UV, return value will be 0
  Serial.print("UV index: ");
  Serial.println(uv / 100.0); //UV needs to be divided by 100 because registers in sensor can
                              //store only integers so using this method we can increase precision
  delay(1000);
}
