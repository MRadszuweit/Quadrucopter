
#include "usart.h"

#ifndef USART_DOUBLE_SPEED
	#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)   // clever runden
	#define BAUD_REAL (F_CPU/(16*(UBRR_VAL+1)))     // Reale Baudrate
#else
	#define UBRR_VAL ((F_CPU+BAUD*4)/(BAUD*8)-1)  
	#define BAUD_REAL (F_CPU/(8*(UBRR_VAL+1)))    	
#endif

#define BAUD_ERROR ((BAUD_REAL*1000)/BAUD) // Fehler in Promille, 1000 = kein Fehler. 
#if ((BAUD_ERROR<990) || (BAUD_ERROR>1010))
  #error Baud rate error too large: (BAUD_ERROR-1000)/10 %
#endif

void enable_async_usart(uint16_t baud){	
	DDRD &= ~(1<<PD0);
	PORTD |= (1<<PD0);
	uint16_t ubrr_val = baud;//UBRR_VAL;			// 9600 baud @ 3.6864 MHz
	UBRRH = (uint8_t) (ubrr_val>>8);				// double speed enabled
	UBRRL = (uint8_t) ubrr_val;	
#ifdef USART_DOUBLE_SPEED
	UCSRA |= (1<<U2X);
#endif
	UCSRB = (1<<RXEN) | (1<<TXEN);
	UCSRC = (1<<URSEL)  | (1<<UCSZ1) | (1<<UCSZ0); 	// no parity bits
													// one stop bit, two: (1<<USBS)
													// 8 bit frame		
}

void uart_enable_receive_int(void){
	UCSRB |= (1<<RXCIE);
}

void uart_disable_receive_int(void){
	UCSRB &= ~(1<<RXCIE);
}

void uart_transmit_byte(uint8_t val){
	while (!(UCSRA & (1<<UDRE))){};
	UDR = val;
}

uint8_t uart_receive_byte(uint16_t timeout){
	uint16_t i = 0;
	while (!(UCSRA & (1<<RXC)) && i<timeout){i++;};
	if (i==timeout) return 0; else return UDR;
}

void uart_string_transmit(char* String,volatile uint8_t size){
	uint8_t i;													// first 8 chars reserved
	uart_transmit_byte(ASCII_START);							// subtract 8 at receiver
	uart_transmit_byte(size+8);
	for (i=0;i<size;i++) uart_transmit_byte(String[i]);
	uart_transmit_byte(ASCII_END);
}

uint8_t string_length(char* str,volatile uint8_t max){
	volatile uint8_t i = 0;
	while (str[i]!='\0' && i<max){i++;}
	return i;
}

void string_append(char* dest,const char* source,uint8_t len){
	volatile uint8_t i = 0;
	while (dest[i]!='\0'){i++;}
	volatile uint8_t j = 0;
	while (source[j]!='\0' && j<len){
		dest[i+j] = source[j];
		j++;
	}
	dest[i+j] = '\0';
}

void uart_decimal_transmit(int8_t n){
	char String[5];	
	ltoa((long)n,String,0x0A);
	uart_string_transmit(String,5);
}

uint8_t get_ubrr(void){
	return UBRR_VAL;
}

char dec_to_ascii(uint8_t d){
	return 0x30+d;
}

char byte_to_hexascii(uint8_t d){
	if (d<0x0A) return 0x30+d;
	else{
		if (d==0x0A) return 'a';
		if (d==0x0B) return 'b';
		if (d==0x0C) return 'c';
		if (d==0x0D) return 'd';
		if (d==0x0E) return 'e';
		if (d==0x0F) return 'f';		
	}
}

void to_string8(char* Buffer,int8_t val){
	int len = 0;
	if (val & (1<<8)){
		Buffer[0]='-';
		val *= -1;
		len++;
	}
	uint8_t start = 0x00;
	uint8_t d = val / 0x64;
	if (d!=0){
		Buffer[len] = dec_to_ascii(d);
		val -= 0x64*d;
		start = 0x01;
		len++;
	}
	d = val / 0x0A;
	if (d!=0 || start){
		Buffer[len] = dec_to_ascii(d);
		val -= 0x0A*d;
		start = 0x01;
		len++;
	}
	Buffer[len] = dec_to_ascii(val);
	Buffer[len+1] = '\0';	
}

void to_string16(char* Buffer,int16_t val){
	uint8_t sreg = SREG;
	cli();
	int len = 0;
	if (val & (0x8000)){
		Buffer[0]='-';
		val *= -1;
		len++;
	}
	uint8_t start = 0x00;
	uint16_t d = (int16_t) val / 0x2710;
	if (d!=0){
		Buffer[len] = dec_to_ascii(d);
		val -= 0x2710*d;
		start = 0x01;
		len++;
	}
	d = val / 0x03E8;
	if (d!=0 || start){
		Buffer[len] = dec_to_ascii(d);
		val -= 0x03E8*d;
		start = 0x01;
		len++;
	}
	d = val / 0x0064;
	if (d!=0 || start){
		Buffer[len] = dec_to_ascii(d);
		val -= 0x0064*d;
		start = 0x01;
		len++;
	}
	d = val / 0x000A;
	if (d!=0 || start){
		Buffer[len] = dec_to_ascii(d);
		val -= 0x000A*d;
		start = 0x01;
		len++;
	}	
	Buffer[len] = dec_to_ascii(val);
	Buffer[len+1] = '\0';
	SREG = sreg;	
}

void single_to_hex_string(char* Buffer,uint32_t data){
	uint8_t halfbyte,i;
	uint8_t size = 8;
	Buffer[size] = '\0';	
	
	i = size;
	while (i>0){
		halfbyte = (uint8_t) data;		
		Buffer[i-1] = byte_to_hexascii(halfbyte & 0x0F);	
		data >>= 4;
		i--;
	};	
}
