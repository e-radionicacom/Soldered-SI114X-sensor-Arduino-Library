/**
 **************************************************
 *
 * @file        si1147_proximity_interruptOnComplete.ino
 * @brief       Example for sending an interrupt when a measurement cycle is complete with the SI1142 sensor.
 *              For more info see solde.red/333076
 *
 *
 * @authors     Robert Soric @ soldered.com
 ***************************************************/

// To run this example, connect the SI114X breakout board to your Dasduino board via easyC
// Also connect an IR LED (anode to 3V3, cathode to LED1 pin)
// IR LED and the breakout board need to be placed close to each other so the sensor can detect IR light reflection
// which indicates proximity

// NOTE: For this example to work, you must remove the short on JP5/JP6 on your SI114X breakout board

#define INT_PIN 32 // Change interrupt pin here

#include "SI114X-light-sensor-easyc-SOLDERED.h"

SI1147 lightSensor;              // Create SI1142 sensor object
volatile bool interrupt = false; // Variable which stores if an interrupt occured

// Interrupt service routine which runs on any interrupt recieved
void ISR()
{
    interrupt = true;
}

void setup()
{
    Serial.begin(115200);           // Begin serial communication with PC using 115200 baud rate
    pinMode(INT_PIN, INPUT_PULLUP); // Set the pin mode for the interrupt pin

    if (!lightSensor.begin(MEASUREMENT_MODE_AUTO)) // Initialize sensor
    {
        Serial.println("Didn't find SI1147");
        while (1)
        {
            // Loop forever if sensor is not found
            delay(100);
            Serial.println("Didn't find SI1147");
        }
    }

    // Attatch the interrupt pin to the interrupt service routine
    // INT pin gets pulled LOW when there is an interrupt, so detect falling edge
    attachInterrupt(digitalPinToInterrupt(INT_PIN), ISR, FALLING);

    // Set the auto measurement rate
    // Interrupt should thus happen approx every 2s
    lightSensor.setAutoMeasurementRate(AUTO_2000_MS);

    // Enable proximity measurement with UV LED connected to pin LED 1 and measurement mode AUTO
    // Make sure you set the measurement rate first
    lightSensor.enableProximityLED(1);
    // You may also use the LED2 pin or both of them

    // Enable PS interrupts
    // This will enable interrupts for the previously initialized LED pins
    lightSensor.enablePSInterrupts();
}

void loop()
{
    // Only make a reading if an interrupt was read
    if (interrupt == true)
    {
        Serial.println("Interrupt received!");

        uint16_t prox;                  // Variable to store IR light intensity
        prox = lightSensor.readProx(2); // Get IR light intensity in lux from LED1
        Serial.print("Proximity: ");
        Serial.println(prox); // Print light intensity in lux units
        Serial.println("");   // Newline

        // Clear the INT pin on the sensor
        // This must be done to reset the INT pin state
        lightSensor.clearPSInterrupts();
        interrupt = false; // Clear the local interrupt variable
    }
}