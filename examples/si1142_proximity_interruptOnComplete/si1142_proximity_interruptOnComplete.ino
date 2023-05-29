/**
 **************************************************
 *
 * @file        si1142_proximity_interruptOnComplete.ino
 * @brief       Example for sending an interrupt on a complete proximity measurement
 *
 *              To successfully run the sketch:
 *              -Connect the breakout to your Dasduino board according to the diagram below
 *              -Also connect the interrupt pin and IR LED!
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

// In this example the sensor is periodically measuring values (auto mode) and storing the measurements
// We are reading the measurements in the main loop

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
 * IMPORTANT: an IR LED must be connected!
 * SI1142                       IR LED
 * VLED------------------------>CATHODE (-)
 * VCC------------------------->ANODE (+)
 * INT------------------------->INT_PIN (set by user)
 * 
 */
#define INT_PIN 3 // Change interrupt pin here

SI1142 lightSensor;              // Create SI1142 sensor object
volatile bool interrupt = false; // Variable which stores if an interrupt occured

// Interrupt service routine which runs on any interrupt recieved
void isr()
{
    interrupt = true;
}

void setup()
{
    Serial.begin(115200); // Begin serial communication with PC using 115200 baud rate
    pinMode(INT_PIN, INPUT_PULLUP); // Set the pin mode for the interrupt pin

    if (!lightSensor.begin(MEASUREMENT_MODE_AUTO)) // Initialize sensor
    {
        Serial.println("Didn't find SI1142");
        while (1)
        {
            // Loop forever if sensor is not found
            delay(100);
        }
    }

    // Attatch the interrupt pin to the interrupt service routine
    // INT pin gets pulled LOW when there is an interrupt, so detect falling edge
    attachInterrupt(digitalPinToInterrupt(INT_PIN), isr, FALLING);

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
        prox = lightSensor.readProx(1); // Get IR light intensity in lux from LED1
        Serial.print("Proximity: ");
        Serial.println(prox); // Print light intensity in lux units
        Serial.println(""); // Newline

        // Clear the INT pin on the sensor
        // This must be done to reset the INT pin state
        lightSensor.clearPSInterrupts();
        interrupt = false; // Clear the local interrupt variable
    }
}