#include <stdio.h>
#include "mc25aa.h"
#include "glow_manager.h"
#include "button.h"

#define NUMCOLORS 20
#define MAXDEFPATTERNS 6

#define FBLACK   0xC0
#define FWHITE   0xC1 
#define FRED     0xC2
#define FGREEN   0xC3
#define FBLUE    0xC4  
#define FYELLOW  0xC5
#define FPURPLE  0xC6
#define FCYAN    0xC7

#define RFADE  0x00

#define BLACK         0x80
#define WHITE         0x81 
#define RED           0x82
#define GREEN         0x83
#define BLUE          0x84  
#define YELLOW        0x85
#define PURPLE        0x86
#define CYAN          0x87
#define PALE_YELLOW   0x88
#define MID_BLUE      0x89
#define RED_ORANGE    0x8A
#define ORANGE_YELLOW 0x8B
#define LIME          0x8C
#define PEAR          0x8D
#define BANANA        0x8E
#define LIGHT_PURPLE  0x8F
#define LIGHT_BLUE    0x90
#define PALE_PINK     0x91
#define PINK          0x92
#define PALE_ORANGE   0x93
#define LIGHT_ORANGE  0x94

#define RESET   0x00

#define MS1 1
#define MS2 2
#define MS4 3
#define MS8 4
#define MS15 5
#define MS25 6
#define MS50 7
#define MS75 8
#define MS100 9
#define MS250 10
#define MS500 11
#define MS750 12
#define MS1000 13
#define MS1500 14
#define MS2000 15
#define MS2500 16
#define MS3000 17
#define MS3500 18
#define MS4000 19
#define MS5000 20
#define MS7500 21
#define MS10000 22



