//
// Created by Jeka8833 on 09.01.2022.
//

#include "LightScreen.h"
#include "Render/Control.h"
#include "Render/Render.h"
#include "Config.h"

#define WORK_AREA 114

uint8_t LightScreen::editItem;

void LightScreen::render() {
    Render::u8g2.setFont(u8g2_font_micro_mn);
    Render::u8g2.setDrawColor(1);
    Render::u8g2.drawVLine(WORK_AREA + 1, 0, DISPLAY_HEIGHT);

    for (int i = 0; i <= COUNT_LABELS; ++i) {
        const uint8_t posY = 5 + ((DISPLAY_HEIGHT - 5) * i) / COUNT_LABELS;
        int value = ((COUNT_LABELS - i) * 100) / COUNT_LABELS;
        char str[4] = "000";
        str[0] += value / 100;
        str[1] += (value / 10) % 10;
        str[2] += value % 10;
        Render::u8g2.drawStr(DISPLAY_WIDTH - 11, posY, str);
        for (int x = 0; x <= WORK_AREA + 1; x += 4) {
            Render::u8g2.drawPixel(x, posY - 3);
        }
    }
    uint8_t lastX;
    uint8_t lastY;
    for (int i = 0; i < LIGHT_COUNT_COLUMN; i++) {
        const uint8_t posX = 5 + (i * (WORK_AREA - 5 - 5)) / (LIGHT_COUNT_COLUMN - 1);
        const uint8_t posY = DISPLAY_HEIGHT - (3 + (Config::lightParam[i] * (DISPLAY_HEIGHT - 3 - 3)) / 255);
        if (!(editItem == i + 1 && Render::secondAnimation))
            Render::u8g2.drawVLine(posX, posY, DISPLAY_HEIGHT - posY);
        Render::u8g2.drawBox(posX - 1, posY - 1, 3, 3);
        if (i) {
            Render::u8g2.drawLine(lastX, lastY, posX, posY);
        }
        lastX = posX;
        lastY = posY;
    }

    const uint8_t lightSensorLinePosX = 5 + (Control::lightSensor * (WORK_AREA - 5 - 5)) / 255;
    for (int i = 0; i < DISPLAY_HEIGHT; i += 2)
        Render::u8g2.drawPixel(lightSensorLinePosX, i);
    const uint8_t correctedLight =
            5 + (min(255, max(0, (((int16_t) Control::lightSensor) + ((int16_t) Control::potentiometer - 127)))) *
                 (WORK_AREA - 5 - 5)) / 255;
    Render::u8g2.drawVLine(correctedLight, 0, DISPLAY_HEIGHT);
}

void LightScreen::buttonChangeState(uint8_t button, uint8_t state) {
    if (state) {
        switch (button) {
            case BUTTON_MENU:
                if (editItem) {
                    editItem = 0;
                    Config::read();
                } else {
                    Render::screen = MENU_SCREEN;
                }
                break;
            case BUTTON_SELECT:
                editItem++;
                if (editItem > LIGHT_COUNT_COLUMN) {
                    Config::write();
                    editItem = 0;
                }
                break;
        }
    }
    if (button == BUTTON_POTENTIOMETER && editItem) {
        Config::lightParam[editItem - 1] = state;
    }
}

uint8_t LightScreen::getScreenBrightness() {
    if (editItem)
        return Config::lightParam[editItem - 1];

    const uint8_t currentValue = min(255, max(0, (((int16_t) Control::lightSensor) +
                                                  ((int16_t) Control::potentiometer - 127))));
    for (int i = 0; i < LIGHT_COUNT_COLUMN - 1; i++) {
        const uint8_t minValue = (255 * i) / (LIGHT_COUNT_COLUMN - 1);
        const uint8_t maxValue = (255 * (i + 1)) / (LIGHT_COUNT_COLUMN - 1);
        if (minValue <= currentValue && maxValue >= currentValue) {
            return Config::lightParam[i] +
                   ((currentValue - minValue) * (Config::lightParam[i + 1] - Config::lightParam[i])) /
                   (maxValue - minValue);
        }
    }
    return 255;
}
