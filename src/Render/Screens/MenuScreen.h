//
// Created by Jeka8833 on 10.01.2022.
//

#ifndef BAROGRAFV3_MENUSCREEN_H
#define BAROGRAFV3_MENUSCREEN_H

#include "Arduino.h"

class MenuScreen {
public:
    static void render();

    static void buttonChangeState(uint8_t button, uint8_t state);

private:
    static uint8_t editItem;

};


#endif //BAROGRAFV3_MENUSCREEN_H
