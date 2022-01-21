/**
 **************************************************
 *
 * @file        Sensitivity.ino
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
    sensor.setVISSensitivity(NORMAL_S); //  Set sensitivity for visible light
    sensor.setIRSensitivity(NORMAL_S);  //  Set sensitivity for IR light
    float light; 
    light = sensor.getLightIntensity(); // get light intensity
    Serial.print("Normal sensitivity light intensity: ");
    Serial.print(light);
    Serial.println(" lux.");
    light = sensor.getLightIntensityIR(); // get light intensity
    Serial.print("Normal sensitivity IR light intensity: ");
    Serial.print(light);
    Serial.println(" lux.");

    sensor.setVISSensitivity(HIGH_S); //  Set sensitivity for visible light
    sensor.setIRSensitivity(HIGH_S);  //  Set sensitivity for IR light
    light = sensor.getLightIntensity(); // get light intensity
    Serial.print("High sensitivity light intensity: ");
    Serial.print(light);
    Serial.println(" lux.");
    light = sensor.getLightIntensityIR(); // get light intensity
    Serial.print("High sensitivity IR light intensity: ");
    Serial.print(light);
    Serial.println(" lux.");
  }
  else Serial.println("Communication error");
  delay(4000);
}
