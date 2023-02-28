/**
 **************************************************
 *
 * @file        si1147_light_UV_interruptOnComplete.ino
 * @brief       Example for sending an interrupt when a measurement cycle is complete with the SI1147 sensor.
 *              For more info see solde.red/333074
 *
 *
 * @authors     Robert Soric @ soldered.com
 ***************************************************/

// To run this example, connect the SI114X breakout board to your Dasduino board via easyC
// Also connect the INT pin to a digital input which supports interrupts

// In this example, the sensor will send an interrupt via the INT pin periodically when the internal measurement process
// is complete and a new value is ready to be read

#define INT_PIN 32 // Change interrupt pin here

#include "SI114X-light-sensor-easyc-SOLDERED.h"

SI1147 lightSensor;              // Create SI1147 sensor object
volatile bool interrupt = false; // Variable which stores if an interrupt occured

void setup()
{
    Serial.begin(115200);           // Begin serial communication with PC using 115200 baud rate
    pinMode(INT_PIN, INPUT_PULLUP); // Set the pin mode for the interrupt pin

    // Attatch the interrupt pin to the interrupt service routine
    // INT pin gets pulled LOW when there is an interrupt, so detect falling edge
    attachInterrupt(digitalPinToInterrupt(INT_PIN), ISR, FALLING);

    if (!lightSensor.begin(MEASUREMENT_MODE_AUTO)) // Initialize sensor in auto mode
    {
        Serial.println("Didn't find Si1147");
        while (1)
        {
            // Loop forever if sensor is not found
            delay(100);
        }
    }

    // Interrupt should happen approx. every 2s
    lightSensor.setAutoMeasurementRate(AUTO_2000_MS);

    // Enable interrupts
    lightSensor.enableALSInterrupts();
}

// Interrupt service routine which runs on any interrupt recieved
void ISR()
{
    interrupt = true;
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
