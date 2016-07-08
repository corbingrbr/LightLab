#define F_CPU 16000000UL

#include "glow_manager.h"
#include "mc25aa.h"
#include <util/delay.h>
#include <stdlib.h>
#include "Adafruit_NeoPixel.h"

Glow_Manager::Glow_Manager( uint8_t n, uint32_t p, uint8_t b) : neopixel(n), numLEDs(n), patternAddr(p), curAddr(0), counter(0), fadeCounter(0), brightness(b){};
 
void Glow_Manager::initialize()
{
  neopixel.begin();
  set_brightness(brightness);
}

void Glow_Manager::show( ) 
{
  neopixel.show();
}

void Glow_Manager::delay_ms( int ms )
{
  int i;

  for (i = 0; i < ms; i++) {
    _delay_ms(1);
  }
}

void Glow_Manager::solid_color( char data ) {
  int16_t r, g, b;

  pixel_get_color( &r, &g, &b, data);

  for ( curLED = 0; curLED < numLEDs; curLED++) {
    neopixel.setPixelColor( curLED, r, g, b );              
  } 
  
  show();
}

void Glow_Manager::fade( ) 
{
  int8_t r, g, b;
 
  for (curLED = 0, r=0, g=0, b=0; curLED < numLEDs; curLED++, r=0, b=0, g=0) {
    
    if (counter % fadeDelays[curLED][0] == 0) {     
      r = colorAdjust[curLED][0];
    }
    
    if (counter % fadeDelays[curLED][1] == 0) {     
      g = colorAdjust[curLED][1];
    }
    
    if (counter % fadeDelays[curLED][2] == 0) {     
      b = colorAdjust[curLED][2];
    }
    
    neopixel.adjustPixelColor(curLED, r, g, b); 
  }

  show();
}
  

void Glow_Manager::update_pixels(void)
{
  for ( curLED = 0; curLED < numLEDs; curLED++) {
    pixel_update();
  } 
}

void Glow_Manager::reset_pattern( ) 
{
  curAddr = patternAddr;
  counter = 0;
  fadeCounter = 0;
  reset_fades( );
  update();
}


void Glow_Manager::reset_fades( )
{
  for ( curLED = 0; curLED < numLEDs; curLED++ ) {
    
    fadeDelays[curLED][0] = 65500;
    fadeDelays[curLED][1] = 65500;
    fadeDelays[curLED][2] = 65500;
    colorAdjust[curLED][0] = 0;
    colorAdjust[curLED][1] = 0;
    colorAdjust[curLED][2] = 0;
  }
}


void Glow_Manager::set_brightness( uint8_t b )
{
  neopixel.setBrightness( b );
}


void Glow_Manager::pixel_update( )
{
  switch ( mc25aa_rbyte(curAddr) & 0xC0 ) {        
  case 0x00 :                                                         // remove pixel fade info
    pixel_remove_fade( );
    pixel_update( );
    break;
  case 0x40 :                                                         // do nothing with pixel
    curAddr++;     
    break;
  case 0x80 :                                                         // change pixel color
    pixel_update_color( );
    break;
  case 0xC0 :                                                         // adjust pixel fade info
    pixel_update_fade( );
    break;
  }   
}                                                           
                                         
void Glow_Manager::pixel_remove_fade( )
{
  colorAdjust[curLED][0] = 0;                
  colorAdjust[curLED][1] = 0;
  colorAdjust[curLED][2] = 0;
  fadeDelays[curLED][0] = 65500;     
  fadeDelays[curLED][1] = 65500;     
  fadeDelays[curLED][2] = 65500;     
  curAddr++; 
}                       

void Glow_Manager::pixel_update_color( )
{
  int16_t r, g, b;
  
  pixel_get_color( &r, &g, &b, mc25aa_rbyte(curAddr++)); 

  neopixel.setPixelColor( curLED, r, g, b);                
  
}
       
void Glow_Manager::pixel_update_fade( )
{
  int16_t r1, g1, b1, r2, g2, b2; 
 
  
  // Beginning Color
  pixel_get_color( &r1, &g1, &b1, mc25aa_rbyte(curAddr++));       // Grab first color and set it
  neopixel.setPixelColor( curLED, r1, g1, b1);              

  // Ending Color
  pixel_get_color( &r2, &g2, &b2, mc25aa_rbyte(curAddr++));

  // RED
  pixel_update_fade_info ( r2 - r1, 0); 
  
  // GREEN
  pixel_update_fade_info ( g2 - g1, 1); 

  // BLUE
  pixel_update_fade_info ( b2 - b1, 2); 
}


void Glow_Manager::update( ) 
{
  switch ( mc25aa_rbyte(curAddr++) ) {         
	 
  case 0 :
    reset_pattern( );
    return;
  case 1 :
    universalDelay = 1;
    break;
  case 2:
    universalDelay = 2;
    break;
  case 3 :
    universalDelay = 4;
    break;
  case 4 :
    universalDelay = 8;
    break;
  case 5 :
    universalDelay = 15;
    break;
  case 6 :
    universalDelay = 25;
    break;
  case 7 :
    universalDelay = 50;
    break;
  case 8 :
    universalDelay = 75;
    break;
  case 9 :
    universalDelay = 100;
    break;
  case 10 :
    universalDelay = 250;
    break;
  case 11 :
    universalDelay = 500;
    break;
  case 12 :
    universalDelay = 750;
    break;
  case 13 :
    universalDelay = 1000;
    break;
  case 14 :
    universalDelay = 1500;
    break;
  case 15 :
    universalDelay = 2000;
    break;
  case 16 :
    universalDelay = 2500;
    break;
  case 17 :
    universalDelay = 3000;
    break;
  case 18 :
    universalDelay = 3500;
    break;
  case 19 :
    universalDelay = 4000;
    break;
  case 20 :
    universalDelay = 5000;
    break;
  case 21 :
    universalDelay = 7500;
    break;
  case 22 :
    universalDelay = 10000;
    break;
  }

  counter = 0;
}


