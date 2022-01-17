//
// Created by Jeka8833 on 06.01.2022.
//

#ifndef BAROGRAFV3_TIMESCREEN_H
#define BAROGRAFV3_TIMESCREEN_H

#include "Arduino.h"

class TimeScreen {
public:
    static uint8_t editItem;

    static void render();

    static void buttonChangeState(uint8_t button, uint8_t state);

private:
    static uint8_t getNumberOfDays(uint8_t month, uint16_t year);
};


#endif //BAROGRAFV3_TIMESCREEN_H
