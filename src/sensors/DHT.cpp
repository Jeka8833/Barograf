/*!
 *  @file DHT.cpp
 *
 *  @mainpage DHT series of low cost temperature/humidity sensors.
 *
 *  @section intro_sec Introduction
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
 *  @section author Author
 *
 *  Written by Adafruit Industries.
 *
 *  @section license License
 *
 *  MIT license, all text above must be included in any redistribution
 */

#include "DHT.h"

#define TIMEOUT                                                                \
  UINT32_MAX /**< Used programmatically for timeout.                           \
                   Not a timeout duration. Type: uint32_t. */

/*!
 *  @brief  Instantiates a new DHT class
 *  @param  pin
 *          pin number that sensor is connected
 *  @param  type
 *          type of sensor
 *  @param  count
 *          number of sensors
 */
DHT::DHT(uint8_t pin, uint8_t count) {
    (void) count; // Workaround to avoid compiler warning.
    _pin = pin;
#ifdef __AVR
    _bit = digitalPinToBitMask(pin);
    _port = digitalPinToPort(pin);
#endif
    _maxcycles =
            microsecondsToClockCycles(1000); // 1 millisecond timeout for
    // reading pulses from DHT sensor.
    // Note that count is now ignored as the DHT reading algorithm adjusts itself
    // based on the speed of the processor.
}

/*!
 *  @brief  Setup sensor pins and set pull timings
 *  @param  usec
 *          Optionally pass pull-up time (in microseconds) before DHT reading
 *starts. Default is 55 (see function declaration in DHT.h).
 */
void DHT::begin() const {
    pinMode(_pin, INPUT_PULLUP);
}

/*!
 *  @brief  Read temperature
 *  @param  S
 *          Scale. Boolean value:
 *					- true = Fahrenheit
 *					- false = Celcius
 *  @param  force
 *          true if in force mode
 *	@return Temperature value in selected scale
 */
float DHT::readTemperature() {
    float f = data[2];
    if (data[3] & 0x80) {
        f = -1 - f;
    }
    f += (data[3] & 0x0f) * 0.1f;
    return f;
}

/*!
 *  @brief  Converts Celcius to Fahrenheit
 *  @param  c
 *					value in Celcius
 *	@return float value in Fahrenheit
 */
constexpr float DHT::convertCtoF(float c) { return c * 1.8f + 32; }

/*!
 *  @brief  Converts Fahrenheit to Celcius
 *  @param  f
 *					value in Fahrenheit
 *	@return float value in Celcius
 */
constexpr float DHT::convertFtoC(float f) { return (f - 32) * 0.55555f; }

/*!
 *  @brief  Read Humidity
 *  @param  force
 *					force read mode
 *	@return float value - humidity in percent
 */
uint8_t DHT::readHumidity() {
    return (data[0] + data[1] * 0.1f);
}

/*!
 *  @brief  Compute Heat Index
 *  				Using both Rothfusz and Steadman's equations
 *					(http://www.wpc.ncep.noaa.gov/html/heatindex_equation.shtml)
 *  @param  temperature
 *          temperature in selected scale
 *  @param  percentHumidity
 *          humidity in percent
 *  @param  isFahrenheit
 * 					true if fahrenheit, false if celcius
 *	@return float heat index
 */
float DHT::computeHeatIndex(float temperature, uint8_t percentHumidity) {
    temperature = convertCtoF(temperature);

    float hi = 0.5 * (temperature + 61.0 + ((temperature - 68.0) * 1.2) +
                      (percentHumidity * 0.094));

    if (hi > 79) {
        hi = -42.379 + 2.04901523 * temperature + 10.14333127 * percentHumidity +
             -0.22475541 * temperature * percentHumidity +
             -0.00683783 * temperature * temperature +
             -0.05481717 * percentHumidity * percentHumidity +
             0.00122874 * temperature * temperature * percentHumidity +
             0.00085282 * temperature * percentHumidity * percentHumidity +
             -0.00000199 * temperature * temperature * percentHumidity * percentHumidity;

        if ((percentHumidity < 13) && (temperature >= 80.0) &&
            (temperature <= 112.0))
            hi -= ((13.0 - percentHumidity) * 0.25) *
                  sqrt((17.0 - abs(temperature - 95.0)) * 0.05882);

        else if ((percentHumidity > 85.0) && (temperature >= 80.0) &&
                 (temperature <= 87.0))
            hi += ((percentHumidity - 85.0) * 0.1) * ((87.0 - temperature) * 0.2);
    }

    return convertFtoC(hi);
}

/*!
 *  @brief  Read value from sensor or return last one from less than two
 *seconds.
 *  @param  force
 *          true if using force mode
 *	@return float value
 */
