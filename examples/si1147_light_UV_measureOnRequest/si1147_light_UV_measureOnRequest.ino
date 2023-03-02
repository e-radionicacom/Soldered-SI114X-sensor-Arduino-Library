/**
 **************************************************
 *
 * @file        si1147_light_UV_measureOnRequest.ino
 * @brief       Example for reading light intensity with SI1147 sensor in forced conversion mode.
 *              For more info see solde.red/333076
 *
 *
 * @authors     Robert Soric @ soldered.com
 ***************************************************/

// To run this example, connect the SI114X breakout board to your Dasduino board via easyC

// In this example, the device is set into 'Forced Conversion' measurement mode, meaning it will complete a measurement
// and then go back to standby, useful for saving power

#include "SI114X-light-sensor-easyc-SOLDERED.h"

SI1147 lightSensor; // Create SI1142 sensor object

void setup()
{
    Serial.begin(115200); // Begin serial communication with PC using 115200 baud rate

    if (!lightSensor.begin(MEASUREMENT_MODE_CONVERSION)) // Initialize sensor in forced conversion mode
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
    // Periodically print sensor readings
    
    uint16_t visLight;                    // Variable to store visible light intensity
    visLight = lightSensor.readVisible(); // Get visible light intensity in lux
    Serial.print("Light intensity: ");
    Serial.print(visLight); // Print visible light intensity
    Serial.println(" lux.");

    uint16_t IRLight;               // Variable to store IR (infrared) light intensity
    IRLight = lightSensor.readIR(); // Get IR light intensity in lux
    Serial.print("IR light intensity: ");
    Serial.print(IRLight); // Print lIR ight intensity
    Serial.println(" lux.");

    float UVindex;               // Variable to store UV (ultraviolet) index
    UVindex = lightSensor.readUV(); // Get UV light intensity in lux
    Serial.print("UV light index: ");
    Serial.println(UVindex); // Print UV ight intensity

    delay(2500); // wait a bit so the output isn't too fast
}
