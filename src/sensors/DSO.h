//
// Created by Jeka8833 on 19.10.2020.
//

#ifndef BAROGRAPHV2_DSO_H
#define BAROGRAPHV2_DSO_H

#include "Arduino.h"

class DSO {
public:
    static uint8_t Minute;
    static uint8_t Hour;
    static uint8_t wDay;
    static uint8_t Day;
    static uint8_t Month;
    static uint16_t Year;

    static void read();

    static void write();

private:
    static constexpr uint8_t bcd2dec(uint8_t n);

    static constexpr uint8_t decToBcd(uint8_t n);
};


#endif //BAROGRAPHV2_DSO_H
