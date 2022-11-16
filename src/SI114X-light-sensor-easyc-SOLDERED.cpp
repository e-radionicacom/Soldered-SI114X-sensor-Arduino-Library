/**
 **************************************************
 *
 * @file        SI114X-light-sensor-SOLDERED.cpp
 * @brief       Example functions to overload in base class.
 *
 *
 * @copyright GNU General Public License v3.0
 * @authors     Goran Juric for soldered.com
 ***************************************************/


#include "SI114X-light-sensor-easyc-SOLDERED.h"

/**
 * @brief                   Default constructor
 *
 */
SI114X::SI114X(sensor_variant_t sensor) : Adafruit_SI1145()
{
    this->sensor = sensor;
}

bool SI114X::begin()
{
    switch (this->sensor)
    {
    case SI1142:
        return Adafruit_SI1145::begin(0x5A);
    case SI1147:
        return Adafruit_SI1145::begin(0X60);
    }
}

/**
 *
 * SI1142
 *
 *
 */
