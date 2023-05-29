/**
 **************************************************
 *
 * @file        si1147_proximity_autoMeasure.ino
 * @brief       Example for reading proximity on SI1147 sensor in auto mode using an external IR LED.
 *
 *              To successfully run the sketch:
 *              -Connect the breakout to your Dasduino board according to the diagram below
 *              -Also connect the IR LED
 *              -Open the serial monitor at 115200 baud!
 * 
 *              IMPORTANT: For this example to work, you must remove the short on JP5/JP6 on your SI114X breakout board
 *
 *              SI1147 Digital light & proximity sensor: solde.red/333076
 *              IR LED: IR www.solde.red/101922
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
 * SI1147                       Dasduino Core / Connect / ConnectPlus
 * VCC------------------------->VCC
 * GND------------------------->GND
 * SCL------------------------->A5/IO5/IO22
 * SDA------------------------->A4/IO4/IO21
 * 
 * Or, simply use an easyC cable!
 * 
 * IMPORTANT: an IR LED must be connected!
 * SI1142                       IR LED
 * VLED------------------------>CATHODE (-)
 * VCC------------------------->ANODE (+)
 * 
 */

SI1147 lightSensor; // Create SI1147 sensor object

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

    delay(200); // Wait a bit so the output isn't too fast
}
