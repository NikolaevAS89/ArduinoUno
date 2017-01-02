#include "CyberLib.h"
#include <avr/interrupt.h>
#include <avr/io.h> 

//USART, RX Complete Handler
ISR( USART_RX_vect )
{
  setHi(DPIN_4);
  delay(1000);              // wait for a second
}

//USART, UDR Empty Handler
ISR( USART_UDRE_vect )
{
  setHi(DPIN_4);
  delay(1000);              // wait for a second
}

//USART, TX Complete Handler
ISR(USART_TX_vect){
  setHi(DPIN_4);
  delay(1000);              // wait for a second
}


void setup()
{
   
  UBRR0H = (UART_CALC_BAUDRATE(9600)>>8) & 0xFF;
  UBRR0L = (UART_CALC_BAUDRATE(9600) & 0xFF);
  UCSR0B = ((1 << TXEN0)  | (1 << RXEN0) | (1 << RXCIE0));
  UCSR0C = ((1 << UCSZ01) | (1 << UCSZ00 ));



  setPortOut(DPIN_4);
  interrupts();
}

void loop()
{
}