//
// Created by Jeka8833 on 17.10.2020.
//

#ifndef UNTITLED2_BMP_H
#define UNTITLED2_BMP_H

#include "Arduino.h"

class BMP {

public:
    static uint32_t pressure;
    //static float temperature;

    static void begin();

    static void update();

private:
    static int32_t computeB5(int32_t UT);

    static uint8_t read8(uint8_t addr);

    static uint16_t read16(uint8_t addr);

    static void write8(uint8_t addr, uint8_t data);

    static int16_t ac1, ac2, ac3, b1, b2, /*mb,*/ mc, md;
    static uint16_t ac4, ac5, ac6;
};


#endif //UNTITLED2_BMP_H
