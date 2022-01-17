//
// Created by Jeka8833 on 04.01.2022.
//

#include "Render.h"
#include "Setting.h"
#include "Control.h"
#include "Config.h"
#include "Render/Screens/PressureScreen.h"
#include "Render/Screens/TimeScreen.h"
#include "Render/Screens/InfoScreen.h"
#include "Render/Screens/LightScreen.h"
#include "Render/Screens/MenuScreen.h"
#include "Render/Screens/AboutScreen.h"
#include "Render/Screens/PageSettingScreen.h"

bool Render::secondAnimation;
bool Render::lastSecondAnimation;
bool Render::needUpdate = true;
uint8_t Render::screen;
uint8_t Render::scrollTime = 30;
U8G2_ST7920_128X64_1_HW_SPI Render::u8g2(U8G2_R0, /*IN_DISPLAY_CLOCK, PIN_DISPLAY_DATA,*/ PIN_DISPLAY_CS,
                                         PIN_DISPLAY_RESET);

void Render::init() {
    pinMode(PIN_LED_PWM, OUTPUT);

    // u8g2.setBusClock(1000000);
    u8g2.begin();
    u8g2.enableUTF8Print();
}

void Render::update() {
    secondAnimation = (millis() % 1000) > 500;
    if (lastSecondAnimation != secondAnimation) {
        lastSecondAnimation = secondAnimation;
        if (secondAnimation) {
            if (screen <= PRESSURE_SCREEN) {
                if (Config::pageSetting[screen]) {
                    if(scrollTime == 0){
                        screen++;
                        if(screen > PRESSURE_SCREEN)
                            screen = 0;
                        scrollTime = Config::pageSetting[screen];
                        Render::needUpdate = true;
                    } else {
                        scrollTime--;
                    }
                }
            }
        }
        switch (screen) {
            case TIME_SCREEN:
            case MENU_SCREEN:
            case PAGE_SETTING_SCREEN:
                Render::needUpdate = true;
        }
    }

    if (Render::screen == LIGHT_SCREEN)
        Render::needUpdate = true;

    if (needUpdate) {
        u8g2.firstPage();
        do {
            switch (screen) {
                case MENU_SCREEN:
                    MenuScreen::render();
                    break;
                case LIGHT_SCREEN:
                    LightScreen::render();
                    break;
                case ABOUT_SCREEN:
                    AboutScreen::render();
                    break;
                case INFO_SCREEN:
                    InfoScreen::render();
                    break;
                case TIME_SCREEN:
                    TimeScreen::render();
                    break;
                case PRESSURE_SCREEN:
                    PressureScreen::render();
                    break;
                case PAGE_SETTING_SCREEN:
                    PageSettingScreen::render();
                    break;
            }
        } while (u8g2.nextPage());
        needUpdate = false;
    }

}

void Render::setBrightness(uint8_t value) {
    analogWrite(PIN_LED_PWM, value);
}

void Render::buttonAction(uint8_t button, uint8_t state) {
    if (button <= BUTTON_MINUS && state) {
        if(Config::pageSetting[screen] && screen <= PRESSURE_SCREEN){
            scrollTime = 30;
        }
        needUpdate = true;
        if (Config::beeper)
            tone(PIN_BUZZER, 1000, 150);
    }

    switch (screen) {
        case INFO_SCREEN:
            if (state) {
                switch (button) {
                    case BUTTON_MENU:
                        screen = MENU_SCREEN;
                        break;
                    case BUTTON_MINUS:
                        screen = PRESSURE_SCREEN;
                        break;
                    case BUTTON_PLUS:
                        screen = TIME_SCREEN;
                        break;
                }
            }
            break;
        case TIME_SCREEN:
            TimeScreen::buttonChangeState(button, state);
            break;
        case PRESSURE_SCREEN:
            if (state) {
                switch (button) {
                    case BUTTON_MENU:
                        screen = MENU_SCREEN;
                        break;
                    case BUTTON_MINUS:
                        screen = TIME_SCREEN;
                        break;
                    case BUTTON_PLUS:
                        screen = INFO_SCREEN;
                        break;
                }
            }
            break;
        case LIGHT_SCREEN:
            LightScreen::buttonChangeState(button, state);
            break;
        case MENU_SCREEN:
            MenuScreen::buttonChangeState(button, state);
            break;
        case ABOUT_SCREEN:
            if (state && button != BUTTON_POTENTIOMETER)
                screen = MENU_SCREEN;
            break;
        case PAGE_SETTING_SCREEN:
            PageSettingScreen::buttonChangeState(button, state);
            break;
    }
}