void Glow_Manager::pixel_update_fade_info( int16_t delta_color, uint8_t index )
{ 
  int8_t sign = delta_color/abs(delta_color);
  double d_color = abs(delta_color);
  uint16_t f_Delay;
  int16_t c_Adjust;
  
  if ( d_color ) {
    find_starting_vals(&f_Delay, &c_Adjust); 
    colorAdjust[curLED][index] = c_Adjust * sign;
    fadeDelays[curLED][index] = f_Delay;
  }
  
}

void Glow_Manager::find_starting_vals( uint16_t *f_Delay, int16_t *c_Adjust) {
  
  switch ( universalDelay ) {
  case 1 :
    *c_Adjust = 0; //////////////////
    *f_Delay = 0;
    break;
  case 2 :
    *c_Adjust = 0; // Dummy Vals
    *f_Delay = 0;
    break;
  case 4 :
   *c_Adjust = 0; 
    *f_Delay = 0;
    break;
  case 8 :
    *c_Adjust = 0; 
    *f_Delay = 0; /////////////
    break;
  case 15 :
    *c_Adjust = 8; 
    *f_Delay = 1;
    break;
  case 25 :
    *c_Adjust = 4; 
    *f_Delay = 1;
    break;
  case 50 :
    *c_Adjust = 2; 
    *f_Delay = 1;
    break;
  case 75 :
    *c_Adjust = 0;   // Find this one
    *f_Delay = 0;   
    break;
  case 100 :
    *c_Adjust = 1; 
    *f_Delay = 1;
    break;
  case 250 :
    *c_Adjust = 2; 
    *f_Delay = 5;
    break;
  case 500 :
    *c_Adjust = 1; 
    *f_Delay = 5;
    break;
  case 750 :
    *c_Adjust = 2; 
    *f_Delay = 15;
    break;
  case 1000 :
    *c_Adjust = 1; 
    *f_Delay = 10;
    break;
  case 1500 :
    *c_Adjust = 1; 
    *f_Delay = 15;
    break;
  case 2000 :
    *c_Adjust = 1; 
    *f_Delay = 20;
    break;
  case 2500 :
    *c_Adjust = 1; 
    *f_Delay = 25;
    break;
  case 3000 :
    *c_Adjust = 1; 
    *f_Delay = 30;
    break;
  case 3500 :
    *c_Adjust = 1; 
    *f_Delay = 35;
    break;
  case 4000 :
    *c_Adjust = 1; 
    *f_Delay = 40;
    break;
  case 5000 :
    *c_Adjust = 1; 
    *f_Delay = 50;
    break;
  case 7500 :
    *c_Adjust = 1; 
    *f_Delay = 75;
    break;
  case 10000 :
    *c_Adjust = 1; 
    *f_Delay = 100;
    break;
  }

  if ( brightness == 200 ) { 
    *c_Adjust *= 2; 
  } else if ( brightness == 50 ) {
    *f_Delay *= 2;
  }
}

void Glow_Manager::pixel_get_color( int16_t *r, int16_t *g, int16_t *b, char data ) {
  
  switch ( data & 0x3F ) {      
  case 0 :    // BLACK
    *r = 0;
    *g = 0;
    *b = 0;
    break;
  case 1 :    // WHITE
    *r = 255;
    *g = 255;
    *b = 255;
    break;        
  case 2 :    // RED
    *r = 255;
    *g = 0;
    *b = 0;
    break;        
  case 3 :    // GREEN
    *r = 0;
    *g = 255;
    *b = 0;
    break;  
  case 4 :    // BLUE
    *r = 0;
    *g = 0;
    *b = 255;
    break;  
  case 5 :    // YELLOW
    *r = 255;
    *g = 255;
    *b = 0;
    break;
  case 6 :    // PURPLE
    *r = 255;
    *g = 0;
    *b = 255;
    break;
  case 7 :    // CYAN
    *r = 0;
    *g = 255;
    *b = 255;
    break;
  case 8 :    // PALE_YELLOW
    *r = 200;
    *g = 200;
    *b = 60;
    break;
  case 9 :    // MID_BLUE
    *r = 0;
    *g = 100;
    *b = 255;
    break;
  case 10 :   // RED_ORANGE
    *r = 255;
    *g = 50;
    *b = 0;
    break;
  case 11 :   // ORANGE_YELLOW
    *r = 255;
    *g = 120;
    *b = 0;
    break;
  case 12 :   // LIME
    *r = 80;
    *g = 255;
    *b = 0;
    break;
  case 13 :   // PEAR
    *r = 140;
    *g = 255;
    *b = 0;
    break;
  case 14 :   // BANANA
    *r = 170;
    *g = 255;
    *b = 0;
    break;
  case 15 :   // LIGHT_PURPLE
    *r = 110;
    *g = 0;
    *b = 255;
    break;
  case 16 :   // LIGHT_BLUE
    *r = 70;
    *g = 0;
    *b = 255;
    break;
 case 17 :    // PALE_PINK
    *r = 255;
    *g = 170;
    *b = 170;
    break;
  case 18 :   // PINK
    *r = 255;
    *g = 70;
    *b = 130;
    break;
  case 19 :   // PALE_ORANGE
    *r = 255;
    *g = 190;
    *b = 75;
    break;
  case 20 :   // LIGHT_ORANGE
    *r = 255;
    *g = 135;
    *b = 35;
    break;
 
 }
}
