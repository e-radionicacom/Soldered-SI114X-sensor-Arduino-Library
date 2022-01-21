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

#include "SI1147-light-sensor-easyc-SOLDERED.h"

SI114X sensor; //initialize sensor

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  if(sensor.Available()) //Checking if sensor is available
  {
    float light; 
    light = sensor.getLightIntensity(); // get light intensity
    Serial.print("Light intensity: ");
    Serial.print(light);
    Serial.println(" lux.");
    light = sensor.getLightIntensityIR(); // get light intensity
    Serial.print("IR light intensity: ");
    Serial.print(light);
    Serial.println(" lux.");
  }
  else Serial.println("Communication error");
  delay(1000);
}