int main() {   

  unsigned int defaultPatternAddrs[] = {
    1,
    20,
    75,
    127,
    173,
    300
  };
  

  char defaultPatterns[] = {
    3,
    MS2,               // 1
    WHITE,
    WHITE,
    WHITE,
    WHITE,
    WHITE,
    WHITE,
    WHITE,
    WHITE,
    MS4,
    BLACK,
    BLACK,
    BLACK,
    BLACK,
    BLACK,
    BLACK,
    BLACK,
    BLACK,
    RESET,
    /////////////////////////////////////////////
    MS25,              // 20
    RED,
    RED,
    RED,
    RED,
    RED,
    RED,
    RED,
    RED,
    MS8,
    BLACK,
    BLACK,
    BLACK,
    BLACK,
    BLACK,
    BLACK,
    BLACK,
    BLACK,
    MS25,
    GREEN,
    GREEN,          // 40
    GREEN,
    GREEN,
    GREEN,
    GREEN,
    GREEN,
    GREEN,
    MS8,
    BLACK,
    BLACK,
    BLACK,
    BLACK,
    BLACK,
    BLACK,
    BLACK,
    BLACK,
    MS25,
    BLUE,
    BLUE,
    BLUE,
    BLUE,         // 60
    BLUE,
    BLUE,
    BLUE,
    BLUE,
    MS8,
    BLACK,
    BLACK,
    BLACK,
    BLACK,
    BLACK,
    BLACK,
    BLACK,
    BLACK,
    RESET,
    ////////////////////////////////////////////////////////
    MS3000,       // 75
    FRED,
    FGREEN,
    FRED,
    FGREEN,
    FRED,          // 80
    FGREEN,
    FRED,
    FGREEN,
    FRED,
    FGREEN,
    FRED,
    FGREEN,
    FRED,
    FGREEN,
    FRED,
    FGREEN,

    MS3000,
    FGREEN,
    FBLUE,
    FGREEN,
    FBLUE,
    FGREEN,
    FBLUE,
    FGREEN,
    FBLUE,       // 100
    FGREEN,
    FBLUE,
    FGREEN,
    FBLUE,
    FGREEN,
    FBLUE,
    FGREEN,
    FBLUE,

    MS3000,
    FBLUE,
    FRED,
    FBLUE,
    FRED,
    FBLUE,
    FRED,
    FBLUE,
    FRED,
    FBLUE,
    FRED,
    FBLUE,      // 120
    FRED,
    FBLUE,
    FRED,
    FBLUE,
    FRED,
    RESET,
    //////////////////////////////////////////////////
    MS2,        // 127
    LIGHT_BLUE,
    BLACK,
    BLACK,
    BLACK,
    BLACK,
    BLACK,
    BLACK,
    LIGHT_BLUE,
    MS2,
    BLACK,
    LIGHT_BLUE,
    BLACK,
    BLACK,      // 140
    BLACK,
    BLACK,
    LIGHT_BLUE,
    BLACK,
    MS2,
    BLACK,
    BLACK,
    LIGHT_BLUE,
    BLACK,
    BLACK,
    LIGHT_BLUE,
    BLACK,
    BLACK,
    MS2,
    BLACK,
    BLACK,
    BLACK,
    LIGHT_BLUE,
    LIGHT_BLUE,
    BLACK,       // 160
    BLACK,
    BLACK,
    MS4,
    BLACK,
    BLACK,
    BLACK,
    BLACK,
    BLACK,
    BLACK,
    BLACK,
    BLACK,
    RESET,
    //////////////////////////////////////
    MS1,        // 173
    RED,
    BLACK,
    BLACK,
    BLACK,
    BLACK,
    BLACK,
    BLACK,      // 180
    BLUE,
    MS1,
    BLACK,
    RED,
    BLACK,
    BLACK,
    BLACK,
    BLACK,
    BLUE,
    BLACK,
    MS1,
    BLACK,
    BLACK,
    RED,
    BLACK,
    BLACK,
    BLUE,
    BLACK,
    BLACK,      
    MS1,          // 200
    BLACK,
    BLACK,
    BLACK,
    RED,
    BLUE,
    BLACK,
    BLACK,
    BLACK,
    MS1,
    BLACK,
    BLACK,
    BLACK,
    BLUE,
    RED,
    BLACK,
    BLACK,
    BLACK,
    MS1,
    BLACK,
    BLACK,        // 220
    BLUE,
    BLACK,
    BLACK,
    RED,
    BLACK,
    BLACK,
    MS1,
    BLACK,
    BLUE,
    BLACK,
    BLACK,
    BLACK,
    BLACK,
    RED,
    BLACK,
    MS1,
    BLUE,
    BLACK,
    BLACK,
    BLACK,       // 240
    BLACK,
    BLACK,
    BLACK,
    RED,
    MS1,
    BLACK,
    BLUE,
    BLACK,
    BLACK,
    BLACK,
    BLACK,
    RED,
    BLACK,
    MS1,
    BLACK,
    BLACK,
    BLUE,
    BLACK,
    BLACK,
    RED,         // 260
    BLACK,
    BLACK,
    MS1,
    BLACK,
    BLACK,
    BLACK,
    BLUE,
    RED,
    BLACK,
    BLACK,
    BLACK,
    MS1,
    BLACK,
    BLACK,
    BLACK,
    RED,
    BLUE,
    BLACK,
    BLACK,
    BLACK,        // 280
    MS1,
    BLACK,
    BLACK,
    RED,
    BLACK,
    BLACK,
    BLUE,
    BLACK,
    BLACK,
    MS1,
    BLACK,
    RED,
    BLACK,
    BLACK,
    BLACK,
    BLACK,
    BLUE,
    BLACK,
    RESET,
    //////////////////////////////////////
    MS15,
GREEN,
GREEN,
YELLOW,
YELLOW,
YELLOW,
RED,
RED,
RED,

MS15,
GREEN,
YELLOW,
YELLOW,
YELLOW,
RED,
RED,
RED,
GREEN,

MS15,
YELLOW,
YELLOW,
YELLOW,
RED,
RED,
RED,
GREEN,
GREEN,

MS15,
YELLOW,
YELLOW,
RED,
RED,
RED,
GREEN,
GREEN,
GREEN,

MS15,
YELLOW,
RED,
RED,
RED,
GREEN,
GREEN,
GREEN,
YELLOW,

MS15,
RED,
RED,
RED,
GREEN,
GREEN,
GREEN,
YELLOW,
YELLOW,

MS15,
RED,
RED,
GREEN,
GREEN,
GREEN,
YELLOW,
YELLOW,
YELLOW,

MS15,
RED,
GREEN,
GREEN,
GREEN,
YELLOW,
YELLOW,
YELLOW,
RED,

MS15,
GREEN,
GREEN,
GREEN,
YELLOW,
YELLOW,
YELLOW,
RED,
RED,

RESET 

  };    
     
   	 
  Button button; // PORTC 
  Glow_Manager glo(8, 0, 100); // 50, 100, 200 

  // ------------------------------------------------------------------------
  // Writing pattern to memory
 
  spi_init();
  
  mc25aa_wren();
  mc25aa_wpage(0, 256, defaultPatterns);
  wait_while_wip();

  mc25aa_wren();
  mc25aa_wpage(256, 256, defaultPatterns + 256);
  wait_while_wip();
 
  // ------------------------------------------------------------------------ 
  
  glo.initialize();
  button.Init();
  glo.show();
   
  // ------------------------------------------------------------------------
 
  while ( !button.tierChosen ) {
    glo.solid_color(button.tier + 1);
    button.WaitForResponse();
    
    if ( button.time <= 500 ) {
      button.ChangeTier();
    } else if (button.TurnOffGlo()){
      glo.solid_color(BLACK);
      return 0;
    } else {
      button.tierChosen = 1; 
      button.time = 0;  
    } 
  }
 


  switch ( button.tier ) {
    
  case 1 :  //DEFAULT PATTERN TIER
    
    button.tier = 0;
    button.time = 0;
    glo.patternAddr = defaultPatternAddrs[button.tier];
    glo.curAddr = glo.patternAddr;

    while( button.time < 1000 ) {

      if (glo.counter % glo.universalDelay == 0) {  
	glo.update();    
	glo.update_pixels();
	glo.show();
      }
    
      glo.delay_ms(1);
      glo.counter++;
      glo.fade( );

      if ( button.IsPressed()) {
	button.time++;
      } else {

	if ( button.time < 1000 && button.time > 10 ) {
	  button.tier++;
	  if ( button.tier == MAXDEFPATTERNS ) { button.tier = 0; }
	  glo.patternAddr = defaultPatternAddrs[button.tier];
	  glo.curAddr = glo.patternAddr;
	  glo.reset_fades();
	  glo.counter = glo.universalDelay;
	}
	button.time = 0;
      } 
    }
    
    glo.solid_color(BLACK);
    break;
  case 2 :
    // SOLID PATTERN TIER
    button.tier = 1;
    
    while( 1 ) {
      glo.solid_color( button.tier );
      button.WaitForResponse();
      
      if ( button.time < 1500 ) {
	button.tier++;
	if ( button.tier > NUMCOLORS ) {
	  button.tier = 1;
	}
      } else {
	glo.solid_color( BLACK );
	return 0;
      }
    } 
    break;
  case 3 :
    // CUSTOM PATTERN TIER*/
    while (1) {   
      if (glo.counter % glo.universalDelay == 0) {  
	glo.update();    
	glo.update_pixels();
	glo.show();
      }
    
      glo.delay_ms(1);
      glo.counter++;
      glo.fade( );
    } 
    break;
    }
    return 0;
}


  
  
