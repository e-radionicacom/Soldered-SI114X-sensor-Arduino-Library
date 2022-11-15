/**
 **************************************************
 *
 * @file        si1142.ino
 * @brief       Example for reading light intensity
 *
 *
 *              Product used is www.solde.red/333044
 * @authors     Goran Juric for soldered.com
 ***************************************************/

//Connecting diagram
//Plug in easyC cable

#include "SI114X-light-sensor-easyc-SOLDERED.h"

SI114X sensor(SI1142); // initialize sensor

void setup()
{
  Serial.begin(115200); //Begin serial communication with PC using 115200 baud rate
  if (! sensor.begin())
  { 
    Serial.println("Didn't find SI1142");
    while (1);
    {
      delay(100);
    }
  }
}

void loop()
{
  float light;
  Serial.print("Light intensity: ");
  Serial.print(sensor.readVisible());
  Serial.println(" lux.");

  delay(500);


}