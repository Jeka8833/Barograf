//
// Created by Jeka8833 on 11.01.2022.
//

#include "PageSettingScreen.h"
#include "Render/Render.h"
#include "Render/Control.h"
#include "Config.h"

bool PageSettingScreen::isEdit;
uint8_t PageSettingScreen::editItem;

void PageSettingScreen::render() {
    Render::u8g2.setFont(u8g2_font_6x13_t_cyrillic);
    Render::u8g2.setDrawColor(2);

    Render::u8g2.drawUTF8((DISPLAY_WIDTH - Render::u8g2.getUTF8Width("Время страницы")) >> 1, 9, "Время страницы");
    Render::u8g2.drawHLine(0, 11, DISPLAY_WIDTH);

    char infoPageText[34] = "ст.Информация: 000с.";
    char timePageText[29] = "ст.Время:      000с.";
    char pressurePageText[32] = "ст.Давление:   000с.";

    uint8_t infoPageValue = Config::pageSetting[0];
    uint8_t timePageValue = Config::pageSetting[1];
    uint8_t pressurePageValue = Config::pageSetting[2];
    infoPageText[27] += infoPageValue / 100;
    infoPageText[28] += (infoPageValue / 10) % 10;
    infoPageText[29] += infoPageValue % 10;

    timePageText[22] += timePageValue / 100;
    timePageText[23] += (timePageValue / 10) % 10;
    timePageText[24] += timePageValue % 10;

    pressurePageText[25] += pressurePageValue / 100;
    pressurePageText[26] += (pressurePageValue / 10) % 10;
    pressurePageText[27] += pressurePageValue % 10;

    Render::u8g2.drawUTF8(0, 23, infoPageText);
    Render::u8g2.drawUTF8(0, 36, timePageText);
    Render::u8g2.drawUTF8(0, 49, pressurePageText);

    if(isEdit || Render::secondAnimation)
    switch (editItem) {
        case 0:
            Render::u8g2.drawHLine(90, 24, 30);
            break;
        case 1:
            Render::u8g2.drawHLine(90, 37, 30);
            break;
        case 2:
            Render::u8g2.drawHLine(90, 50, 30);
            break;
    }
}

void PageSettingScreen::buttonChangeState(uint8_t button, uint8_t state) {
    if (state) {
        switch (button) {
            case BUTTON_MENU:
                if (isEdit) {
                    isEdit = false;
                    Config::read();
                } else {
                    Render::screen = MENU_SCREEN;
                }
                break;
            case BUTTON_SELECT:
                if (isEdit) {
                    isEdit = false;
                    Config::write();
                } else {
                    isEdit = true;
                }
                break;
            case BUTTON_MINUS:
                if (isEdit) {
                    if (Config::pageSetting[editItem])
                        Config::pageSetting[editItem]--;
                } else {
                    if (!editItem)
                        editItem = MAIN_PAGE_COUNT - 1;
                    else
                        editItem--;
                }
                break;
            case BUTTON_PLUS:
                if (isEdit) {
                    if (Config::pageSetting[editItem] <= 240)
                        Config::pageSetting[editItem]++;
                } else {
                    if (editItem >= MAIN_PAGE_COUNT - 1)
                        editItem = 0;
                    else
                        editItem++;
                }
                break;
        }
    }
}
