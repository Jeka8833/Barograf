//
// Created by Jeka8833 on 04.01.2022.
//

#include "PressureScreen.h"
#include "Render/Render.h"
#include "sensors/BMP.h"

#define WORK_AREA 114

// 700 * (101325 / 760)
//#define MIN_PRESSURE 93325
// 800 * (101325 / 760)
//#define MAX_PRESSURE 106657

// 700 * (101325 / 760)
#define MIN_PRESSURE 68557
// 800 * (101325 / 760)
#define MAX_PRESSURE 134091

uint16_t PressureScreen::pressures[COUNT_PRESSURE_COLUMN];

void PressureScreen::render() {
    // 101325 is 760 mm.rt
    //constexpr uint16_t normPressureInByte = 101325 - MIN_PRESSURE;

    uint16_t minV = UINT16_MAX;
    uint16_t maxV = 0;

    for (int i = 0; i < COUNT_PRESSURE_COLUMN; ++i) {
        const uint16_t value = pressures[i];
        if(value < 2)
            continue;
        maxV = max(maxV, value + 1);
        minV = min(minV, value - 1);
    }

    const uint16_t range = maxV - minV;
    //const uint8_t normPressure = normPressureInByte > maxV ? 0 :
    //                             normPressureInByte < minV ? DISPLAY_HEIGHT :
    //                             DISPLAY_HEIGHT - ((normPressureInByte - minV) * DISPLAY_HEIGHT) / range;


    Render::u8g2.setFont(u8g2_font_micro_mn);
    Render::u8g2.setDrawColor(2);
    /*for (int i = 0; i <= WORK_AREA + 2; i += 2)
        Render::u8g2.drawPixel(i, normPressure);

    Render::u8g2.drawStr(DISPLAY_WIDTH - 11, min(max(normPressure + 3, 5), DISPLAY_HEIGHT), "760");
    */

    for (int i = 0; i <= 8; ++i) {
        const uint8_t posY = 5 + ((DISPLAY_HEIGHT - 5) * i) / 8;
        //if (!(posY > normPressure - 3 && posY < normPressure + 9)) {
        const uint16_t value = (((minV + ((float) (DISPLAY_HEIGHT - posY - 3) / DISPLAY_HEIGHT) * range) *
                                       (MAX_PRESSURE - MIN_PRESSURE)) / 0xFFFF +
                                      MIN_PRESSURE) /
                                     (101325.f / 760);

            char str[4] = "000";
            str[0] += value / 100;
            str[1] += (value / 10) % 10;
            str[2] += value % 10;
            Render::u8g2.drawStr(DISPLAY_WIDTH - 11, posY, str);
            Render::u8g2.drawHLine(WORK_AREA, posY - 3, 3);
       //}
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