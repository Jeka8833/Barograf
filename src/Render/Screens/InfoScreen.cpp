//
// Created by Jeka8833 on 07.01.2022.
//

#include "InfoScreen.h"
#include "Render/Render.h"
#include "sensors/BMP.h"
#include "Setting.h"

DHT InfoScreen::dht(PIN_DHT_SENSOR);

void InfoScreen::init() {
    dht.begin();
}

void InfoScreen::render() {
    char temperatureText[24] = "Температу:00.0";
    char heatIndexText[24] = "Ощущается:00.0";
    char humidityText[23] = "Влажность:00%";
    char pressureText[23] = "Давление:000.0";

    const int16_t temperature = dht.readTemperature() * 10;

    temperatureText[19] += temperature / 100;
    temperatureText[20] += (temperature / 10) % 10;
    temperatureText[22] += temperature % 10;

    const uint8_t humidity = dht.readHumidity();
    const uint16_t heatIndex = dht.computeHeatIndex(dht.readTemperature(), humidity) * 10;
    heatIndexText[19] += heatIndex / 100;
    heatIndexText[20] += (heatIndex / 10) % 10;
    heatIndexText[22] += heatIndex % 10;

    humidityText[19] += humidity / 10;
    humidityText[20] += humidity % 10;

// (7600 * BMP::pressure) / 101325 => simplify => (303 * BMP::pressure) / 4053
    const uint16_t pressure = (303 * BMP::pressure) / 4053;
    pressureText[17] += pressure / 1000;
    pressureText[18] += (pressure / 100) % 10;
    pressureText[19] += (pressure / 10) % 10;
    pressureText[21] += pressure % 10;

    Render::u8g2.setFont(u8g2_font_9x15_t_cyrillic);
    Render::u8g2.drawUTF8(0, 11, temperatureText);
    Render::u8g2.drawUTF8(0, 24, heatIndexText);
    Render::u8g2.drawUTF8(0, 37, humidityText);
    Render::u8g2.drawUTF8(0, 50, pressureText);
}
