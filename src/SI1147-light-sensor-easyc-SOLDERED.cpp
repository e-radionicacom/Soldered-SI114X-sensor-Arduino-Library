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


#include "SI1147-light-sensor-easyc-SOLDERED.h"

/**
 * @brief                   Default constructor
 * 
 */
SI114X::SI114X()
{
    begin(SI1141_ADDRESS);
    if(sendAddress(0x00)) begin(SI1145_ADDRESS); 
    init();
}

/**
 * @brief                   Sensor specific native constructor.
 *
 * @param int _pin          Example parameter.
 */
SI114X::SI114X(int _pin)
{
    pin = _pin;
    native = 1;
}

/**
 * @brief                   Overloaded function for virtual in base class to initialize sensor specific.
 */
void SI114X::initializeNative()
{
    pinMode(pin, INPUT);
}

/**
 * @brief                   Function which gets light intensity data through I2C communication
 *                          and converts it to int
 *
 * @return                  light intensity
 */
float SI114X::getLightIntensity()
{   
    readRegister(REG_ALS_VIS_DATA0, raw,
                 sizeof(uint8_t) * 2); // Get two bytes that consist of light intensity data
    if(!raw[1]) return 0;
    value = ((raw[0] | raw[1] << 8) - 256) ;       // I2C sends 8 bits at once, this function merges two bytes into one variable
    float lux = ((5.41f * value) + (-0.08f * getLightIntensityIR())) / (1 + 2 * readVisibleGain());
    return lux;
}


float SI114X::getLightIntensityIR()
{   
    readRegister(REG_ALS_IR_DATA0, raw,
                 sizeof(uint8_t) * 2); // Get two bytes that consist of light intensity data
    if(!raw[1]) return 0;
    value = (raw[0] | raw[1] << 8) - 256;       // I2C sends 8 bits at once, this function merges two bytes into one variable
    float luxir = value / (1 * 2.44f);
    if (luxir < 0)
        luxir = 0.0;
    return luxir;
}

uint16_t SI114X::readUV(void) 
{ 
    if(getReg(REG_PART_ID) | 0X04) return 0;
    readRegister(REG_AUX_DATA0 , raw, sizeof(uint8_t) * 2);
    value = (raw[0] | raw[1] << 8) / 100.0;

    return value;

}

/**
 * @brief                   Function which checks if I2C device is responding
 *
 * @return                  1 if device is responding, 0 if not
 */
bool SI114X::Available()
{
    return !(
        sendAddress(REG_PART_ID)); // checking if address is succesfully sent, if not device is not ready to communicate
}

/**
 * @brief                   Function that fetches specific register data
 *
 * @param int _addr          Example parameter.
 *
 * @return                  specific register data
 */
char SI114X::getReg(int addr)
{
    char a = 'a';
    readRegister(addr, &a, 1);
    return a;
}


/**
 * @brief                   Function which initializes sensor
 *
 */
void SI114X::init()
{
    a[0] = REG_HW_KEY;
    a[1] = HW_KEY_VAL0;
    sendData((const uint8_t*) a, 2 * sizeof(char));

    readRegister(REG_PART_ID, a ,sizeof(char));
    if(*a | 0x04)
    {
        enableUV();
    }

    paramSet(PARAM_CHLIST,0x30);
  
    paramSet(PARAM_ALSIRADCMUX, PARAM_ADCMUX_SMALLIR);
    // fastest clocks, clock div 1
    paramSet(PARAM_ALSIRADCGAIN, 0);
    // take 511 clocks to measure
    paramSet(PARAM_ALSIRADCOUNTER, PARAM_ADCCOUNTER_511CLK);
    // in high range mode
    paramSet(PARAM_ALSIRADCMISC, PARAM_ALSIRADCMISC_RANGE);

    // fastest clocks, clock div 1
    paramSet(PARAM_ALSVISADCGAIN, 0);
    // take 511 clocks to measure
    paramSet(PARAM_ALSVISADCOUNTER, PARAM_ADCCOUNTER_511CLK);
    // in high range mode (not normal signal)
    paramSet(PARAM_ALSVISADCMISC, PARAM_ALSVISADCMISC_VISRANGE);

    /************************/

    // measurement rate for auto
    a[0] = REG_MEAS_RATE;
    a[1] = 0x84; // 255 * 31.25uS = 8ms
    sendData((const uint8_t*)a, 2 * sizeof(char));

    a[0] = REG_ALS_RATE;
    a[1] = 0x08; // 255 * 31.25uS = 8ms
    sendData((const uint8_t*)a, 2 * sizeof(char));

    // auto run
    a[0] = REG_COMMAND;
    a[1] =  PSALS_AUTO;
    sendData((const uint8_t*)a, 2 * sizeof(char));

    a[0] = REG_PS_LED21;
    a[1] = (LED_MIN_CURRENT << 4) | LED_MIN_CURRENT;
    sendData((const uint8_t*)a, 2 * sizeof(char));
    a[0] = REG_PS_LED3;
    a[1] = LED_MIN_CURRENT;
    sendData((const uint8_t*)a, 2 * sizeof(char));

    a[0] = REG_COMMAND;
    a[1] = 0x07;
    sendData((const uint8_t*)a, 2 * sizeof(char));

    a[0] = REG_COMMAND;
    a[1] = 0x07;
    sendData((const uint8_t*)a, 2 * sizeof(char));

}

