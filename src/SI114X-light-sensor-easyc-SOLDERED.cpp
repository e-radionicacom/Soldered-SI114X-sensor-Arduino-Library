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
#include "Wire.h"

/**
 * @brief                   Initialization function with custom i2c address
 *
 */

bool SI114X::begin()
{
    this->_i2c_addr = SI1142;
    return this->Adafruit_SI1145::begin(_i2c_addr,&Wire);
} 

bool SI114X::begin(uint8_t _i2c_addr)
{
    this->_i2c_addr = _i2c_addr;
    return this->Adafruit_SI1145::begin(_i2c_addr,&Wire);
}

bool SI114X::changeAddress(uint8_t _new_i2c_addr)
{
    Wire.beginTransmission(this->_i2c_addr);
    Wire.write(SI1145_PARAM_I2CADDR);
    Wire.write(_new_i2c_addr);
    Wire.write(SI1145_REG_COMMAND);
    Wire.write(SI1145_BUSADDR);
    Wire.endTransmission(true);
    this->_i2c_addr = _new_i2c_addr;
    return this->Adafruit_SI1145::begin(this->_i2c_addr,&Wire);
}