void DHT::read() {
#if defined(ESP8266)
    yield(); // Handle WiFi / reset software watchdog
#endif
/*
    // Send start signal.  See DHT datasheet for full signal diagram:
    //   http://www.adafruit.com/datasheets/Digital%20humidity%20and%20temperature%20sensor%20AM2302.pdf

    // Go into high impedence state to let pull-up raise data line level and
    // start the reading process.
    pinMode(_pin, INPUT_PULLUP);
    delay(1);

    // First set data line low for a period according to sensor type
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);

    delay(18); // data sheet says at least 18ms, 20ms just to be safe
    */
    uint32_t cycles[80];
/*
    // End the start signal by setting data line high for 40 microseconds.
    pinMode(_pin, INPUT_PULLUP);

    // Delay a moment to let sensor pull data line low.
    delayMicroseconds(55);
*/
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);            // 1.
    delay(20);                         // specs [2]: 18us

    // Pull high and set to input, before wait 40us.
    // @see https://github.com/winlinvip/SimpleDHT/issues/4
    // @see https://github.com/winlinvip/SimpleDHT/pull/5
    digitalWrite(_pin, HIGH);           // 2.
    pinMode(_pin, INPUT);
    delayMicroseconds(40);             // specs [2]: 20-40us

    // Now start reading the data line to get the value from the DHT sensor.
    {
        //noInterrupts();
        // First expect a low signal for ~80 microseconds followed by a high signal
        // for ~80 microseconds again.
        if (expectPulse(LOW) == TIMEOUT) {
            return;
        }
        if (expectPulse(HIGH) == TIMEOUT) {
            return;
        }

        // Reset 40 bits of received data to zero.
        data[0] = data[1] = data[2] = data[3] = data[4] = 0;

        // Now read the 40 bits sent by the sensor.  Each bit is sent as a 50
        // microsecond low pulse followed by a variable length high pulse.  If the
        // high pulse is ~28 microseconds then it's a 0 and if it's ~70 microseconds
        // then it's a 1.  We measure the cycle count of the initial 50us low pulse
        // and use that to compare to the cycle count of the high pulse to determine
        // if the bit is a 0 (high state cycle count < low state cycle count), or a
        // 1 (high state cycle count > low state cycle count). Note that for speed
        // all the pulses are read into a array and then examined in a later step.
        for (int i = 0; i < 80; i += 2) {
            cycles[i] = expectPulse(LOW);
            cycles[i + 1] = expectPulse(HIGH);
        }
        //interrupts();
    } // Timing critical code is now complete.

    // Inspect pulses and determine which ones are 0 (high state cycle count < low
    // state cycle count), or 1 (high state cycle count > low state cycle count).
    for (int i = 0; i < 40; ++i) {
        uint32_t lowCycles = cycles[2 * i];
        uint32_t highCycles = cycles[2 * i + 1];
        if ((lowCycles == TIMEOUT) || (highCycles == TIMEOUT)) {
            return;
        }
        data[i / 8] <<= 1;
        // Now compare the low and high cycle times to see if the bit is a 0 or 1.
        if (highCycles > lowCycles) {
            // High cycles are greater than 50us low cycle count, must be a 1.
            data[i / 8] |= 1;
        }
        // Else high cycles are less than (or equal to, a weird case) the 50us low
        // cycle count so this must be a zero.  Nothing needs to be changed in the
        // stored data.
    }
/*
    // Check we read 40 bits and that the checksum matches.
    if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
        _lastresult = true;
        return _lastresult;
    } else {
        _lastresult = false;
        return _lastresult;
    }*/
}

// Expect the signal line to be at the specified level for a period of time and
// return a count of loop cycles spent at that level (this cycle count can be
// used to compare the relative time of two pulses).  If more than a millisecond
// ellapses without the level changing then the call fails with a 0 response.
// This is adapted from Arduino's pulseInLong function (which is only available
// in the very latest IDE versions):
//   https://github.com/arduino/Arduino/blob/master/hardware/arduino/avr/cores/arduino/wiring_pulse.c
uint32_t DHT::expectPulse(bool level) {
#if (F_CPU > 16000000L)
    uint32_t count = 0;
#else
    uint16_t count = 0; // To work fast enough on slower AVR boards
#endif
// On AVR platforms use direct GPIO port access as it's much faster and better
// for catching pulses that are 10's of microseconds in length:
#ifdef __AVR
    uint8_t portState = level ? _bit : 0;
    while ((*portInputRegister(_port) & _bit) == portState) {
        if (count++ >= _maxcycles) {
            return TIMEOUT; // Exceeded timeout, fail.
        }
    }
// Otherwise fall back to using digitalRead (this seems to be necessary on
// ESP8266 right now, perhaps bugs in direct port access functions?).
#else
    while (digitalRead(_pin) == level) {
    if (count++ >= _maxcycles) {
      return TIMEOUT; // Exceeded timeout, fail.
    }
  }
#endif

    return count;
}