int16_t SI114X::_waitUntilSleep()
{
    uint8_t  retval = -1;
    uint8_t count = 0;
    // This loops until the Si114x is known to be in its sleep state
    // or if an i2c error occurs
    while(count < 200)
    {
        readRegister(REG_CHIP_STAT ,(char*) &retval , 1);
        if(retval == 1) break;
        if(retval <  0) return retval;
        count++;
        delay(1);
    }
    return 0;
}

uint8_t SI114X::paramSet(uint8_t add, uint8_t val){
    uint8_t retval;
    uint8_t response_stored;
    uint8_t response;
    if((retval = _waitUntilSleep())!=0) return retval;
    readRegister(REG_RESPONSE ,(char*) &response_stored, 1);
    a[0] = REG_PARAM_WR;
    a[1] = val;
    a[2] = 0xA0 + (add & 0x1F);
    sendData((const uint8_t*)a ,sizeof(uint8_t) * 3);
    if(retval != 0) return retval;

// Wait for command to finish
    readRegister(REG_RESPONSE,(char*) &response , 1);
    while(response == response_stored )
    {
        readRegister(REG_RESPONSE,(char*) &response , 1);
        if (response == response_stored)
    {
        delay(1);
    }
    }
    if(retval < 0)
        return retval;
    else
        return 0;

}

uint8_t SI114X::paramRead(uint8_t addr)
{
    a[0] = REG_COMMAND;
    a[1] = addr | 0x80;
    sendData((const uint8_t*)a,2);
    readRegister(REG_PARAM_RD, a, 1);
    return *a;
}

// returns the visible gain
uint8_t SI114X::readVisibleGain() {

  return paramRead(PARAM_ALSVISADCGAIN);
}

// returns the IR gain
uint8_t SI114X::readIRGain() {
  return paramRead(PARAM_ALSIRADCGAIN);
}

void SI114X::enableUV(){
    a[0] = REG_UCOEFF0;
    a[1] = 0x29;
    sendData((const uint8_t*) a, sizeof(char)*2);
    a[0] = REG_UCOEFF1;
    a[1] = 0x89;
    sendData((const uint8_t*) a, sizeof(char)*2);
    a[0] = REG_UCOEFF2;
    a[1] = 0x02;
    sendData((const uint8_t*) a, sizeof(char)*2);
    a[0] = REG_UCOEFF3;
    a[1] = 0x00;
    sendData((const uint8_t*) a, sizeof(char)*2);
    // enable UV sensor
    paramSet(PARAM_CHLIST, PARAM_CHLIST_ENUV | PARAM_CHLIST_ENALSIR | PARAM_CHLIST_ENALSVIS | PARAM_CHLIST_ENPS1);
    // enable interrupt on every sample
    a[0] = REG_INTCFG;
    a[1] = REG_INTCFG_INTOE;
    sendData((const uint8_t*) a, sizeof(char)*2);
    a[0] = REG_IRQEN;
    a[1] = REG_IRQEN_ALSEVERYSAMPLE;
    sendData((const uint8_t*) a, sizeof(char)*2);
}