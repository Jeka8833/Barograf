//
// Created by Jeka8833 on 08.01.2022.
//

#include "Control.h"
#include "Setting.h"
#include "Render.h"
#include "Render/Screens/LightScreen.h"

uint8_t Control::activeButton;
uint8_t Control::potentiometer;
uint8_t Control::lastPotentiometer;
uint8_t Control::lightSensor;

void Control::init() {
    pinMode(PIN_BUTTON_MENU, INPUT);
    pinMode(PIN_BUTTON_SELECT, INPUT);
    pinMode(PIN_BUTTON_PLUS, INPUT);
    pinMode(PIN_BUTTON_MINUS, INPUT);
    pinMode(PIN_POTENTIOMETER, INPUT);
    pinMode(PIN_LIGHT_SENSOR, INPUT);
}

void Control::update() {
    if (digitalRead(PIN_BUTTON_MENU)) {
        if (!(activeButton & BUTTON_MENU)) {
            activeButton |= BUTTON_MENU;
            Render::buttonAction(BUTTON_MENU, HIGH);
        }
    } else if (activeButton & BUTTON_MENU) {
        activeButton &= ~BUTTON_MENU;
        Render::buttonAction(BUTTON_MENU, LOW);
    }

    if (digitalRead(PIN_BUTTON_SELECT)) {
        if (!(activeButton & BUTTON_SELECT)) {
            activeButton |= BUTTON_SELECT;
            Render::buttonAction(BUTTON_SELECT, HIGH);
        }
    } else if (activeButton & BUTTON_SELECT) {
        activeButton &= ~BUTTON_SELECT;
        Render::buttonAction(BUTTON_SELECT, LOW);
    }

    if (digitalRead(PIN_BUTTON_PLUS)) {
        if (!(activeButton & BUTTON_PLUS)) {
            activeButton |= BUTTON_PLUS;
            Render::buttonAction(BUTTON_PLUS, HIGH);
        }
    } else if (activeButton & BUTTON_PLUS) {
        activeButton &= ~BUTTON_PLUS;
        Render::buttonAction(BUTTON_PLUS, LOW);
    }

    if (digitalRead(PIN_BUTTON_MINUS)) {
        if (!(activeButton & BUTTON_MINUS)) {
            activeButton |= BUTTON_MINUS;
            Render::buttonAction(BUTTON_MINUS, HIGH);
        }
    } else if (activeButton & BUTTON_MINUS) {
        activeButton &= ~BUTTON_MINUS;
        Render::buttonAction(BUTTON_MINUS, LOW);
    }

    lightSensor = analogRead(PIN_LIGHT_SENSOR) >> 2;

    potentiometer = analogRead(PIN_POTENTIOMETER) >> 2;
    if (abs((int16_t) potentiometer - lastPotentiometer) > 3) {
        Render::buttonAction(BUTTON_POTENTIOMETER, potentiometer);
    }
}

void Control::tick() {
    if (!LightScreen::editItem)
        lastPotentiometer = potentiometer;
}
