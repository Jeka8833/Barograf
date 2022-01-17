#include <Wire.h>
#include "Config.h"
#include "Setting.h"
#include "sensors/BMP.h"
#include "sensors/DSO.h"
#include "Render/Control.h"
#include "Render/Render.h"
#include "Render/Screens/PressureScreen.h"
#include "Render/Screens/TimeScreen.h"
#include "Render/Screens/InfoScreen.h"
#include "Render/Screens/LightScreen.h"

unsigned long previousMillis = 0;
uint16_t timeWritePressure = 1000;

void setup() {
    Wire.begin();

    pinMode(PIN_BUZZER, OUTPUT);
    digitalWrite(PIN_BUZZER, LOW);

    Config::read();
    Control::init();
    Render::init();
    BMP::begin();
    InfoScreen::init();
}

void loop() {
    Control::update();
    Render::update();
    Render::setBrightness(LightScreen::getScreenBrightness());

    unsigned long currentMillis = millis();
    if ((unsigned long)(currentMillis - previousMillis) >= 2000) {
        previousMillis = currentMillis;

        Render::needUpdate = true;
        Control::tick();
        if (Render::screen == INFO_SCREEN)
            InfoScreen::dht.read();

        BMP::update();
        if (!TimeScreen::editItem)
            DSO::read();


        if (timeWritePressure > 900) {
            timeWritePressure = 0;
            PressureScreen::addColumn();
        }
        timeWritePressure++;
    }
}