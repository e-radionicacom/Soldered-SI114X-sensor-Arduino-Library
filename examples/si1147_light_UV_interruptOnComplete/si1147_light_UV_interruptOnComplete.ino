/**
 **************************************************
 *
 * @file        si1147_light_UV_interruptOnComplete.ino
 * @brief       Example for sending an interrupt when a measurement cycle is complete with the SI1147 sensor.
 *
 *              To successfully run the sketch:
 *              -Connect the breakout to your Dasduino board according to the diagram below
 *              -Also connect the interrupt pin
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
 * SI1147                       Dasduino
 * INT------------------------->INT_PIN (set by user)
 *
 */

#define INT_PIN 3 // Change interrupt pin here, it has to be a pin which supports interrupts

SI1147 lightSensor;              // Create SI1147 sensor object
volatile bool interrupt = false; // Variable which stores if an interrupt occured

// Interrupt service routine which runs on any interrupt recieved
void isr()
{
    interrupt = true;
}

void setup()
{
    Serial.begin(115200);           // Begin serial communication with PC using 115200 baud rate
    pinMode(INT_PIN, INPUT_PULLUP); // Set the pin mode for the interrupt pin

    if (!lightSensor.begin(MEASUREMENT_MODE_AUTO)) // Initialize sensor in auto mode
    {
        Serial.println("Didn't find Si1147");
        while (1)
        {
            // Loop forever if sensor is not found
            delay(100);
        }
    }

    // Attatch the interrupt pin to the interrupt service routine
    // INT pin gets pulled LOW when there is an interrupt, so detect falling edge
    attachInterrupt(digitalPinToInterrupt(INT_PIN), isr, FALLING);

    // Set the measurement rate
    // Thus, interrupt should happen approx. every 2s
    lightSensor.setAutoMeasurementRate(AUTO_2000_MS);

    // Enable interrupts for light sensor
    lightSensor.enableALSInterrupts();
}

void loop()
{
    // Only make a reading if an interrupt was read
    if (interrupt == true)
    {
        Serial.println("Interrupt received!");

        // Print completed reading
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

        float UVindex;                  // Variable to store UV (ultraviolet) index
        UVindex = lightSensor.readUV(); // Get UV light intensity in lux
        Serial.print("UV light index: ");
        Serial.println(UVindex); // Print UV ight intensity

        // Clear the INT pin on the sensor
        // This must be done to reset the INT pin state
        lightSensor.clearALSInterrupt();
        interrupt = false; // Clear the local interrupt variable
    }
}
