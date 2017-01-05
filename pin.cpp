#include "pin.h"

//ANALOG READ
uint16_t AnRead(uint8_t An_pin)
{
  ADMUX=An_pin;   
  delay_us(10);	  
  ADCSRA=B11000110;	//B11000111-125kHz B11000110-250kHz 
  while (ADCSRA & (1 << ADSC));
  An_pin = ADCL;
  uint16_t An = ADCH; 
  return (An<<8) + An_pin;
}