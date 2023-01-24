/**
 **************************************************
 *
 * @file        si1142.ino
 * @brief       Example for reading light intensity with SI1142 sensor variant. 
 *              For more info see solde.red/333074
 *
 *
 * @authors     Goran Juric & Robert Soric @ soldered.com
 ***************************************************/

// To run this example, connect the SI114X breakout board with your Dasduino board via easyC

#include "SI114X-light-sensor-easyc-SOLDERED.h"

SI1142 lightSensor; // Create SI1142 sensor object

void setup()
{
    Serial.begin(115200);           // Begin serial communication with PC using 115200 baud rate
    
    if (!lightSensor.begin()) // Initialize sensor and specify which sensor is connected
    {
        Serial.println("Didn't find Si1142");
        while (1)
        {
            // Loop forever if sensor is not found
            delay(100);
        } 
    }

    // Set sensor to do automatic measurement
    //lightSensor.setMeasurementMode(MEASUREMENT_MODE_AUTO);
}

void loop()
{
    uint16_t visLight;
    visLight = lightSensor.readVisible(); // get visible light intensity in lux
    Serial.print("Light intensity: ");
    Serial.print(visLight); // print visible light intensity
    Serial.println(" lux.");

    uint16_t IRLight;
    IRLight = lightSensor.readIR(); // get infrared light intensity in lux
    Serial.print("IR light intensity: ");
    Serial.print(IRLight); // Print light intensity in lux units
    Serial.println(" lux.");

    delay(3000); // wait a bit so the output isn't too fast
}
