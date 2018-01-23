#ifndef USART
#define USART

#ifndef F_CPU
#define F_CPU 3686400UL
#endif

#define BAUD 115200UL
//#define BAUD 230400UL

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define ASCII_START 0x02
#define ASCII_END 0x03
#define USART_DOUBLE_SPEED

void enable_async_usart(uint16_t baud);
void uart_transmit_byte(uint8_t val);
uint8_t uart_receive_byte(uint16_t timeout);
uint8_t get_ubrr(void);
void uart_string_transmit(char* String,uint8_t size);
void uart_decimal_transmit(int8_t n);
uint8_t string_length(char* str,uint8_t max);
void string_append(char* dest,const char* source,uint8_t len);
void to_string8(char* Buffer,int8_t val);
void to_string16(char* Buffer,int16_t val);
void uart_enable_receive_int(void);
void uart_disable_receive_int(void);

#endif
