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

#ifndef _Light_Sensor_
#define _Light_Sensor_

#include "Arduino.h"
#include "libs/Generic-easyC/easyC.h"

#define SI1141_ADDRESS          0x5A
#define SI1145_ADDRESS          0x60
//
// I2C Registers
//
#define REG_PART_ID               0x00
#define REG_REV_ID                0x01
#define REG_SEQ_ID                0x02
#define REG_INT_CFG               0x03
#define REG_IRQ_ENABLE            0x04
#define REG_IRQ_MODE1             0x05
#define REG_IRQ_MODE2             0x06
#define REG_HW_KEY                0x07
#define REG_MEAS_RATE             0x08
#define REG_ALS_RATE              0x09
#define REG_PS_RATE               0x0A
#define REG_ALS_LO_TH_LSB         0x0B
#define REG_ALS_LO_TH_MSB         0x0C
#define REG_ALS_HI_TH_LSB         0x0D
#define REG_ALS_HI_TH_MSB         0x0E
#define REG_PS_LED21              0x0F
#define REG_PS_LED3               0x10
#define REG_PS1_TH_LSB            0x11
#define REG_PS1_TH_MSB            0x12
#define REG_PS2_TH_LSB            0x13
#define REG_PS2_TH_MSB            0x14
#define REG_PS3_TH_LSB            0x15
#define REG_PS3_TH_MSB            0x16
#define REG_PARAM_WR              0x17
#define REG_COMMAND               0x18
#define REG_RESPONSE              0x20
#define REG_IRQ_STATUS            0x21
#define REG_ALS_VIS_DATA0         0x22
#define REG_ALS_VIS_DATA1         0x23
#define REG_ALS_IR_DATA0          0x24
#define REG_ALS_IR_DATA1          0x25
#define REG_PS1_DATA0             0x26
#define REG_PS1_DATA1             0x27
#define REG_PS2_DATA0             0x28
#define REG_PS2_DATA1             0x29
#define REG_PS3_DATA0             0x2A
#define REG_PS3_DATA1             0x2B
#define REG_AUX_DATA0             0x2C
#define REG_AUX_DATA1             0x2D
#define REG_PARAM_OUT             0x2E
#define REG_PARAM_RD              0x2E
#define REG_CHIP_STAT             0x30
#define REG_UCOEF0                0x13  
#define REG_UCOEF1                0x14
#define REG_UCOEF2                0x15  
#define REG_UCOEF3                0x16  

#define HW_KEY_VAL0               0x17
#define LED_MIN_CURRENT           0X00
#define LED_MAX_CURRENT           0X15

// PARAM_CH_LIST
#define PS1_TASK                  0x01
#define PS2_TASK                  0x02
#define PS3_TASK                  0x04
#define ALS_VIS_TASK              0x10
#define ALS_IR_TASK               0x20
#define AUX_TASK                  0x40

// COMMANDS 
#define PARAM_QUERY 0x80
#define PARAM_SET 0xA0
#define NOP 0x0
#define RESET 0x01
#define BUSADDR 0x02
#define PS_FORCE 0x05
#define ALS_FORCE 0x06
#define PSALS_FORCE 0x07
#define PS_PAUSE 0x09
#define ALS_PAUSE 0x0A
#define PSALS_PAUSE 0xB
#define PS_AUTO 0x0D
#define ALS_AUTO 0x0E
#define PSALS_AUTO 0x0F
#define GET_CAL 0x12

//Parameters
#define PARAM_I2CADDR 0x00
#define PARAM_CHLIST 0x01
#define PARAM_CHLIST_ENUV 0x80
#define PARAM_CHLIST_ENAUX 0x40
#define PARAM_CHLIST_ENALSIR 0x20
#define PARAM_CHLIST_ENALSVIS 0x10
#define PARAM_CHLIST_ENPS1 0x01
#define PARAM_CHLIST_ENPS2 0x02
#define PARAM_CHLIST_ENPS3 0x04

#define PARAM_PSLED12SEL 0x02
#define PARAM_PSLED12SEL_PS2NONE 0x00
#define PARAM_PSLED12SEL_PS2LED1 0x10
#define PARAM_PSLED12SEL_PS2LED2 0x20
#define PARAM_PSLED12SEL_PS2LED3 0x40
#define PARAM_PSLED12SEL_PS1NONE 0x00
#define PARAM_PSLED12SEL_PS1LED1 0x01
#define PARAM_PSLED12SEL_PS1LED2 0x02
#define PARAM_PSLED12SEL_PS1LED3 0x04

#define PARAM_PSLED3SEL 0x03
#define PARAM_PSENCODE 0x05
#define PARAM_ALSENCODE 0x06

#define PARAM_PS1ADCMUX 0x07
#define PARAM_PS2ADCMUX 0x08
#define PARAM_PS3ADCMUX 0x09
#define PARAM_PSADCOUNTER 0x0A
#define PARAM_PSADCGAIN 0x0B
#define PARAM_PSADCMISC 0x0C
#define PARAM_PSADCMISC_RANGE 0x20
#define PARAM_PSADCMISC_PSMODE 0x04

#define PARAM_ALSIRADCMUX 0x0E
#define PARAM_AUXADCMUX 0x0F

#define PARAM_ALSVISADCOUNTER 0x10
#define PARAM_ALSVISADCGAIN 0x11
#define PARAM_ALSVISADCMISC 0x12
#define PARAM_ALSVISADCMISC_VISRANGE 0x20

#define PARAM_ALSIRADCOUNTER 0x1D
#define PARAM_ALSIRADCGAIN 0x1E
#define PARAM_ALSIRADCMISC 0x1F
#define PARAM_ALSIRADCMISC_RANGE 0x20

#define PARAM_ADCCOUNTER_511CLK 0x70

#define PARAM_ADCMUX_SMALLIR 0x00
#define PARAM_ADCMUX_LARGEIR 0x03

#define REG_UCOEFF0 0x13
#define REG_UCOEFF1 0x14
#define REG_UCOEFF2 0x15
#define REG_UCOEFF3 0x16
#define REG_INTCFG 0x03
#define REG_INTCFG_INTOE 0x01
#define REG_INTCFG_INTMODE 0x02
#define REG_IRQEN 0x04
#define REG_IRQEN_ALSEVERYSAMPLE 0x01
#define REG_IRQEN_PS1EVERYSAMPLE 0x04
#define REG_IRQEN_PS2EVERYSAMPLE 0x08
#define REG_IRQEN_PS3EVERYSAMPLE 0x10

class SI114X : public SI114X_light_sensor_easyc::EasyC
{
  public:
    SI114X(int _pin);
    SI114X();
    float getLightIntensity();
    float getLightIntensityIR();
    bool Available();
    void init(void);
    char getReg(int addr);
    uint8_t paramSet(uint8_t, uint8_t);
    uint8_t paramRead(uint8_t);
    int16_t _waitUntilSleep();
    float calcGain(uint16_t);
    uint8_t readVisibleGain();
    uint8_t readIRGain();
    uint16_t readUV(void);
    void enableUV();

  protected:
    void initializeNative();

  private:
    int pin;
    char a[4], raw[2];
    uint16_t value;
};

#endif
