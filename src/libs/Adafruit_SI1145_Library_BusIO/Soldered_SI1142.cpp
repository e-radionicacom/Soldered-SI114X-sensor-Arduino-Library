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

  Modified by Soldered for use on SI114X breakout board with some expanded functionality
 ****************************************************/

#include "Soldered_SI1142.h"

/**
 * @brief       Destructor
 *
 */
Soldered_SI1142::~Soldered_SI1142()
{
    if (i2c_dev)
        delete i2c_dev;
}
/**
 * @brief       Default constructor
 *
 */
Soldered_SI1142::Soldered_SI1142()
{
}

/**
 * @brief       Sensor initialization function
 *
 */
boolean Soldered_SI1142::begin(TwoWire *pBus)
{
    return begin(MEASUREMENT_MODE_AUTO, SI1142_ADDR, pBus);
}

/**
 * @brief       Sensor initialization function with specific parameters
 *
 * @param       MEASUREMENT_MODE mode, the desired measurement mode (auto or conversion request)
 *
 * @param       addr The I2C address of the Sensor
 *
 * @param       pBus Pointer to the `TwoWire` instance to use
 *
 * @return      True if successful, false if it fails
 */
boolean Soldered_SI1142::begin(MEASUREMENT_MODE mode, uint8_t addr, TwoWire *pBus)
{
    // Delay the minimum required amount of time to make sure the sensor is ready to be configured
    delay(25);

    if (i2c_dev)
        delete i2c_dev;
    i2c_dev = new Adafruit_I2CDevice(addr, pBus);
    if (!i2c_dev->begin())
    {
        return false;
    }

    // Check if the part ID corresponds to SI1142
    uint8_t id = read8(SI1142_REG_PARTID);
    if (id != 0x42)
        return false;

    reset();

    // Enable visible light and IR light channels and set automeasure rate
    writeParam(SI1142_PARAM_CHLIST, SI1142_PARAM_CHLIST_ENALSIR | SI1142_PARAM_CHLIST_ENALSVIS);
    setAutoMeasurementRate(AUTO_20_MS);

    // Make measurement every time the device wakes up
    write8(SI1142_REG_ALSRATE, SI1142_PARAM_ALS_RATE_MAX);

    // Configure ambient light sensor
    writeParam(SI1142_PARAM_ALSVISADCGAIN, SI1142_PARAM_ALS_ADCGAIN);
    writeParam(SI1142_PARAM_ALSVISADCOUNTER, SI1142_PARAM_ADCCOUNTER_511CLK);

    // Configure IR sensor
    writeParam(SI1142_PARAM_ALSIRADCMISC, SI1142_PARAM_ALSIRADCMISC_RANGE);
    writeParam(SI1142_PARAM_ALSIRADCGAIN, SI1142_PARAM_IR_ADCGAIN);
    writeParam(SI1142_PARAM_ALSIRADCOUNTER, SI1142_PARAM_ADCCOUNTER_511CLK);
    writeParam(SI1142_PARAM_ALSIRADCMUX, SI1142_PARAM_ADCMUX_SMALLIR);

    // Set expected ADC offset
    writeParam(SI1142_PARAM_ADCOFFSET, SI1142_ADC_OFFSET_256);

    // Set given measurement mode
    setALSMeasurementMode(mode);
    currentMeasurementMode = mode;

    return true;
}

/**
 * @brief       Sets the according measurement mode for ambient light sensor
 *
 * @param       measMode MEASUREMENT_MODE_AUTO (1) means the sensor will automatically make measurements and store the
 * results, MEASUREMENT_MODE_CONVERSION (2) means that the sensor will be in standby and perform measurements only when
 * asked, useful to save power
 *
 * @return      None
 *
 */
void Soldered_SI1142::setALSMeasurementMode(MEASUREMENT_MODE mode)
{
    if (mode == MEASUREMENT_MODE_AUTO)
    {
        write8(SI1142_REG_COMMAND, SI1142_ALS_AUTO);
    }

    else if (mode == MEASUREMENT_MODE_CONVERSION)
    {
        setAutoMeasurementRate(OFF);
        write8(SI1142_REG_COMMAND, SI1142_ALS_PAUSE);
    }
}

/**
 * @brief       Sets the according measurement mode for proximity sensor
 *
 * @param       measMode MEASUREMENT_MODE_AUTO (1) means the sensor will automatically make measurements and store the
 * results, MEASUREMENT_MODE_CONVERSION (2) means that the sensor will be in standby and perform measurements only when
 * asked, useful to save power
 *
 * @return      None
 *
 */
