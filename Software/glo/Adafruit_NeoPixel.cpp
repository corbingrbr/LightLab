/*-------------------------------------------------------------------------
Arduino library to control a wide variety of WS2811- and WS2812-based RGB
LED devices such as Adafruit FLORA RGB Smart Pixels and NeoPixel strips.
Currently handles 400 and 800 KHz bitstreams on 8, 12 and 16 MHz ATmega
MCUs, with LEDs wired for RGB or GRB color order. 8 MHz MCUs provide
output on PORTB and PORTD, while 16 MHz chips can handle most output pins
(possible exception with upper PORT registers on the Arduino Mega).

Written by Phil Burgess / Paint Your Dragon for Adafruit Industries,
contributions by PJRC and other members of the open source community.

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing products
from Adafruit!

-------------------------------------------------------------------------
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
-------------------------------------------------------------------------*/

#include "Adafruit_NeoPixel.h"
#include <avr/interrupt.h>
#include <stdlib.h>
#include <avr/io.h>

Adafruit_NeoPixel::Adafruit_NeoPixel(uint8_t n) : port(&PORTD), pinMask(0x04), numLEDs(n), numBytes(n*3){ pixels = (uint8_t *)malloc(numBytes);}

Adafruit_NeoPixel::~Adafruit_NeoPixel() {
  if(pixels) free(pixels);
  DDRD &= ~pinMask;
}

void Adafruit_NeoPixel::begin(void) {
  DDRD |= pinMask;
  PORTD &= ~pinMask;
}

void Adafruit_NeoPixel::delay_us( uint16_t us )
{
  us *= 16;
  
  while( us > 0 ){us--;}
}

void Adafruit_NeoPixel::show(void) {

  if(!pixels) return;

  // Data latch = 50+ microsecond pause in the output stream. Rather than
  // put a delay at the end of the function, the ending time is noted and
  // the function will simply hold off (if needed) on issuing the
  // subsequent round of data until the latch time has elapsed. This
  // allows the mainline code to start generating the next frame of data
  // rather than stalling for the latch.
  delay_us(10);
  // endTime is a private member (rather than global var) so that mutliple
  // instances on different pins can be quickly issued in succession (each
  // instance doesn't delay the next).

  // In order to make this code runtime-configurable to work with any pin,
  // SBI/CBI instructions are eschewed in favor of full PORT writes via the
  // OUT or ST instructions. It relies on two facts: that peripheral
  // functions (such as PWM) take precedence on output pins, so our PORT-
  // wide writes won't interfere, and that interrupts are globally disabled
  // while data is being issued to the LEDs, so no other code will be
  // accessing the PORT. The code takes an initial 'snapshot' of the PORT
  // state, computes 'pin high' and 'pin low' values, and writes these back
  // to the PORT register as needed.

  noInterrupts(); // Need 100% focus on instruction timing

  volatile uint16_t
    i = numBytes; // Loop counter
  volatile uint8_t
   *ptr = pixels, // Pointer to next byte
    b = *ptr++, // Current byte value
    hi, // PORT w/output bit set high
    lo; // PORT w/output bit set low

  // Hand-tuned assembly code issues data to the LED drivers at a specific
  // rate. There's separate code for different CPU speeds (8, 12, 16 MHz)
  // for both the WS2811 (400 KHz) and WS2812 (800 KHz) drivers. The
  // datastream timing for the LED drivers allows a little wiggle room each
  // way (listed in the datasheets), so the conditions for compiling each
  // case are set up for a range of frequencies rather than just the exact
  // 8, 12 or 16 MHz values, permitting use with some close-but-not-spot-on
  // devices (e.g. 16.5 MHz DigiSpark). The ranges were arrived at based
  // on the datasheet figures and have not been extensively tested outside
  // the canonical 8/12/16 MHz speeds; there's no guarantee these will work
  // close to the extremes (or possibly they could be pushed further).
  // Keep in mind only one CPU speed case actually gets compiled; the
  // resulting program isn't as massive as it might look from source here.



// 16 MHz(ish) AVR --------------------------------------------------------

    // WS2811 and WS2812 have different hi/lo duty cycles; this is
    // similar but NOT an exact copy of the prior 400-on-8 code.

    // 20 inst. clocks per bit: HHHHHxxxxxxxxLLLLLLL
    // ST instructions: ^ ^ ^ (T=0,5,13)

    volatile uint8_t next, bit;

    hi = *port | pinMask;
    lo = *port & ~pinMask;
    next = lo;
    bit = 8;

    asm volatile(
     "head20:" "\n\t" // Clk Pseudocode (T = 0)
      "st %a[port], %[hi]" "\n\t" // 2 PORT = hi (T = 2)
      "sbrc %[byte], 7" "\n\t" // 1-2 if(b & 128)
       "mov %[next], %[hi]" "\n\t" // 0-1 next = hi (T = 4)
      "dec %[bit]" "\n\t" // 1 bit-- (T = 5)
      "st %a[port], %[next]" "\n\t" // 2 PORT = next (T = 7)
      "mov %[next] , %[lo]" "\n\t" // 1 next = lo (T = 8)
      "breq nextbyte20" "\n\t" // 1-2 if(bit == 0) (from dec above)
      "rol %[byte]" "\n\t" // 1 b <<= 1 (T = 10)
      "rjmp .+0" "\n\t" // 2 nop nop (T = 12)
      "nop" "\n\t" // 1 nop (T = 13)
      "st %a[port], %[lo]" "\n\t" // 2 PORT = lo (T = 15)
      "nop" "\n\t" // 1 nop (T = 16)
      "rjmp .+0" "\n\t" // 2 nop nop (T = 18)
      "rjmp head20" "\n\t" // 2 -> head20 (next bit out)
     "nextbyte20:" "\n\t" // (T = 10)
      "ldi %[bit] , 8" "\n\t" // 1 bit = 8 (T = 11)
      "ld %[byte] , %a[ptr]+" "\n\t" // 2 b = *ptr++ (T = 13)
      "st %a[port], %[lo]" "\n\t" // 2 PORT = lo (T = 15)
      "nop" "\n\t" // 1 nop (T = 16)
      "sbiw %[count], 1" "\n\t" // 2 i-- (T = 18)
       "brne head20" "\n" // 2 if(i != 0) -> (next byte)
      : [port] "+e" (port),
        [byte] "+r" (b),
        [bit] "+r" (bit),
        [next] "+r" (next),
        [count] "+w" (i)
      : [ptr] "e" (ptr),
        [hi] "r" (hi),
        [lo] "r" (lo));


  interrupts();
  // endTime = micros(); // Save EOD time for latch on next call
}

