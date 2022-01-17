//
// Created by Jeka8833 on 09.01.2022.
//

#include "Config.h"

uint8_t EEMEM beeperPointer;
uint8_t EEMEM lightParamPointer[LIGHT_COUNT_COLUMN];
uint8_t EEMEM pageSettingPointer[MAIN_PAGE_COUNT];

uint8_t Config::beeper = 1;
uint8_t Config::lightParam[LIGHT_COUNT_COLUMN];
uint8_t Config::pageSetting[MAIN_PAGE_COUNT];

void Config::read() {
    beeper = eeprom_read_byte(&beeperPointer);
    eeprom_read_block(lightParam, &lightParamPointer, sizeof(lightParam));
    eeprom_read_block(pageSetting, &pageSettingPointer, sizeof(pageSetting));
}

void Config::write() {
    eeprom_update_byte(&beeperPointer, beeper);
    eeprom_update_block(lightParam, &lightParamPointer, sizeof(lightParam));
    eeprom_update_block(pageSetting, &pageSettingPointer, sizeof(pageSetting));
}
