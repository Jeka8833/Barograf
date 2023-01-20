//
// Created by Jeka8833 on 06.01.2022.
//

#include "TimeScreen.h"
#include "Render/Render.h"
#include "Render/Control.h"
#include "sensors/DSO.h"
#include "Render/MyFonts.h"

#define EDIT_HOUR 4
#define EDIT_MINUTE 5
#define EDIT_DAY 3
#define EDIT_MOUNT 2
#define EDIT_YEAR 1
#define EDIT_WDAY 6

uint8_t TimeScreen::editItem;

void TimeScreen::render() {
    Render::u8g2.setFont(u8g2_font_8x13_t_cyrillic);

    if (!(editItem == EDIT_WDAY && Render::secondAnimation)) {
        switch (DSO::wDay) {
            case 1:
                Render::u8g2.drawUTF8((DISPLAY_WIDTH - Render::u8g2.getUTF8Width("Понедельник")) >> 1, 15, "Понедельник");
                break;
            case 2:
                Render::u8g2.drawUTF8((DISPLAY_WIDTH - Render::u8g2.getUTF8Width("Вторник")) >> 1, 15, "Вторник");
                break;
            case 3:
                Render::u8g2.drawUTF8((DISPLAY_WIDTH - Render::u8g2.getUTF8Width("Среда")) >> 1, 15, "Среда");
                break;
            case 4:
                Render::u8g2.drawUTF8((DISPLAY_WIDTH - Render::u8g2.getUTF8Width("Четверг")) >> 1, 15, "Четверг");
                break;
            case 5:
                Render::u8g2.drawUTF8((DISPLAY_WIDTH - Render::u8g2.getUTF8Width("Пятница")) >> 1, 15, "Пятница");
                break;
            case 6:
                Render::u8g2.drawUTF8((DISPLAY_WIDTH - Render::u8g2.getUTF8Width("Суббота")) >> 1, 15, "Суббота");
                break;
            default:
                Render::u8g2.drawUTF8((DISPLAY_WIDTH - Render::u8g2.getUTF8Width("Воскресенье")) >> 1, 15, "Воскресенье");
        }
    }
    char dateText[11] = "00.00.0000";
    if (!(editItem == EDIT_DAY && Render::secondAnimation)) {
        dateText[0] += DSO::Day / 10;
        dateText[1] += DSO::Day % 10;
    } else {
        dateText[0] = '-';
        dateText[1] = '-';
    }
    if (!(editItem == EDIT_MOUNT && Render::secondAnimation)) {
        dateText[3] += DSO::Month / 10;
        dateText[4] += DSO::Month % 10;
    } else {
        dateText[3] = '-';
        dateText[4] = '-';
    }
    if (!(editItem == EDIT_YEAR && Render::secondAnimation)) {
        dateText[6] += DSO::Year / 1000;
        dateText[7] += (DSO::Year / 100) % 10;
        dateText[8] += (DSO::Year / 10) % 10;
        dateText[9] += DSO::Year % 10;
    } else {
        dateText[6] = '-';
        dateText[7] = '-';
        dateText[8] = '-';
        dateText[9] = '-';
    }
    Render::u8g2.drawStr((128 - Render::u8g2.getStrWidth(dateText)) >> 1, 60, dateText);

    char timeText[6] = "00:00";
    if (!(editItem == EDIT_HOUR && Render::secondAnimation)) {
        timeText[0] += DSO::Hour / 10;
        timeText[1] += DSO::Hour % 10;
    } else {
        timeText[0] = '-';
        timeText[1] = '-';
    }
    if (!(editItem == EDIT_MINUTE && Render::secondAnimation)) {
        timeText[3] += DSO::Minute / 10;
        timeText[4] += DSO::Minute % 10;
    } else {
        timeText[3] = '-';
        timeText[4] = '-';
    }
    Render::u8g2.setFont(MyFonts::osb_custom);
    Render::u8g2.drawStr((DISPLAY_WIDTH - Render::u8g2.getStrWidth(timeText)) >> 1, 43, timeText);
}

void TimeScreen::buttonChangeState(uint8_t button, uint8_t state) {
    if (state) {
        switch (button) {
            case BUTTON_MENU:
                if (editItem)
                    editItem = 0;
                else
                    Render::screen = MENU_SCREEN;
                break;
            case BUTTON_SELECT:
                editItem++;
                if (editItem > EDIT_WDAY) {
                    DSO::write();
                    editItem = 0;
                }
                break;
            case BUTTON_MINUS:
                switch (editItem) {
                    case EDIT_HOUR:
                        if (DSO::Hour == 0)
                            DSO::Hour = 23;
                        else
                            DSO::Hour--;
                        break;
                    case EDIT_MINUTE:
                        if (DSO::Minute == 0)
                            DSO::Minute = 59;
                        else
                            DSO::Minute--;
                        break;
                    case EDIT_DAY:
                        if (DSO::Day == 1)
                            DSO::Day = getNumberOfDays(DSO::Month, DSO::Year);
                        else
                            DSO::Day--;
                        break;
                    case EDIT_MOUNT:
                        if (DSO::Month == 1)
                            DSO::Month = 12;
                        else
                            DSO::Month--;
                        break;
                    case EDIT_YEAR:
                        if (DSO::Year > 2022)
                            DSO::Year--;
                        break;
                    case EDIT_WDAY:
                        if (DSO::wDay == 0)
                            DSO::wDay = 6;
                        else
                            DSO::wDay--;
                        break;
                    default:
                        Render::screen = INFO_SCREEN;
                }
                break;
            case BUTTON_PLUS:
                switch (editItem) {
                    case EDIT_HOUR:
                        if (DSO::Hour >= 23)
                            DSO::Hour = 0;
                        else
                            DSO::Hour++;
                        break;
                    case EDIT_MINUTE:
                        if (DSO::Minute >= 59)
                            DSO::Minute = 0;
                        else
                            DSO::Minute++;
                        break;
                    case EDIT_DAY:
                        if (DSO::Day >= getNumberOfDays(DSO::Month, DSO::Year))
                            DSO::Day = 1;
                        else
                            DSO::Day++;
                        break;
                    case EDIT_MOUNT:
                        if (DSO::Month >= 12)
                            DSO::Month = 1;
                        else
                            DSO::Month++;
                        break;
                    case EDIT_YEAR:
                        if (DSO::Year <= 2100)
                            DSO::Year++;
                        break;
                    case EDIT_WDAY:
                        if (DSO::wDay >= 6)
                            DSO::wDay = 0;
                        else
                            DSO::wDay++;
                        break;
                    default:
                        Render::screen = PRESSURE_SCREEN;
                }
                break;
            case BUTTON_POTENTIOMETER:
                break;
        }
        Render::needUpdate = true;
    }
}

uint8_t TimeScreen::getNumberOfDays(uint8_t month, uint16_t year) {
    //leap year condition, if month is 2
    if (month == 2) {
        if ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0))
            return 29;
        else
            return 28;
    }
        //months which has 31 days
    else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8
             || month == 10 || month == 12)
        return 31;
    else
        return 30;
}
