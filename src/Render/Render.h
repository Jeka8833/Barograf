//
// Created by Jeka8833 on 04.01.2022.
//

#ifndef BAROGRAFV3_RENDER_H
#define BAROGRAFV3_RENDER_H

#include "Arduino.h"
#include "../../.pio/libdeps/uno/U8g2/src/U8g2lib.h"

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

#define INFO_SCREEN 0
#define TIME_SCREEN 1
#define PRESSURE_SCREEN 2
#define MENU_SCREEN 3
#define LIGHT_SCREEN 4
#define ABOUT_SCREEN 5
#define PAGE_SETTING_SCREEN 6

class Render {
public:
    static U8G2_ST7920_128X64_1_HW_SPI u8g2;
    static uint8_t screen;
    static uint8_t scrollTime;
    static bool needUpdate;
    static bool secondAnimation;

    static void init();

    static void update();

    static void buttonAction(uint8_t button, uint8_t state);

    static void setBrightness(uint8_t value);

private:
    static bool lastSecondAnimation;
};


#endif //BAROGRAFV3_RENDER_H
