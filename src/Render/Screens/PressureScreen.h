//
// Created by Jeka8833 on 04.01.2022.
//

#ifndef BAROGRAFV3_PRESSURESCREEN_H
#define BAROGRAFV3_PRESSURESCREEN_H

#include "Arduino.h"

// Column per dey
#define COUNT_PRESSURE_COLUMN 76


class PressureScreen {
public:
    static uint16_t pressures[COUNT_PRESSURE_COLUMN];

    static void render();

    static void addColumn();
};


#endif //BAROGRAFV3_PRESSURESCREEN_H
