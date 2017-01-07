#include "pin.h"

//ANALOG READ
uint16_t AnRead(uint8_t An_pin)
{
  ADMUX=An_pin;   
  int tic_us = 40;
	__asm__ volatile 
		  (	
			"1: sbiw %0,1" "\n\t"
			"brne 1b"				
			: "=w" (tic_us)
			: "0" (tic_us)
		  );  
  ADCSRA=0b11000110;	//B11000111-125kHz B11000110-250kHz 
  while (ADCSRA & (1 << ADSC));
  An_pin = ADCL;
  uint16_t An = ADCH; 
  return (An<<8) + An_pin;
}