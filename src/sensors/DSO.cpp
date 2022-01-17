//
// Created by Jeka8833 on 19.10.2020.
//

#include "DSO.h"
#include <Wire.h>

uint8_t DSO::Minute = 0;
uint8_t DSO::Hour = 0;
uint8_t DSO::wDay = 0;
uint8_t DSO::Day = 0;
uint8_t DSO::Month = 0;
uint16_t DSO::Year = 0;

void DSO::read() {
    Wire.beginTransmission(0x68);
    Wire.write(0x01);
    Wire.endTransmission();
    Wire.requestFrom(0x68, 6);
    Minute = bcd2dec(Wire.read());
    Hour = bcd2dec(Wire.read() & ~_BV(6));
    wDay = bcd2dec(Wire.read());
    Day = bcd2dec(Wire.read());
    Month = bcd2dec(Wire.read());
    Year = bcd2dec(Wire.read()) + 2000;
}

void DSO::write() {
    Wire.beginTransmission(0x68);
    Wire.write(0x01);
    Wire.write(decToBcd(Minute));
    Wire.write(decToBcd(Hour) & 0b10111111);
    Wire.write(decToBcd(wDay));
    Wire.write(decToBcd(Day));
    Wire.write(decToBcd(Month));
    Wire.write(decToBcd(Year - 2000));
    Wire.endTransmission();
}

constexpr uint8_t DSO::bcd2dec(uint8_t n) {
    return n - 6 * (n >> 4);
}

constexpr uint8_t DSO::decToBcd(uint8_t n) {
    return n + 6 * (n / 10);
}
