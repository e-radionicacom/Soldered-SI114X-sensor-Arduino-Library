/**
 **************************************************
 *
 * @file        si1142_proximity_measureOnRequest.ino
 * @brief       Example for reading proximity on SI1142 sensor in forced conversion mode using an external IR LED.
 *
 *              To successfully run the sketch:
 *              -Connect the breakout to your Dasduino board according to the diagram below
 *              -Also connect the IR LED!
 *              -Open the serial monitor at 115200 baud!
 * 
 *              IMPORTANT: For this example to work, you must remove the short on JP5/JP6 on your SI114X breakout board
 *
 *              SI1142 Digital light & proximity sensor: solde.red/333074
 *              IR LED: IR www.solde.red/101922
 *              Dasduino Core: www.solde.red/333037
 *              Dasduino Connect: www.solde.red/333034
 *              Dasduino ConnectPlus: www.solde.red/333033
 *
 *
 * @authors     Robert Soric @ soldered.com
 ***************************************************/

#include "SI114X-light-sensor-easyc-SOLDERED.h"

// In this example, the device is set into 'Forced Conversion' measurement mode, meaning it will complete a measurement
// and then go back to standby, useful for saving power

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
 * IMPORTANT: an IR LED must be connected!
 * SI1142                       IR LED
 * VLED------------------------>CATHODE (-)
 * VCC------------------------->ANODE (+)
 * 
 */

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
