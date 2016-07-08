#ifndef GLOW_MANAGER_H_
#define GLOW_MANAGER_H_

#include "Adafruit_NeoPixel.h"


class Glow_Manager { 
  
  public:
  
  /*                          GLOWSTICK FUNCTIONS                                 */

  //////////////////////////////////////////////////////////////////////////////////
  
  Glow_Manager( uint8_t n, uint32_t p, uint8_t b );

  void initialize( );
  void show( );
  void delay_ms( int ms );
  
  void fade( );
  void fade_adjust( char adjustments );  
  void update_pixels( );
  void update( );
  
  
  void next_pattern( );
  void next_section( );
  void reset_pattern( );
  void reset_fades( );
  void set_brightness( uint8_t brightness );

  void pixel_update( );                                                           
                                         
  void pixel_remove_fade( );                       
                                                                       
  void pixel_update_color( );
  void pixel_set_color( );       

  void pixel_update_fade( );
  void pixel_update_fade_delay( );
  void pixel_get_color( int16_t *r, int16_t *g, int16_t *b, char data );
  void solid_color( char data );
  void find_starting_vals( uint16_t *f_Delay, int16_t *c_Adjust);
  void pixel_update_fade_info( int16_t delta_color, uint8_t index);
///////////////////////////////////////////////////////////////////////////////////
  
  uint32_t patternAddr,
           curAddr,
           fadeCounter;

  uint16_t counter,
           fadeDelays[8][3],
           universalDelay;
  
  uint8_t numLEDs,
          curLED,
          brightness;

  int8_t colorAdjust[8][3];
 
  Adafruit_NeoPixel neopixel;
};


#endif
