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

  Modified by Soldered electronics for use on SI114X breakout board
 ****************************************************/
#ifndef _SI1142_H_
#define _SI1142_H_

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "Adafruit_I2CDevice.h"

/* COMMANDS */
#define SI1142_PARAM_QUERY 0x80
#define SI1142_PARAM_SET   0xA0
#define SI1142_PARAM_AND   0xB0
#define SI1142_PARAM_OR    0xE0
#define SI1142_NOP         0x00
#define SI1142_RESET       0x01
#define SI1142_BUSADDR     0x02
#define SI1142_PS_FORCE    0x05
#define SI1142_ALS_FORCE   0x06
#define SI1142_PSALS_FORCE 0x07
#define SI1142_PS_PAUSE    0x09
#define SI1142_ALS_PAUSE   0x0A
#define SI1142_PSALS_PAUSE 0xB
#define SI1142_PS_AUTO     0x0D
#define SI1142_ALS_AUTO    0x0E
#define SI1142_PSALS_AUTO  0x0F

/* Parameters */
#define SI1142_PARAM_I2CADDR         0x00
#define SI1142_PARAM_CHLIST          0x01
#define SI1142_PARAM_CHLIST_ENAUX    0x40
#define SI1142_PARAM_CHLIST_ENALSIR  0x20
#define SI1142_PARAM_CHLIST_ENALSVIS 0x10
#define SI1142_PARAM_CHLIST_ENPS3    0x04
#define SI1142_PARAM_CHLIST_ENPS2    0x02
#define SI1142_PARAM_CHLIST_ENPS1    0x01

#define SI1142_PARAM_PSLED12SEL         0x02
#define SI1142_PARAM_PSLED12SEL_PS2NONE 0x00
#define SI1142_PARAM_PSLED12SEL_PS2LED1 0x10
#define SI1142_PARAM_PSLED12SEL_PS2LED2 0x20
#define SI1142_PARAM_PSLED12SEL_PS2LED3 0x40
#define SI1142_PARAM_PSLED12SEL_PS1NONE 0x00
#define SI1142_PARAM_PSLED12SEL_PS1LED1 0x01
#define SI1142_PARAM_PSLED12SEL_PS1LED2 0x02
#define SI1142_PARAM_PSLED12SEL_PS1LED3 0x04

#define SI1142_PARAM_PSLED3SEL 0x03
#define SI1142_PARAM_PSENCODE  0x05
#define SI1142_PARAM_ALSENCODE 0x06

#define SI1142_PARAM_PS1ADCMUX        0x07
#define SI1142_PARAM_PS2ADCMUX        0x08
#define SI1142_PARAM_PS3ADCMUX        0x09
#define SI1142_PARAM_PSADCOUNTER      0x0A
#define SI1142_PARAM_PSADCGAIN        0x0B
#define SI1142_PARAM_PSADCMISC        0x0C
#define SI1142_PARAM_PSADCMISC_RANGE  0x20
#define SI1142_PARAM_PSADCMISC_PSMODE 0x04

#define SI1142_PARAM_ALSIRADCMUX 0x0E
#define SI1142_PARAM_AUXADCMUX   0x0F

#define SI1142_PARAM_ALSVISADCOUNTER        0x10
#define SI1142_PARAM_ALSVISADCGAIN          0x11
#define SI1142_PARAM_ALSVISADCMISC          0x12
#define SI1142_PARAM_ALSVISADCMISC_VISRANGE 0x20

#define SI1142_PARAM_ALSIRADCOUNTER     0x1D
#define SI1142_PARAM_ALSIRADCGAIN       0x1E
#define SI1142_PARAM_ALSIRADCMISC       0x1F
#define SI1142_PARAM_ALSIRADCMISC_RANGE 0x20
#define SI1142_PARAM_ADCCOUNTER_511CLK 0x70
#define SI1142_PARAM_ADCMUX_SMALLIR 0x00
#define SI1142_PARAM_ADCMUX_LARGEIR 0x03

#define SI1142_PARAM_MAX_LED12_CURRENT 0xFF
#define SI1142_PARAM_MAX_LED3_CURRENT  0x0F

#define SI1142_PARAM_ADCOFFSET 0x1A

/* REGISTERS */
#define SI1142_REG_PARTID 0x00
#define SI1142_REG_REVID  0x01
#define SI1142_REG_SEQID  0x02

#define SI1142_REG_INTCFG   0x03
#define SI1142_REG_IRQEN    0x04
#define SI1142_REG_IRQMODE1 0x05
#define SI1142_REG_IRQMODE2 0x06

#define SI1142_REG_HWKEY       0x07
#define SI1142_REG_MEASRATE    0x08
#define SI1142_REG_ALSRATE     0x09
#define SI1142_REG_PSRATE      0x0A
#define SI1142_REG_ALS_LOW_TH0 0x0B
#define SI1142_REG_ALS_LOW_TH1 0x0C
#define SI1142_REG_ALS_HI_TH0  0x0D
#define SI1142_REG_ALS_HI_TH1  0x0E

#define SI1142_REG_PSLED21 0x0F
#define SI1142_REG_PSLED3  0x10

#define SI1142_REG_PS1_TH0 0x11
#define SI1142_REG_PS1_TH1 0x12
#define SI1142_REG_PS2_TH0 0x13
#define SI1142_REG_PS2_TH1 0x14
#define SI1142_REG_PS3_TH0 0x15
#define SI1142_REG_PS3_TH1 0x16

#define SI1142_REG_PARAMWR  0x17
#define SI1142_REG_COMMAND  0x18
#define SI1142_REG_RESPONSE 0x20
#define SI1142_REG_IRQSTAT  0x21

#define SI1142_REG_ALSVISDATA0 0x22
#define SI1142_REG_ALSVISDATA1 0x23
#define SI1142_REG_ALSIRDATA0  0x24
#define SI1142_REG_ALSIRDATA1  0x25
#define SI1142_REG_PS1DATA0    0x26
#define SI1142_REG_PS1DATA1    0x27
#define SI1142_REG_PS2DATA0    0x28
#define SI1142_REG_PS2DATA1    0x29
#define SI1142_REG_PS3DATA0    0x2A
#define SI1142_REG_PS3DATA1    0x2B

#define SI1142_REG_AUXDATA0 0x2C
#define SI1142_REG_AUXDATA1 0x2D

#define SI1142_REG_PARAMRD  0x2E
#define SI1142_REG_CHIPSTAT 0x30

#define SI1142_ADDR 0x5A

#define MEASUREMENT_MODE_AUTO       1
#define MEASUREMENT_MODE_CONVERSION 2

/**
 * @brief Library for using the Si1145 UV/IR/Visible Light Sensor
 *
 */
class Adafruit_SI1142
{
  public:
    Adafruit_SI1142(void);
    ~Adafruit_SI1142();
    boolean begin(uint8_t addr = SI1142_ADDR, TwoWire *pBus = &Wire);
    boolean begin(TwoWire *pBus);
    void reset();

    uint16_t readIR();
    uint16_t readVisible();
    uint16_t readProx();

    void setMeasurementMode(int measMode);

  private:
    uint16_t read16(uint8_t addr);
    uint8_t read8(uint8_t addr);
    void write8(uint8_t reg, uint8_t val);
    uint8_t readParam(uint8_t p);
    uint8_t writeParam(uint8_t p, uint8_t v);
    Adafruit_I2CDevice *i2c_dev = NULL; ///< Pointer to I2C bus interface
};
#endif
