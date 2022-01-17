//
// Created by Jeka8833 on 09.01.2022.
//

#ifndef BAROGRAFV3_CONFIG_H
#define BAROGRAFV3_CONFIG_H

#include "Arduino.h"

#define LIGHT_COUNT_COLUMN 4
#define MAIN_PAGE_COUNT 3

class Config {
public:
    static uint8_t beeper;
    static uint8_t lightParam[LIGHT_COUNT_COLUMN];
    static uint8_t pageSetting[MAIN_PAGE_COUNT];

    static void read();

    static void write();
};


#endif //BAROGRAFV3_CONFIG_H
