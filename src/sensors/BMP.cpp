//
// Created by Jeka8833 on 17.10.2020.
//

#include "BMP.h"
#include <Wire.h>

#define BMP085_I2CADDR 0x77 //!< BMP085 I2C address

#define BMP085_CAL_AC1 0xAA    //!< R   Calibration data (16 bits)
#define BMP085_CAL_AC2 0xAC    //!< R   Calibration data (16 bits)
#define BMP085_CAL_AC3 0xAE    //!< R   Calibration data (16 bits)
#define BMP085_CAL_AC4 0xB0    //!< R   Calibration data (16 bits)
#define BMP085_CAL_AC5 0xB2    //!< R   Calibration data (16 bits)
#define BMP085_CAL_AC6 0xB4    //!< R   Calibration data (16 bits)
#define BMP085_CAL_B1 0xB6     //!< R   Calibration data (16 bits)
#define BMP085_CAL_B2 0xB8     //!< R   Calibration data (16 bits)
#define BMP085_CAL_MB 0xBA     //!< R   Calibration data (16 bits)
#define BMP085_CAL_MC 0xBC     //!< R   Calibration data (16 bits)
#define BMP085_CAL_MD 0xBE     //!< R   Calibration data (16 bits)

#define BMP085_CONTROL 0xF4         //!< Control register
#define BMP085_TEMPDATA 0xF6        //!< Temperature data register
#define BMP085_PRESSUREDATA 0xF6    //!< Pressure data register
#define BMP085_READTEMPCMD 0x2E     //!< Read temperature control register value
#define BMP085_READPRESSURECMD 0x34 //!< Read pressure control register value


uint32_t BMP::pressure = 0;
//float BMP::temperature = 0;

int16_t  BMP::ac1, BMP::ac2, BMP::ac3, BMP::b1, BMP::b2, /*BMP::mb,*/ BMP::mc, BMP::md;
uint16_t  BMP::ac4, BMP::ac5, BMP::ac6;

void BMP::begin()
{
    if (read8(0xD0) != 0x55)
        return;

    /* read calibration data */
    ac1 = read16(BMP085_CAL_AC1);
    ac2 = read16(BMP085_CAL_AC2);
    ac3 = read16(BMP085_CAL_AC3);
    ac4 = read16(BMP085_CAL_AC4);
    ac5 = read16(BMP085_CAL_AC5);
    ac6 = read16(BMP085_CAL_AC6);

    b1 = read16(BMP085_CAL_B1);
    b2 = read16(BMP085_CAL_B2);

    //mb = read16(BMP085_CAL_MB);
    mc = read16(BMP085_CAL_MC);
    md = read16(BMP085_CAL_MD);
}

void BMP::update() {
    int32_t UP, B3, B5, B6, X1, X2, X3, p;
    uint32_t B4, B7;


    write8(BMP085_CONTROL, BMP085_READPRESSURECMD + (3 << 6));
    delay(26);

    UP = read16(BMP085_PRESSUREDATA);

    UP <<= 8;
    UP |= read8(BMP085_PRESSUREDATA + 2);
    UP >>= (8 - 3);


    write8(BMP085_CONTROL, BMP085_READTEMPCMD);
    delay(5);
    B5 = computeB5(read16(BMP085_TEMPDATA));

    //temperature = ((B5 + 8) >> 4) / 10.f;

    B6 = B5 - 4000;
    X1 = ((int32_t)b2 * ((B6 * B6) >> 12)) >> 11;
    X2 = ((int32_t)ac2 * B6) >> 11;
    X3 = X1 + X2;
    B3 = ((((int32_t)ac1 * 4 + X3) << 3) + 2) / 4;

    X1 = ((int32_t)ac3 * B6) >> 13;
    X2 = ((int32_t)b1 * ((B6 * B6) >> 12)) >> 16;
    X3 = ((X1 + X2) + 2) >> 2;
    B4 = ((uint32_t)ac4 * (uint32_t)(X3 + 32768)) >> 15;
    B7 = ((uint32_t)UP - B3) * (uint32_t)(50000UL >> 3);


    if (B7 < 0x80000000) {
        p = (B7 * 2) / B4;
    }
    else {
        p = (B7 / B4) * 2;
    }
    X1 = (p >> 8) * (p >> 8);
    X1 = (X1 * 3038) >> 16;
    X2 = (-7357 * p) >> 16;

    pressure = (p + ((X1 + X2 + (int32_t)3791) >> 4));

}

int32_t BMP::computeB5(int32_t UT)
{
    int32_t X1 = (UT - (int32_t)ac6) * ((int32_t)ac5) >> 15;
    int32_t X2 = ((int32_t)mc << 11) / (X1 + (int32_t)md);
    return X1 + X2;
}

uint8_t BMP::read8(uint8_t addr)
{
    uint8_t ret;

    Wire.beginTransmission(BMP085_I2CADDR); // start transmission to device
    Wire.write(addr); // sends register address to read from
    Wire.endTransmission(); // end transmission

    Wire.beginTransmission(BMP085_I2CADDR); // start transmission to device
    Wire.requestFrom(BMP085_I2CADDR, 1);    // send data n-bytes read
    ret = Wire.read(); // receive DATA
    Wire.endTransmission(); // end transmission

    return ret;
}

uint16_t BMP::read16(uint8_t addr)
{
    uint16_t ret;

    Wire.beginTransmission(BMP085_I2CADDR); // start transmission to device
    Wire.write(addr); // sends register address to read from
    Wire.endTransmission(); // end transmission

    Wire.beginTransmission(BMP085_I2CADDR); // start transmission to device
    Wire.requestFrom(BMP085_I2CADDR, 2);    // send data n-bytes read
    ret = Wire.read(); // receive DATA
    ret <<= 8;
    ret |= Wire.read(); // receive DATA

    Wire.endTransmission(); // end transmission

    return ret;
}

void BMP::write8(uint8_t addr, uint8_t data)
{
    Wire.beginTransmission(BMP085_I2CADDR); // start transmission to device
    Wire.write(addr); // sends register address to read from
    Wire.write(data); // write data
    Wire.endTransmission(); // end transmission
}
