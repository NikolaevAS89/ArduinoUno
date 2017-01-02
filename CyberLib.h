#ifndef CyberLib_H
#define CyberLib_H

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif 
//work only with my atmega
#if defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
//**********Small UART****************************
#define UART_DOUBLESPEED
#define UART_TXREADY UDRE0
#define UART_RXREADY RXC0
#define UART_DOUBLE	U2X0
#define UDR  UDR0
#define UCSRA UCSR0A
#define UCSRB UCSR0B
#define UCSRC UCSR0C
#define UCSRC_STOPBIT (0) 
#define UDRIE (1 << 5)
//Synchronous USART
#define UCRC_SYNCH ( 1 << UMSEL00 ) 
//8-bit Character Size
#define UCRC_8_BIT ((1<<UCSZ01) | (1<<UCSZ00)) 

#define RXCIE RXCIE0
#define TXCIE TXCIE0
#define RXEN RXEN0
#define TXEN TXEN0
#define UBRRL UBRR0L
#define UBRRH UBRR0H
#define SIG_UART_TRANS SIG_USART_TRANS
#define SIG_UART_RECV  SIG_USART_RECV
#define SIG_UART_DATA  SIG_USART_DATA
#define UART_CALC_BAUDRATE(baudRate) ((uint32_t)((F_CPU) + ((uint32_t)baudRate * 4UL)) / ((uint32_t)(baudRate) * 8UL) - 1)
//PIN's
//DIGITAL
#define DPIN_0 0
#define DPIN_1 1
#define DPIN_2 2
#define DPIN_3 3
#define DPIN_4 4
#define DPIN_5 5
#define DPIN_6 6
#define DPIN_7 7
#define DPIN_8 8
#define DPIN_9 9
#define DPIN_10 10
#define DPIN_11 11
#define DPIN_12 12
#define DPIN_13 13
//ANALOG
#define APIN_0 14
#define APIN_1 15
#define APIN_2 16
#define APIN_3 17
#define APIN_4 18
#define APIN_5 19
//utils
#define _lo(x, y, v1, v2) ((x)<(y) ? (v1) : (v2))
#define _ddr(i) _lo((i), 8, DDRD, _lo((i), 14, DDRB, DDRC))
#define _port(i) _lo((i), 8, PORTD, _lo((i), 14, PORTB, PORTC))
#define _pin(i) _lo((i), 8, PIND, _lo((i), 14, PINB, PINC))
#define _number(i) ((i)-_lo((i), 8, 0, _lo((i), 14, 8, 14)))
#define _an_pin(i) (B01000000 | (i))
#define _byte_pos(pos) (B00000001<<(pos))
#define _byte_inv(pos) (B11111111^(B00000001<<(pos)))
//set In - Out
#define setPortIn(i) _ddr((i)) &= _byte_inv(_number((i)))
#define setPortOut(i) _ddr((i)) |= _byte_pos(_number((i)))
//set Hi- Low
#define setHi(i) _port((i)) |= _byte_pos(_number((i)))
#define setLow(i) _port((i)) &= _byte_inv(_number((i)))
//invert
#define invert(i) _port((i)) ^= _byte_pos(_number((i)))
//read digit pin status
#define readPin(i) ((_pin((i)) & _byte_pos(_number((i))))>>_number((i)))
//read analog pin status
#define readAPin(i) (AnRead(_an_pin(_number((i)))))
/*
	DELAY
	Функции delay_us() и delay_ms() можно применять в прерываниях
	так как они не используют таймер, но следует не забывать что
	точность этих функций зависит от использования в коде обработчиков
	прерываний. Если Вы не используете в коде прерывания то и точность
	будет высокая
*/
/* 
	задержка в мкс, максимальная задержка может быть не больше 16000мкс
	tic_us время задержки в mcs
*/
void delay_us(uint16_t tic_us);	
/*
	задержка в мс, максимальная задержка может быть не больше 65000мс это равно 65сек
	tic_ms время задержки в ms
*/
void delay_ms(uint16_t tic_ms);
//ANALOG READ
uint16_t AnRead(uint8_t An_pin);
/*
  	Soft Reset
  	При помощи этой функции можно отправить контроллер в ребут 
  	в любом месте выполняемой программы
*/
void reset();
/*
	Генерирует звуковые колебания, на любом пине с заданной частотой и длительностью
	dur - длительность от 50мс до 65535 мс
	frq - частота от 10 Гц до 2000 Гц
*/
void beep(uint16_t pin, uint16_t dur, uint16_t frq);

//SERIAL 
/*
	Инициализация последовательного порта
*/
void UART_Init(uint32_t UART_BAUD_RATE);
/*
	получить байт данных из последовательного порта
*/
void UART_SendByte(uint8_t b);
/*
	отправить байт данных в последовательный порт
*/
bool UART_ReadByte(uint8_t & data);
/*
	Функция отправляет в UART порт, массив типа байт, максимальный
	объем которого не должен превышать 65535 байт, минимальный
	объем массива 1 байт. Можно также отправлять часть массива.
	buffer указатель на массив
	bufferSize число отправляемых байт массива
*/
void UART_SendArray(uint8_t *buffer, uint16_t bufferSize);
#else
#error
#endif	
 
/*

//************SPI********************************
	void StartSPI(uint8_t SPI_Mode, uint8_t SPI_Div, uint8_t SPI_Change_Shift );
	void StopSPI(void);
	uint8_t ReadSPI(void);
	void SendSPI(uint8_t SPI_data) ;
	
//**************Converter*******************	
	uint8_t CharToDec(uint8_t digit);	
	uint8_t DecToChar(uint8_t number);
	
//******************EEPROM*******************************
	void WriteEEPROM_Byte(uint8_t addr, uint8_t data);  //��������� � EEPROM ��� Byte
	void WriteEEPROM_Word(uint16_t addr, uint16_t data);  //��������� � EEPROM ��� Word
	void WriteEEPROM_Long(uint8_t addr, uint32_t data);  //��������� � EEPROM ��� Long
	uint8_t ReadEEPROM_Byte(uint8_t addr);  // ��������� �������� Byte �� EEPROM
	uint16_t ReadEEPROM_Word(uint16_t addr);  // ��������� �������� Word �� EEPROM
	uint32_t ReadEEPROM_Long(uint8_t addr);  // ��������� �������� Long �� EEPROM

//**************Timer1*************************
	extern void (*func)();	
	void StartTimer1(void (*isr)(), uint32_t set_us);
	void StopTimer1(void);
	void ResumeTimer1(void);
	void RestartTimer1(void);
	
//**************Find****************************
uint16_t find_similar(uint16_t *buf, uint8_t size_buff, uint8_t range);



 
		*/
//**************End***************************
#endif //CyberLib_H
