//
// Created by Jeka8833 on 08.01.2022.
//

#ifndef BAROGRAFV3_CONTROL_H
#define BAROGRAFV3_CONTROL_H

#include "Arduino.h"

#define BUTTON_MENU 1
#define BUTTON_SELECT 2
#define BUTTON_PLUS 4
#define BUTTON_MINUS 8
#define BUTTON_POTENTIOMETER 16

class Control {
public:
    static uint8_t potentiometer;
    static uint8_t lightSensor;

    static void init();

    static void update();

    static void tick();

private:
    static uint8_t activeButton;
    static uint8_t lastPotentiometer;
};

#endif //BAROGRAFV3_CONTROL_H
