/**
 **************************************************
 *
 * @file        SI114X-light-sensor-SOLDERED.h
 * @brief       Header file for sensor specific code.
 *
 *
 * @copyright GNU General Public License v3.0
 * @authors     Goran Juric for soldered.com
 ***************************************************/

#ifndef __LIGHT_SENSOR_SI114X_BREAKOUT__
#define __LIGHT_SENSOR_SI114X_BREAKOUT__

#include "Arduino.h"
#include "libs/Adafruit_SI1145_Library_BusIO/Adafruit_SI1145.h"
#include "libs/Adafruit_SI1145_Library_BusIO/Soldered_SI1142.h"

class SI1147 : public Adafruit_SI1145
{
  public:
    SI1147() : Adafruit_SI1145()
    {
    }

  private:
    uint8_t _i2c_addr;
};

class SI1142 : public Soldered_SI1142
{
  public:
    SI1142() : Soldered_SI1142()
    {
    }

  private:
    uint8_t _i2c_addr;
};

#endif