/**
 **************************************************
 *
 * @file        si1142_proximity_autoMeasure.ino
 * @brief       Example for reading proximity on SI1142 sensor in auto mode using an external UV LED.
 *              For more info see solde.red/333074
 *
 *
 * @authors     Robert Soric @ soldered.com
 ***************************************************/

// To run this example, connect the SI114X breakout board to your Dasduino board via easyC
// Also connect an IR LED (anode to 3V3, cathode to LED1 pin)
// IR LED and the breakout board need to be placed close to each other so the sensor can detect IR light reflection
// which indicates proximity

// NOTE: For this example to work, you must remove the short on JP5/JP6 on your SI114X breakout board

#include "SI114X-light-sensor-easyc-SOLDERED.h"

SI1142 lightSensor; // Create SI1142 sensor object

void setup()
{
    Serial.begin(115200); // Begin serial communication with PC using 115200 baud rate

    if (!lightSensor.begin(MEASUREMENT_MODE_AUTO)) // Initialize sensor in auto mode
    {
        Serial.println("Didn't find SI1142");
        while (1)
        {
            // Loop forever if sensor is not found
            delay(100);
        }
    }

    // Set the measurement rate
    lightSensor.setAutoMeasurementRate(AUTO_10_MS);

    // Enable proximity measurement with UV LED connected to pin LED 1 and measurement mode AUTO
    // Make sure you set the measurement rate first
    lightSensor.enableProximityLED(1);
    // You may also use the LED2 pin or both
}

void loop()
{
    uint16_t prox;                  // Variable to store IR light intensity
    prox = lightSensor.readProx(1); // Get IR light intensity in lux from LED1
    Serial.print("Proximity reading: ");
    Serial.println(prox); // Print light intensity in lux units

    delay(2500); // Wait a bit so the output isn't too fast
}