void Soldered_SI1142::setPSMeasurementMode(MEASUREMENT_MODE mode)
{
    if (mode == MEASUREMENT_MODE_AUTO)
    {
        write8(SI1142_REG_COMMAND, SI1142_PS_AUTO);
    }

    else if (mode == MEASUREMENT_MODE_CONVERSION)
    {
        setAutoMeasurementRate(OFF);
        write8(SI1142_REG_COMMAND, SI1142_PS_PAUSE);
    }
}

/**
 * @brief       Sets the auto measurement rate as a raw uint8_t
 *
 * @param       rate a uint8_t of the measurement rate.
 *
 * @note        Check the datasheet on how to calculate it, we reccomend sticking to the values defined in AUTO_MEAS_RATE
 *
 * @return      None
 *
 */
void Soldered_SI1142::setAutoMeasurementRate(uint8_t rate)
{
    write8(SI1142_REG_MEASRATE, rate);
}

/**
 * @brief       Sets the auto measurement rate
 *
 * @param       AUTO_MEAS_RATE the desired measurement rate
 * 
 * @note        Check the AUTO_MEAS_RATE enum for all the values that are available
 *
 * @return      None
 *
 */
void Soldered_SI1142::setAutoMeasurementRate(AUTO_MEAS_RATE rate)
{
    write8(SI1142_REG_MEASRATE, rate);
}

/**
 * @brief       Enable interrupts by the ambient light sensor
 * 
 * @note        Interrupts will be triggered on measurement completion, when a new value is ready to be read.
 *
 * @return      None
 *
 */
void Soldered_SI1142::enableALSInterrupts()
{
    write8(SI1142_REG_INTCFG, SI1142_PARAM_ENABLE_INT);
    write8(SI1142_REG_IRQMODE1, SI1142_PARAM_INT_ON_COMPLETE);

    uint8_t reg = read8(SI1142_REG_IRQEN);
    write8(SI1142_REG_IRQEN, reg | SI1142_PARAM_ALS_ENABLE_INT);
}

/**
 * @brief       Clear the interrupt set by the ambient light sensor
 * 
 * @note        This must be called every time the interrupt flag is read. Check example light_interruptOnComplete for more info.
 *
 * @return      None
 *
 */
void Soldered_SI1142::clearALSInterrupt()
{
    uint8_t reg = read8(SI1142_REG_IRQSTAT);
    write8(SI1142_REG_IRQSTAT, reg | SI1142_PARAM_ALS_CLEAR_INT);
}

/**
 * @brief       Enable interrupts by the proximity sensor
 * 
 * @note        Interrupts will be triggered on measurement completion, when a new value is ready to be read.
 *
 * @return      None
 *
 */
void Soldered_SI1142::enablePSInterrupts()
{
    write8(SI1142_REG_INTCFG, SI1142_PARAM_ENABLE_INT);
    write8(SI1142_REG_IRQMODE1, SI1142_PARAM_INT_ON_COMPLETE);

    uint8_t reg = read8(SI1142_REG_IRQEN);
    write8(SI1142_REG_IRQEN,
           reg | (ps1LEDEnabled * SI1142_PARAM_PS1_ENABLE_INT) | (ps2LEDEnabled * SI1142_PARAM_PS2_ENABLE_INT));
}

/**
 * @brief       Clear the interrupt set by the proximity sensor
 * 
 * @note        This must be called every time the interrupt flag is read. Check example proximity_interruptOnComplete for more info.
 *
 * @return      None
 *
 */
void Soldered_SI1142::clearPSInterrupts()
{
    uint8_t reg = read8(SI1142_REG_IRQSTAT);
    write8(SI1142_REG_IRQSTAT, reg | SI1142_PARAM_PS1_CLEAR_INT | SI1142_PARAM_PS2_CLEAR_INT);
}


/**
 * @brief       Enable proximity measurement on given LED pin
 *
 * @param       led selection of which led pin to enable (1 for LED1 and 2 for LED2)
 *
 * @return      None
 *
 */
