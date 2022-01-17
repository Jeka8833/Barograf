//
// Created by Jeka8833 on 04.01.2022.
//

#include "PressureScreen.h"
#include "Render/Render.h"
#include "sensors/BMP.h"

#define WORK_AREA 114
#define MIN_PRESSURE 68557
#define MAX_PRESSURE 134091

uint16_t PressureScreen::pressures[COUNT_PRESSURE_COLUMN];

void PressureScreen::render() {
    uint16_t minV = UINT16_MAX;
    uint16_t maxV = 0;

    for (int i = 0; i < COUNT_PRESSURE_COLUMN; ++i) {
        const uint16_t value = pressures[i];
        if (value == 0)
            continue;
        maxV = max(maxV, value);
        minV = min(minV, value);
    }

    if (maxV == minV) {
        maxV++;
        minV--;
    }

    const uint16_t range = maxV - minV;

    Render::u8g2.setFont(u8g2_font_micro_mn);
    Render::u8g2.setDrawColor(2);

    for (int i = 0; i <= 8; ++i) {
        const uint8_t posY = 5 + ((DISPLAY_HEIGHT - 5) * i) / 8;
        const uint16_t value =
                (760 * (MIN_PRESSURE + minV + ((DISPLAY_HEIGHT - posY + 2) * range) / (DISPLAY_HEIGHT - 1))) / 101325;

        char str[4] = "000";
        str[0] += value / 100;
        str[1] += (value / 10) % 10;
        str[2] += value % 10;
        Render::u8g2.drawStr(DISPLAY_WIDTH - 11, posY, str);
        Render::u8g2.drawHLine(WORK_AREA, posY - 3, 3);
    }

    Render::u8g2.drawVLine(WORK_AREA + 1, 0, DISPLAY_HEIGHT);
    for (int i = 0; i < COUNT_PRESSURE_COLUMN; i++) {
        if (pressures[i] == 0)
            continue;
        const uint8_t startPos = ((uint32_t) (pressures[i] - minV) * (DISPLAY_HEIGHT - 1)) / range + 1;
        Render::u8g2.drawVLine((i * WORK_AREA) / COUNT_PRESSURE_COLUMN, DISPLAY_HEIGHT - startPos, startPos);
    }
}

void PressureScreen::addColumn() {
    for (int i = 1; i < COUNT_PRESSURE_COLUMN; i++) {
        pressures[i - 1] = pressures[i];
    }

    uint32_t value = BMP::pressure;
    if (value > MAX_PRESSURE)
        value = MAX_PRESSURE;
    if (value < MIN_PRESSURE)
        value = MIN_PRESSURE;
    pressures[COUNT_PRESSURE_COLUMN - 1] = value - MIN_PRESSURE;
}