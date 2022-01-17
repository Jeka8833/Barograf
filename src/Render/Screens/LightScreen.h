//
// Created by Jeka8833 on 09.01.2022.
//

#ifndef BAROGRAFV3_LIGHTSCREEN_H
#define BAROGRAFV3_LIGHTSCREEN_H

#include "Arduino.h"

#define COUNT_LABELS 5

class LightScreen {
public:
    static uint8_t editItem;
    static void render();

    static void buttonChangeState(uint8_t button, uint8_t state);

    static uint8_t getScreenBrightness();
};


#endif //BAROGRAFV3_LIGHTSCREEN_H
