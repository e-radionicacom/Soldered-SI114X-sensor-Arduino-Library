/**
 **************************************************
 *
 * @file        ReadLightIntensity.ino
 * @brief       Example for reading light intensity
 *
 *
 *              Product used is www.solde.red/333
 * @authors     Goran Juric for soldered.com
 ***************************************************/

#include "SI1147-light-sensor-easyc-SOLDERED.h"

SI114X sensor;

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  if(sensor.Available())
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
    int uv = sensor.readUV(); //If your sensor does not support UV, return value will be 0
    Serial.print("UV index: ");
    Serial.println(uv);
  }
  else Serial.println("Communication error");
  delay(1000);
}
