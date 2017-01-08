#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "fifo.h"

#define ATMEGA_BAUD 9600UL
#define ATMEGA_UBRR (F_CPU/(16UL*ATMEGA_BAUD))-1

FIFO( 64 ) uart_tx_fifo;
FIFO( 64 ) uart_rx_fifo;

void uart_init(unsigned int ubrr)
{
  //Для настройки скорости используется регистр UBRR  (Usart Boud Rate Register)
   UBRR0H = (unsigned char)(ubrr>>8);
   UBRR0L = (unsigned char)ubrr;
  /*
    UCSRnA = RXCn TXCn UDREn FEn DORn UPEn U2Xn MPCMn
  */
  //UCSR0A = ;
  /*
    UCSRnB = RXCIEn | TXCIEn | UDRIEn | RXENn | TXENn | UCSZn2 | RXB8n | TXB8n
    RXCIEn: RX Complete Interrupt Enable n
    TXCIEn: TX Complete Interrupt Enable n
    UDRIEn: USART Data Register Empty Interrupt Enable n
    RXENn: Receiver Enable n
    TXENn: Transmitter Enable n
    UCSZn2: Character Size n
    RXB8n: Receive Data Bit 8 n
    TXB8n: Transmit Data Bit 8 n

    разрешить прием, передачу данных и прерывание по приёму байта
  */
  UCSR0B = (1 << RXCIE0)
         | (1 << TXCIE0)
         | (0 << UDRIE0)
         | (1 << TXEN0)
         | (1 << RXEN0);
  /*
    UCSRnC = UMSELn1 | UMSELn0 | UPMn1 | UPMn0 | USBSn | UCSZn1 | UCSZn0 | UCPOLn
    UMSELn1 UMSELn0 :
          0       0   Asynchronous USART
          0       1   Synchronous USART
          1       0   (Reserved)
          1       1   Master SPI (MSPIM)(1)
    UPMn1 UPMn0 Parity Mode
        0     0 Disabled
        0     1 Reserved
        1     0 Enabled, Even Parity
        1     1 Enabled, Odd Parity
    USBSn Stop Bit(s)
        0 1-bit
        1 2-bit
    UCSZn2 UCSZn1 UCSZn0 Character Size
         0      0      0 5-bit
         0      0      1 6-bit
         0      1      0 7-bit
         0      1      1 8-bit
         1      0      0 Reserved
         1      0      1 Reserved
         1      1      0 Reserved
         1      1      1 9-bit
    UCPOLn :
    This bit is used for synchronous mode only. Write this bit to zero when asynchronous mode is used. The
    UCPOLn bit sets the relationship between data output change and data input sample, and the synchronous
    clock (XCKn).
  */
  UCSR0C = (0 << UMSEL01)
         | (0 << UMSEL00)
         | (0 << UPM01)
         | (0 << UPM00)
         | (0 << USBS0)
         | (0 << UCSZ02)
         | (1 << UCSZ01)
         | (1 << UCSZ00);
}


//Обработчик прерывания по окончанию приёма байта
ISR( USART_RX_vect )
{
  unsigned char rxbyte = UDR0;
  if( !FIFO_IS_FULL( uart_rx_fifo ) ) {
    FIFO_PUSH( uart_rx_fifo, rxbyte );
  }
}

//USART, Data Register Empty
ISR( USART_UDRE_vect )
{
  if( FIFO_IS_EMPTY( uart_tx_fifo ) ) {
    //если данных в fifo больше нет то запрещаем это прерывание
    UCSR0B &= ~( 1 << UDRIE0 );
  }
  else {
    //иначе передаем следующий байт
    char txbyte = FIFO_FRONT( uart_tx_fifo );
    FIFO_POP( uart_tx_fifo );
    UDR0 = txbyte;
  }
}



int uart_putc(  char c, FILE *file )
{
  int ret;
  cli(); //запрещаем прерывания
  if( !FIFO_IS_FULL( uart_tx_fifo ) ) {
    //если в буфере есть место, то добавляем туда байт
    FIFO_PUSH( uart_tx_fifo, c );
    //и разрешаем прерывание по освобождению передатчика
    UCSR0B |= ( 1 << UDRIE0 );
    ret = 0;
  }
  else {
    ret = -1; //буфер переполнен
  }
  sei(); //разрешаем прерывания
  return ret;
}


int uart_getc( FILE* file )
{
  int ret;
  cli(); //запрещаем прерывания
  if( !FIFO_IS_EMPTY( uart_rx_fifo ) ) {
    //если в буфере есть данные, то извлекаем их
    ret = FIFO_FRONT( uart_rx_fifo );
    FIFO_POP( uart_rx_fifo );
  }
  else {
    ret = _FDEV_EOF; //данных нет
  }
  sei(); //разрешаем прерывания
  return ret;
}


FILE uart_stream = FDEV_SETUP_STREAM(uart_putc, uart_getc, _FDEV_SETUP_RW);

int main( void )
{
  stdout = stdin = &uart_stream;
  uart_init(ATMEGA_UBRR);
  sei();
  while( 1 ) {
    int c = getchar();
    if( c != EOF ) {
      putchar( c );
    }
  }
  return 0;
}