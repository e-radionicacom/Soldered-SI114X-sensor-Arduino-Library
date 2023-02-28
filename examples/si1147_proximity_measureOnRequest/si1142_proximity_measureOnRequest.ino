/**
 **************************************************
 *
 * @file        si1142_proximity_measureOnRequest.ino
 * @brief       Example for reading proximity on SI1142 sensor in forced conversion mode using an external UV LED.
 *              For more info see solde.red/333074
 *
 *
 * @authors     Goran Juric & Robert Soric @ soldered.com
 ***************************************************/

// To run this example, connect the SI114X breakout board to your Dasduino board via easyC
// Also connect an IR LED (anode to 3V3, cathode to LED1 pin)
// IR LED and the breakout board need to be placed close to each other so the sensor can detect IR light reflection
// which indicates proximity

// In this example, the device is set into 'Forced Conversion' measurement mode, meaning it will complete a measurement
// and then go back to standby, useful for saving power

#include "SI114X-light-sensor-easyc-SOLDERED.h"

SI1142 lightSensor; // Create SI1142 sensor object

void setup()
{
    Serial.begin(115200); // Begin serial communication with PC using 115200 baud rate

    if (!lightSensor.begin(MEASUREMENT_MODE_CONVERSION)) // Initialize sensor
    {
        Serial.println("Didn't find SI1142");
        while (1)
        {
            // Loop forever if sensor is not found
            delay(100);
        }
    }

    // Enable proximity measurement with UV LED connected to pin LED 1 and measurement MODE CONVERSION
    lightSensor.enableProximityLED(1);

    // You may also use the LED2 pin or both of them
}

void loop()
{
    uint16_t prox;                  // Variable to store IR light intensity
    prox = lightSensor.readProx(1); // Get IR light intensity in lux from LED1
    Serial.print("Proximity: ");
    Serial.println(prox); // Print light intensity in lux units

    delay(2500); // Wait a bit so the output isn't too fast
}
