/***************************************************
  This is a library for the Si1145 UV/IR/Visible Light Sensor

  Designed specifically to work with the Si1145 sensor in the
  adafruit shop
  ----> https://www.adafruit.com/products/1777

  These sensors use I2C to communicate, 2 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include "SI1142.h"

/**
 * @brief Destructor
 *
 */
Adafruit_SI1142::~Adafruit_SI1142()
{
    if (i2c_dev)
        delete i2c_dev;
}
/**
 * @brief Construct a new Adafruit_SI1145::Adafruit_SI1145 object
 *
 */
Adafruit_SI1142::Adafruit_SI1142()
{
}
/**
 * @brief Initize the driver, specifying the `TwoWire` bus to use
 *
 * @param pBus Pointer to the `TwoWire` I2C bus to use
 * @return boolean true: success false: failure to initialize the sensor
 */
boolean Adafruit_SI1142::begin(TwoWire *pBus)
{
    return begin(SI1142_ADDR, pBus);
}

/**
 * @brief Initize the driver, supplying both a `TwoWire` bus and I2C address
 *
 * @param addr The I2C address of the Sensor
 * @param pBus Pointer to the `TwoWire` instance to use
 * @return boolean true: success false: failure to initize the sensor
 */
boolean Adafruit_SI1142::begin(uint8_t addr, TwoWire *pBus)
{
    // Delay the minimum required amount of time to make sure the sensor is in standby
    delay(25);

    if (i2c_dev)
        delete i2c_dev;
    i2c_dev = new Adafruit_I2CDevice(addr, pBus);
    if (!i2c_dev->begin())
    {
        return false;
    }

    uint8_t id = read8(SI1142_REG_PARTID);
    if (id != 0x42)
        return false; // look for SI1142

    reset();

    // Initialize LED current
    // write8(SI1142_REG_PSLED21, SI1142_PARAM_MAX_LED12_CURRENT);
    // write8(SI1142_REG_PSLED3, SI1142_PARAM_MAX_LED3_CURRENT);

    

    // Configure visible light measurement
    write8(SI1142_REG_ALSRATE, 0x08); // Set ALS to measure every time the device wakes up
    writeParam(SI1142_PARAM_ALSVISADCOUNTER, 0);
    writeParam(SI1142_PARAM_ALSIRADCGAIN, 0x07);
    writeParam(SI1142_PARAM_ALSVISADCMISC, 0x00);


    writeParam(SI1142_PARAM_ALSIRADCMUX, SI1142_PARAM_ADCMUX_SMALLIR);
    // fastest clocks, clock div 1
    writeParam(SI1142_PARAM_ALSIRADCGAIN, 0);
    // take 511 clocks to measure
    writeParam(SI1142_PARAM_ALSIRADCOUNTER, SI1142_PARAM_ADCCOUNTER_511CLK);
    // in high range mode
    writeParam(SI1142_PARAM_ALSIRADCMISC, SI1142_PARAM_ALSIRADCMISC_RANGE);

    // Set adc offset to 256
    writeParam(SI1142_PARAM_ADCOFFSET, 0X80);

    // Enable als_ir, als_vis
    writeParam(SI1142_PARAM_CHLIST, SI1142_PARAM_CHLIST_ENALSIR | SI1142_PARAM_CHLIST_ENALSVIS);

    // Measurement rate for auto
    write8(SI1142_REG_MEASRATE, 0xFF);
    write8(SI1142_REG_COMMAND, SI1142_PSALS_AUTO);

    return true;
}

/**
 * @brief     Set the measurement mode
 *
 * @param     measMode MEASUREMENT_MODE_AUTO (1) means the sensor will automatically make measurements and store the
 * results, MEASUREMENT_MODE_CONVERSION (2) means that the sensor will be in standby and perform measurements only when
 * asked, useful to save power
 *
 */
void Adafruit_SI1142::setMeasurementMode(int measMode)
{
    if (measMode == MEASUREMENT_MODE_AUTO)
    {
    }
    else if (measMode == MEASUREMENT_MODE_CONVERSION)
    {
    }
}


/**
 * @brief Reset the sensor's registers to an initial state
 *
 */
void Adafruit_SI1142::reset()
{
    write8(SI1142_REG_MEASRATE, 0);
    write8(SI1142_REG_IRQEN, 0);
    write8(SI1142_REG_IRQMODE1, 0);
    write8(SI1142_REG_IRQMODE2, 0);
    write8(SI1142_REG_INTCFG, 0);
    write8(SI1142_REG_IRQSTAT, 0xFF);

    write8(SI1142_REG_COMMAND, SI1142_RESET);
    delay(10);

    // Write HW key
    write8(SI1142_REG_HWKEY, 0x17);

    delay(10);
}

/**
 * @brief Get the Visible & IR light levels
 *
 * @return uint16_t The Visible & IR light levels
 */
uint16_t Adafruit_SI1142::readVisible(void)
{
    return read16(0x22) - 255;
}

// returns IR light levels
/**
 * @brief Get the Infrared light level
 *
 * @return uint16_t The Infrared light level
 */
uint16_t Adafruit_SI1142::readIR(void)
{
    return read16(0x24) - 255;
}

// returns "Proximity" - assumes an IR LED is attached to LED
/**
 * @brief Gets the Proximity measurement - **Requires an attached IR LED**
 *
 * @return uint16_t The proximity measurement
 */
uint16_t Adafruit_SI1142::readProx(void)
{
    return read16(0x26) - 255;
}

/*********************************************************************/

uint8_t Adafruit_SI1142::writeParam(uint8_t p, uint8_t v)
{
    // Serial.print("Param 0x"); Serial.print(p, HEX);
    // Serial.print(" = 0x"); Serial.println(v, HEX);

    write8(SI1142_REG_PARAMWR, v);
    write8(SI1142_REG_COMMAND, p | SI1142_PARAM_SET);
    return read8(SI1142_REG_PARAMRD);
}

uint8_t Adafruit_SI1142::readParam(uint8_t p)
{
    write8(SI1142_REG_COMMAND, p | SI1142_PARAM_QUERY);
    return read8(SI1142_REG_PARAMRD);
}

/*********************************************************************/

uint8_t Adafruit_SI1142::read8(uint8_t reg)
{
    uint8_t buffer[1] = {reg};
    i2c_dev->write_then_read(buffer, 1, buffer, 1);
    return buffer[0];
}

uint16_t Adafruit_SI1142::read16(uint8_t a)
{
    uint8_t buffer[2] = {a, 0};
    i2c_dev->write_then_read(buffer, 1, buffer, 2);
    return ((uint16_t)buffer[0]) | ((uint16_t)buffer[1] << 8);
}

void Adafruit_SI1142::write8(uint8_t reg, uint8_t val)
{
    uint8_t buffer[2] = {reg, val};
    i2c_dev->write(buffer, 2);
}