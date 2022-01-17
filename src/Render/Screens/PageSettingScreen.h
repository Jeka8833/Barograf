//
// Created by Jeka8833 on 11.01.2022.
//

#ifndef BAROGRAFV3_PAGESETTINGSCREEN_H
#define BAROGRAFV3_PAGESETTINGSCREEN_H

#include "Arduino.h"

class PageSettingScreen {
public:
    static uint8_t editItem;
    static bool isEdit;

    static void render();

    static void buttonChangeState(uint8_t button, uint8_t state);
};


#endif //BAROGRAFV3_PAGESETTINGSCREEN_H
