#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "button.h"
#include "mc25aa.h"

void Button::Init(void) { 
  DDRB &= ~(0x01);    // input
  PORTB |= 0x01;      // pull-up resistor
  tier = 1;
  tierChosen = 0;
  maxTier = mc25aa_rbyte(0);
}

char Button::IsPressed(void) { 
  return !(PINB & 0x01);   
}

char Button::IsReleased(void) {
  return (PINB & 0x01);
}

void Button::ChangeTier(void) {
  tier++;
  
  if ( tier > maxTier ) { tier = 1; }
}

char Button::TurnOffGlo(void) {
  return time >= 1500;
}

void Button::WaitForResponse(void) {
  time = 0;

  while( time <= 30 ) {
    time = 0;
    while( !IsPressed()) {};
    while( !TurnOffGlo() && !IsReleased() ){
      _delay_ms(1);
      time++;
    }
  }
}


