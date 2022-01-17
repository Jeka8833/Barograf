//
// Created by Jeka8833 on 10.01.2022.
//

#include "MenuScreen.h"
#include "Render/Render.h"
#include "Render/Control.h"
#include "Config.h"

#define ABOUT_ITEM 0
#define LIGHT_ITEM 1
#define PAGES_ITEM 2
#define BEEPER_ITEM 3

uint8_t MenuScreen::editItem;

void MenuScreen::render() {
    Render::u8g2.setFont(u8g2_font_9x15_t_cyrillic);
    Render::u8g2.setDrawColor(2);

    Render::u8g2.drawUTF8((DISPLAY_WIDTH - Render::u8g2.getUTF8Width("Меню")) >> 1, 10, "Меню");
    Render::u8g2.drawHLine(0, 11, DISPLAY_WIDTH);

    Render::u8g2.drawUTF8(0, 22, "О устройстве");
    Render::u8g2.drawUTF8(0, 35, "Яркость");
    Render::u8g2.drawUTF8(0, 48, "Время страницы");
    Render::u8g2.drawUTF8(0, 61, Config::beeper ? "Звук: вкл." : "Звук: выкл.");

    if (Render::secondAnimation) {
        switch (editItem) {
            case ABOUT_ITEM:
                Render::u8g2.drawBox(0, 12, DISPLAY_WIDTH, 13);
                break;
            case LIGHT_ITEM:
                Render::u8g2.drawBox(0, 25, DISPLAY_WIDTH, 13);
                break;
            case PAGES_ITEM:
                Render::u8g2.drawBox(0, 38, DISPLAY_WIDTH, 13);
                break;
            case BEEPER_ITEM:
                Render::u8g2.drawBox(0, 51, DISPLAY_WIDTH, 13);
                break;
        }
    }
}

void MenuScreen::buttonChangeState(uint8_t button, uint8_t state) {
    if (state) {
        switch (button) {
            case BUTTON_MENU:
                Render::screen = INFO_SCREEN;
                break;
            case BUTTON_SELECT:
                switch (editItem) {
                    case LIGHT_ITEM:
                        Render::screen = LIGHT_SCREEN;
                        break;
                    case ABOUT_ITEM:
                        Render::screen = ABOUT_SCREEN;
                        break;
                    case PAGES_ITEM:
                        Render::screen = PAGE_SETTING_SCREEN;
                        break;
                    case BEEPER_ITEM:
                        Config::beeper = !Config::beeper;
                        Config::write();
                        break;
                }
                break;
            case BUTTON_MINUS:
                if (!editItem)
                    editItem = BEEPER_ITEM;
                else
                    editItem--;
                break;
            case BUTTON_PLUS:
                if (editItem >= BEEPER_ITEM)
                    editItem = 0;
                else
                    editItem++;
                break;
        }
    }
}
