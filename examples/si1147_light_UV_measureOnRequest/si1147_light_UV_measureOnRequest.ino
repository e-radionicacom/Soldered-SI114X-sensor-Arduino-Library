/**
 **************************************************
 *
 * @file        si1147_light_UV_measureOnRequest.ino
 * @brief       Example for making measurements on SI1147 in forced request mode.
 *
 *              To successfully run the sketch:
 *              -Connect the breakout to your Dasduino board according to the diagram below
 *              -Open the serial monitor at 115200 baud!
 *
 *              SI1147 Digital light & proximity sensor: solde.red/333076
 *              Dasduino Core: www.solde.red/333037
 *              Dasduino Connect: www.solde.red/333034
 *              Dasduino ConnectPlus: www.solde.red/333033
 *
 *
 * @authors     Robert Soric @ soldered.com
 ***************************************************/

#include "SI114X-light-sensor-easyc-SOLDERED.h"

/**
 * Connecting diagram:
 *
 * SI1147                      Dasduino Core / Connect / ConnectPlus
 * VCC------------------------->VCC
 * GND------------------------->GND
 * SCL------------------------->A5/IO5/IO22
 * SDA------------------------->A4/IO4/IO21
 *
 * Or, simply use an easyC cable!
 *
 */

// In this example, the device is set into 'Forced Conversion' measurement mode, meaning it will complete a measurement
// and then go back to standby, useful for saving power

SI1147 lightSensor; // Create SI1147 sensor object

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
    IRLight = lightSensor.readIR(); // Get IR light intensity in ADC counts * m^2/W.
    Serial.print("IR light intensity: ");
    Serial.print(IRLight); // Print IR ight intensity
    Serial.println(" ADC counts * m^2/W.");

    float UVindex;                  // Variable to store UV (ultraviolet) index
    UVindex = lightSensor.readUV(); // Get UV light intensity in lux
    Serial.print("UV light index: ");
    Serial.println(UVindex); // Print UV ight intensity

    delay(2500); // wait a bit so the output isn't too fast
}
