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

SI114X sensor; // initialize sensor

void setup()
{
  Serial.begin(115200);
  if (! sensor.begin(SI1142)) {
    Serial.println("Didn't find Si1142");
    while (1);
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
}
