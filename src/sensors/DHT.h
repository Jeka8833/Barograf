/*!
 *  @file DHT.h
 *
 *  This is a library for DHT series of low cost temperature/humidity sensors.
 *
 *  You must have Adafruit Unified Sensor Library library installed to use this
 * class.
 *
 *  Adafruit invests time and resources providing this open source code,
 *  please support Adafruit andopen-source hardware by purchasing products
 *  from Adafruit!
 *
 *  Written by Adafruit Industries.
 *
 *  MIT license, all text above must be included in any redistribution
 */

#ifndef DHT_H
#define DHT_H

#include "Arduino.h"

/* Uncomment to enable printing out nice debug messages. */
//#define DHT_DEBUG

#define DEBUG_PRINTER                                                          \
  Serial /**< Define where debug output will be printed.                       \
          */

/* Setup debug printing macros. */
#ifdef DHT_DEBUG
#define DEBUG_PRINT(...)                                                       \
  { DEBUG_PRINTER.print(__VA_ARGS__); }
#define DEBUG_PRINTLN(...)                                                     \
  { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
#define DEBUG_PRINT(...)                                                       \
  {} /**< Debug Print Placeholder if Debug is disabled */
#define DEBUG_PRINTLN(...)                                                     \
  {} /**< Debug Print Line Placeholder if Debug is disabled */
#endif

#if defined(TARGET_NAME) && (TARGET_NAME == ARDUINO_NANO33BLE)
#ifndef microsecondsToClockCycles
/*!
 * As of 7 Sep 2020 the Arduino Nano 33 BLE boards do not have
 * microsecondsToClockCycles defined.
 */
#define microsecondsToClockCycles(a) ((a) * (SystemCoreClock / 1000000L))
#endif
#endif

/*!
 *  @brief  Class that stores state and functions for DHT
 */
class DHT {
public:
    DHT(uint8_t pin, uint8_t count = 6);

    void begin() const;

    float readTemperature();

    static float constexpr convertCtoF(float);

    static float constexpr convertFtoC(float);

    float computeHeatIndex(float temperature, uint8_t percentHumidity);

    uint8_t readHumidity();

    void read();

private:
    uint8_t data[5];
    uint8_t _pin;
#ifdef __AVR
    // Use direct GPIO access on an 8-bit AVR so keep track of the port and
    // bitmask for the digital pin connected to the DHT.  Other platforms will use
    // digitalRead.
    uint8_t _bit, _port;
#endif
    uint32_t _maxcycles;

    uint32_t expectPulse(bool level);
};

/*!
 *  @brief  Class that defines Interrupt Lock Avaiability
 */
class InterruptLock {
public:
    InterruptLock() {
#if !defined(ARDUINO_ARCH_NRF52)
        noInterrupts();
#endif
    }

    ~InterruptLock() {
#if !defined(ARDUINO_ARCH_NRF52)
        interrupts();
#endif
    }
};

#endif