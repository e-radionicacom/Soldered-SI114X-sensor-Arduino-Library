/**
 **************************************************
 *
 * @file        SI114X-light-sensor-SOLDERED.h
 * @brief       Header file for sensor specific code.
 *
 *
 * @copyright GNU General Public License v3.0
 * @authors     Goran Juric @ Soldered.com
 *              Robert Sorić @ Soldered.com
 ***************************************************/

#ifndef _Light_Sensor_
#define _Light_Sensor_

#include "Arduino.h"
#include "libs/Adafruit_SI1145_Library_BusIO/Adafruit_SI1145.h"

typedef enum {
  SI1142,
  SI1147
} sensor_variant_t;

class SI114X : public Adafruit_SI1145
{
  public:
    SI114X(sensor_variant_t sensor);
    bool begin();
  
  private:
    sensor_variant_t sensor;
};

#endif