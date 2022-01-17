//
// Created by Jeka8833 on 11.01.2022.
//

#include "AboutScreen.h"
#include "Render/Render.h"

void AboutScreen::render() {
    Render::u8g2.setFont(u8g2_font_6x13_t_cyrillic);
    Render::u8g2.drawUTF8((DISPLAY_WIDTH - Render::u8g2.getUTF8Width("Барограф")) >> 1, 9, "Барограф");
    Render::u8g2.drawUTF8(0, 22, "Сделал:");
    Render::u8g2.drawStr(DISPLAY_WIDTH - Render::u8g2.getStrWidth("Jeka8833"), 22, "Jeka8833");
    Render::u8g2.drawUTF8(0, 32, "Дата создания:");
    Render::u8g2.drawUTF8(DISPLAY_WIDTH - Render::u8g2.getUTF8Width("03.03.2016"), 43, "03.03.2016");
    Render::u8g2.drawUTF8(0, 53, "Дата переделки:");
    Render::u8g2.drawUTF8(DISPLAY_WIDTH - Render::u8g2.getUTF8Width("11.01.2022"), 63, "11.01.2022");
}
