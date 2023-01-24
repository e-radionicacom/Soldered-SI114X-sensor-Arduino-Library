/**
 **************************************************
 *
 * @file        si1147.ino
 * @brief       Example for reading light intensity with SI1147 sensor variant. For more info see solde.red/333076
 *
 *
 * @authors     Goran Juric & Robert Soric @ soldered.com
 ***************************************************/

// Connecting diagram
// Plug in easyC cable into the SI114X Breakout board

#include "SI114X-light-sensor-easyc-SOLDERED.h"

SI1147 lightSensor; // initialize sensor

void setup()
{
    Serial.begin(115200); // Begin serial communication with PC using 115200 baud rate

    if (!lightSensor.begin()) // Initialize sensor and specify which sensor is connected
    {
        Serial.println("Didn't find Si1147");
        while (1)
        {
            // Loop forever if sensor is not found
            delay(100);
        } 
    }
}

void loop()
{
    float visLight;
    visLight = lightSensor.readVisible(); // Get visible light intensity in lux
    Serial.print("Light intensity: ");
    Serial.print(visLight); // Print visible light intensity
    Serial.println(" lux.");

    float IRLight;
    IRLight = lightSensor.readIR(); // Get infrared light intensity in lux
    Serial.print("IR light intensity: ");
    Serial.print(IRLight); // Print light intensity in lux units
    Serial.println(" lux.");

    int uvLight = lightSensor.readUV(); // The SI1147 also supports reading UV light
                                        // If your sensor version does not support it, the value will be 0
    Serial.print("UV index: ");
    Serial.println(uvLight / 100.0); // UV needs to be divided by 100 because registers in sensor can
                                     // store only integers so using this method we can increase precision

    delay(3000); // Wait a bit so the output isn't too fast
}
