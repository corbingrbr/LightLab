/*--------------------------------------------------------------------
This file is part of the Adafruit NeoPixel library.

NeoPixel is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of
the License, or (at your option) any later version.

NeoPixel is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with NeoPixel. If not, see
<http://www.gnu.org/licenses/>.
--------------------------------------------------------------------*/

#ifndef ADAFRUIT_NEOPIXEL_H
#define ADAFRUIT_NEOPIXEL_H

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
 

// 'type' flags for LED pixels (third parameter to constructor):
#define NEO_GRB 0x01 // Wired for GRB data order
#define NEO_COLMASK 0x01
#define NEO_KHZ800 0x02 // 800 KHz datastream
#define NEO_SPDMASK 0x02

#define interrupts() sei()
#define noInterrupts() cli()


class Adafruit_NeoPixel {

 public:

  // Constructor: number of LEDs
  Adafruit_NeoPixel(uint8_t n);
  ~Adafruit_NeoPixel();

  void
    begin(void),
    show(void),
    setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b),
    adjustPixelColor(uint16_t n, int8_t r, int8_t g, int8_t b),
    setBrightness(uint8_t b),
    delay_us( uint16_t us);
  
  uint8_t *
    getPixels(void);

  uint32_t
    getPixelColor(uint8_t n);

 private:
  
 uint8_t *pixels; // Holds LED color values (3 bytes each)
 
  uint8_t
    numLEDs,
    numBytes,
    pin,
    brightness;

  uint32_t
    endTime; // Latch timing reference
#ifdef __AVR__
  const volatile uint8_t
    *port; // Output PORT register
  uint8_t
    pinMask; // Output PORT bitmask
#endif

};

#endif // ADAFRUIT_NEOPIXEL_H