void Soldered_SI1142::enableProximityLED(int led)
{
    if (led == 1)
    {
        ps1LEDEnabled = true;
    }
    if (led == 2)
    {
        ps2LEDEnabled = true;
    }

    writeParam(SI1142_PARAM_CHLIST, (ps1LEDEnabled * SI1142_PARAM_CHLIST_ENPS1) |
                                        (ps2LEDEnabled * SI1142_PARAM_CHLIST_ENPS2) | SI1142_PARAM_CHLIST_ENALSIR |
                                        SI1142_PARAM_CHLIST_ENALSVIS);

    write8(SI1142_REG_PSLED21,
           (ps1LEDEnabled * SI1142_PARAM_PS1_LED_CURRENT) | (ps2LEDEnabled * SI1142_PARAM_PS2_LED_CURRENT));

    writeParam(SI1142_PARAM_PSLED12SEL,
               (ps1LEDEnabled * SI1142_PARAM_PSLED12SEL_PS1LED1) | (ps2LEDEnabled * SI1142_PARAM_PSLED12SEL_PS2LED2));

    writeParam(SI1142_PARAM_PS1ADCMUX, SI1142_PARAM_ADCMUX_LARGEIR);
    writeParam(SI1142_PARAM_PS2ADCMUX, SI1142_PARAM_ADCMUX_LARGEIR);
    writeParam(SI1142_PARAM_PSADCGAIN, SI1142_PARAM_PS_ADC_GAIN);
    writeParam(SI1142_PARAM_PSADCOUNTER, SI1142_PARAM_ADCCOUNTER_511CLK);
    writeParam(SI1142_PARAM_PSADCMISC, SI1142_PARAM_PSADCMISC_RANGE | SI1142_PARAM_PSADCMISC_PSMODE);
    write8(SI1142_REG_PSRATE, SI1142_PARAM_PS_RATE_MAX); // Set PS to measure every time the device wakes up
    setPSMeasurementMode(currentMeasurementMode);        // Set the correct measurement mode
}


/**
 * @brief       Reset the sensor's registers to an initial state
 *
 */
void Soldered_SI1142::reset()
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
 * @brief       Read the visible light levels from the sensor
 *
 * @return      uint16_t of the visible light levels
 *
 */
uint16_t Soldered_SI1142::readVisible(void)
{
    if (currentMeasurementMode == MEASUREMENT_MODE_CONVERSION)
    {
        write8(SI1142_REG_COMMAND, SI1142_ALS_FORCE);
        delay(2);
    }

    uint16_t value = read16(0x22) - 255;

    // The sensor can potentially return negative values when there is almost no light
    // This is why an offset of 265 is added to the ADC
    // Values below 256 will thus overflow the uint16_t
    // We will consider the negative values as 0
    // For more information, view Si114X designer's guide chapter 5.6.3. ADC Number System

    if (value < 65535 - 256)
    {
        return value;
    }
    else
        return 0;
}

/**
 * @brief       Read the infrared light levels from the sensor
 *
 * @return      uint16_t of the IR light levels
 *
 */
uint16_t Soldered_SI1142::readIR(void)
{
    if (currentMeasurementMode == MEASUREMENT_MODE_CONVERSION)
    {
        write8(SI1142_REG_COMMAND, SI1142_ALS_FORCE);
        delay(2);
    }

    uint16_t value = read16(0x24) - 255;

    if (value < 65535 - 256)
    {
        return value;
    }
    else
        return 0;
}

/**
 * @brief       Gets the Proximity measurement, **Requires an attached IR LED**
 *
 * @note        View autoMeasureProximity.ino example for more info
 *
 * @param       int led, selector which LED's proximity to read (1 for LED1 and 2 for LED2)
 *
 * @return      uint16_t The proximity measurement, 0 if invalid LED selection
 */
uint16_t Soldered_SI1142::readProx(int led)
{
    uint16_t value;

    if (currentMeasurementMode == MEASUREMENT_MODE_CONVERSION)
    {
        write8(SI1142_REG_COMMAND, SI1142_PS_FORCE);
        delay(2);
    }

    if (led == 1)
    {
        value = read16(0x26) - 255;
    }
    else if (led == 2)
    {
        value = read16(0x28) - 255;
    }
    else
    {
        return 0;
    }

    if (value < 65535 - 256)
    {
        return value;
    }
    else
        return 0;
}


/*********************************************************************/

uint8_t Soldered_SI1142::writeParam(uint8_t p, uint8_t v)
{
    write8(SI1142_REG_PARAMWR, v);
    write8(SI1142_REG_COMMAND, p | SI1142_PARAM_SET);
    return read8(SI1142_REG_PARAMRD);
}

uint8_t Soldered_SI1142::readParam(uint8_t p)
{
    write8(SI1142_REG_COMMAND, p | SI1142_PARAM_QUERY);
    return read8(SI1142_REG_PARAMRD);
}

/*********************************************************************/

uint8_t Soldered_SI1142::read8(uint8_t reg)
{
    uint8_t buffer[1] = {reg};
    i2c_dev->write_then_read(buffer, 1, buffer, 1);
    return buffer[0];
}

uint16_t Soldered_SI1142::read16(uint8_t a)
{
    uint8_t buffer[2] = {a, 0};
    i2c_dev->write_then_read(buffer, 1, buffer, 2);
    return ((uint16_t)buffer[0]) | ((uint16_t)buffer[1] << 8);
}

void Soldered_SI1142::write8(uint8_t reg, uint8_t val)
{
    uint8_t buffer[2] = {reg, val};
    i2c_dev->write(buffer, 2);
}