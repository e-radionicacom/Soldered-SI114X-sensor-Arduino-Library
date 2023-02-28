/**
 **************************************************
 *
 * @file        si1147_light_UV_autoMeasure.ino
 *
 * @brief       Example for reading light intensity and UV with SI1147 sensor in auto mode.
 *              For more info see solde.red/333076
 *
 *
 * @authors     Robert Soric @ soldered.com
 ***************************************************/

// To run this example, connect the SI114X breakout board to your Dasduino board via easyC

// In this example the sensor is periodically measuring values (auto mode) and storing the measurements
// We are reading the measurements in the main loop

#include "SI114X-light-sensor-easyc-SOLDERED.h"

SI1147 lightSensor; // Create SI1142 sensor object

void setup()
{
    Serial.begin(115200); // Begin serial communication with PC using 115200 baud rate

    if (!lightSensor.begin(MEASUREMENT_MODE_AUTO)) // Initialize sensor in auto mode
    {
        Serial.println("Didn't find SI1147");
        while (1)
        {
            // Loop forever if sensor is not found
            delay(100);
        }
    }

    // Set how often the sensor will wake up and make a new reading
    // Reading in between the times a new measurement is complete will result in reading the same value
    // You may use AUTO_10_MS = 0x84, AUTO_20_MS = 0x94, AUTO_100_MS = 0xB9, AUTO_500_MS = 0xDF, AUTO_2000_MS = 0xFF
    // If you want to use a different specific value, see the SI114X Designer's guide and the datasheet
    lightSensor.setAutoMeasurementRate(AUTO_20_MS);
}

void loop()
{
    // Print reading
    uint16_t visLight;                    // Variable to store visible light intensity
    visLight = lightSensor.readVisible(); // Get visible light intensity in lux
    Serial.print("Light intensity: ");
    Serial.print(visLight); // Print visible light intensity
    Serial.println(" lux.");

    uint16_t IRLight;               // Variable to store IR (infrared) light intensity
    IRLight = lightSensor.readIR(); // Get IR light intensity in lux
    Serial.print("IR light intensity: ");
    Serial.print(IRLight); // Print IR ight intensity
    Serial.println(" lux.");

    float UVindex;               // Variable to store UV (ultraviolet) index
    UVindex = lightSensor.readUV(); // Get UV light intensity in lux
    Serial.print("UV light index: ");
    Serial.println(UVindex); // Print UV ight intensity

    delay(2500); // wait a bit so the output isn't too fast
}