/*
unsigned long Adafruit_NeoPixel::micros() {
         unsigned long m;
         uint8_t oldSREG = SREG, t;
        
         cli();  
         m = timer0_overflow_count;
         t = TCNT0;
  
         if ((TIFR0 & _BV(TOV0)) && (t < 255))
                 m++;
 

         SREG = oldSREG;
        
         return ((m << 8) + t) * (64 / 16);
} 
*/


// Set pixel color from separate R,G,B components:
void Adafruit_NeoPixel::setPixelColor(
 uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
  if(n < numLEDs) {
    if(brightness) { // See notes in setBrightness()
      r = (r * brightness) >> 8;
      g = (g * brightness) >> 8;
      b = (b * brightness) >> 8;
    }
    uint8_t *p = &pixels[n * 3];
#ifdef NEO_RGB
    if((type & NEO_COLMASK) == NEO_GRB) {
#endif
      *p++ = g;
      *p++ = r;
#ifdef NEO_RGB
    } else {
      *p++ = r;
      *p++ = g;
    }
#endif
    *p = b;
  }
}


void Adafruit_NeoPixel::adjustPixelColor(uint16_t n, int8_t r, int8_t g, int8_t b)
{
  uint16_t ofs = n * 3;
  
  if ( pixels[ofs + 1] + r > 0 && pixels[ofs + 1] + r < 255) {pixels[ofs + 1] += r;}
  
  if ( pixels[ofs] + g > 0  && pixels[ofs] + g < 255) {pixels[ofs] += g;}
  
  if ( pixels[ofs + 2] + b > 0 && pixels[ofs + 2] + b < 255) {pixels[ofs +2] += b;}
}

// Query color from previously-set pixel (returns packed 32-bit RGB value)
uint32_t Adafruit_NeoPixel::getPixelColor(uint8_t n) {

  if(n < numLEDs) {
    uint16_t ofs = n * 3;
    return (uint32_t)(pixels[ofs + 2]) | ((uint32_t)(pixels[ofs ]) << 8) | ((uint32_t)(pixels[ofs + 1]) << 16);
  }

  return 0; // Pixel # is out of bounds
}

uint8_t *Adafruit_NeoPixel::getPixels(void) {
  return pixels;
}

// Adjust output brightness; 0=darkest (off), 255=brightest. This does
// NOT immediately affect what's currently displayed on the LEDs. The
// next call to show() will refresh the LEDs at this level. However,
// this process is potentially "lossy," especially when increasing
// brightness. The tight timing in the WS2811/WS2812 code means there
// aren't enough free cycles to perform this scaling on the fly as data
// is issued. So we make a pass through the existing color data in RAM
// and scale it (subsequent graphics commands also work at this
// brightness level). If there's a significant step up in brightness,
// the limited number of steps (quantization) in the old data will be
// quite visible in the re-scaled version. For a non-destructive
// change, you'll need to re-render the full strip data. C'est la vie.
void Adafruit_NeoPixel::setBrightness(uint8_t b) {
  // Stored brightness value is different than what's passed.
  // This simplifies the actual scaling math later, allowing a fast
  // 8x8-bit multiply and taking the MSB. 'brightness' is a uint8_t,
  // adding 1 here may (intentionally) roll over...so 0 = max brightness
  // (color values are interpreted literally; no scaling), 1 = min
  // brightness (off), 255 = just below max brightness.
  
  uint8_t newBrightness = b + 1;
  if(newBrightness != brightness) { // Compare against prior value
    // Brightness has changed -- re-scale existing data in RAM
    uint8_t c,
            *ptr = pixels,
             oldBrightness = brightness - 1; // De-wrap old brightness value
    uint16_t scale;
    if(oldBrightness == 0) scale = 0; // Avoid /0
    else if(b == 255) scale = 65535 / oldBrightness;
    else scale = (((uint16_t)newBrightness << 8) - 1) / oldBrightness;
    for(uint16_t i=0; i<numBytes; i++) {
      c = *ptr;
      *ptr++ = (c * scale) >> 8;
    }
  
    brightness = newBrightness;

    }
}
