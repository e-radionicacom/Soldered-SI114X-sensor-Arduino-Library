/**
 **************************************************
 *
 * @file        si1142.ino
 * @brief       Example for reading light intensity with SI1142 sensor variant. For more info see solde.red/333074
 *
 *
 * @authors     Goran Juric & Robert Soric @ soldered.com
 ***************************************************/

// Connecting diagram
// Plug in easyC cable into the SI114X Breakout board

#include "SI114X-light-sensor-easyc-SOLDERED.h"

SI114X lightSensor; // initialize sensor

void setup()
{
    Serial.begin(115200);           // Begin serial communication with PC using 115200 baud rate
    
    if (!lightSensor.begin(SI1142)) // Initialize sensor and specify which sensor is connected
    {
        Serial.println("Didn't find Si1142");
        while (1)
            ; // Loop forever if sensor is not found
    }
}

void loop()
{
    float visLight;
    visLight = lightSensor.readVisible(); // get visible light intensity in lux
    Serial.print("Light intensity: ");
    Serial.print(visLight); // print visible light intensity
    Serial.println(" lux.");

    float IRLight;
    IRLight = lightSensor.readIR(); // get infrared light intensity in lux
    Serial.print("IR light intensity: ");
    Serial.print(IRLight); // Print light intensity in lux units
    Serial.println(" lux.");

    delay(1000); // wait a bit so the output isn't too fast
}
