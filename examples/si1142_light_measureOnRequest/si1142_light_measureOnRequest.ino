/**
 **************************************************
 *
 * @file        si1142_light_measureOnRequest.ino
 * @brief       Example for reading light intensity with SI1142 sensor in forced conversion mode.
 *
 *              To successfully run the sketch:
 *              -Connect the breakout to your Dasduino board according to the diagram below
 *              -Open the serial monitor at 115200 baud!
 *
 *              SI1142 Digital light & proximity sensor: solde.red/333074
 *              Dasduino Core: www.solde.red/333037
 *              Dasduino Connect: www.solde.red/333034
 *              Dasduino ConnectPlus: www.solde.red/333033
 *
 *
 * @authors     Robert Soric @ soldered.com
 ***************************************************/

// In this example, the device is set into 'Forced Conversion' measurement mode, meaning it will complete a measurement
// and then go back to standby, useful for saving power

#include "SI114X-light-sensor-easyc-SOLDERED.h"

/**
 * Connecting diagram:
 *
 * SI1142                      Dasduino Core / Connect / ConnectPlus
 * VCC------------------------->VCC
 * GND------------------------->GND
 * SCL------------------------->A5/IO5/IO22
 * SDA------------------------->A4/IO4/IO21
 *
 * Or, simply use an easyC cable!
 *
 */

SI1142 lightSensor; // Create SI1142 sensor object

void setup()
{
    Serial.begin(115200); // Begin serial communication with PC using 115200 baud rate

    if (!lightSensor.begin(MEASUREMENT_MODE_CONVERSION)) // Initialize sensor in forced conversion mode
    {
        Serial.println("Didn't find Si1142");
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

    delay(2500); // wait a bit so the output isn't too fast
}
