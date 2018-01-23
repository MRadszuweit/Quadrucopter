#ifndef UART64
#define UART64

#ifndef F_CPU
	#define F_CPU 7372800UL
#endif

#define BAUD 9600UL

#ifndef NO
	#define NO 0x00
#endif
#ifndef YES
	#define YES 0x01
#endif

#ifndef FAILED
	#define FAILED 0x00
#endif
#ifndef SUCCESS
	#define SUCCESS 0x01
#endif

#define UART0 0x00
#define UART1 0x01
#define UART_SINGLE_SPEED 0x00
#define UART_DOUBLE_SPEED 0x01
#define ONE_STOP_BIT 0x01
#define TWO_STOP_BITS 0x02
#define UART1 0x01
#define NO_PARITY 0x00
#define ODD_PARITY 0x01
#define EVEN_PARITY 0x02

#define FRAME_5BIT 0x00
#define FRAME_6BIT 0x01
#define FRAME_7BIT 0x02
#define FRAME_8BIT 0x03
#define FRAME_9BIT 0x07

#define API_READ_SINGLE 0xFF
#define API_READ_ALL 0x00
#define API1 0xF1
#define API2 0xF2
#define API_DELIM 0x7E
#define API_ESCAPE 0x7D
#define API_XON 0x11
#define API_XOFF 0x13
#define API_XOR_MASK 0x20
#define API_RESPONSE 0x01
#define API_QUIET 0x00
#define API_DEFAULT 0x00
#define API_OK 0x00

#define API_TRANSMIT_REQUEST_64BIT 0x00
#define API_AT_COMMAND 0x08
#define API_REMOTE_AT_COMMAND 0x17
#define API_AT_COMMAND_RESPONSE 0x88
#define API_AT_REMOTE_COMMAND_RESPONSE 0x97
#define API_RECEIVE_TRANSMISSION 0x80
#define API_TRANSMIT_STATUS 0x89
#define API_RECEIVE_IO_SAMPLE 0x82
#define API_BROKEN_FRAME 0xFE

#define ERR_BUFFER_EMPTY 0x01
#define ERR_BROKEN_FRAME 0x02
#define ERR_THAT_SHOULD_NOT_HAPPEN 0x03
#define ERR_FRAME_TOO_LARGE 0x04
#define ERR_CHECK_SUM 0x05
#define ERR_UNKNOWN_MESSAGE 0x06
#define ERR_INCONSISTENT_LENGTH 0x07
#define ERR_LENGTH 0x08

#define HEX_0 0x30
#define HEX_A 0x41

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>

typedef union UINT32{
	uint32_t t32;
	uint16_t t16[2];
	uint8_t t8[4];
}uint32;

typedef union UINT16{
	int16_t t16;
	uint8_t t8[2];
}uint16;

//////////////////////////////// functions /////////////////////////////

void enable_uart(uint8_t uart_ID,uint8_t frame_size,uint8_t stop_bits
 ,uint8_t parity,uint8_t speed); 
void uart_enable_receive_int(uint8_t uart_ID);
void uart_enable_transmit_complete_int(uint8_t uart_ID);
void uart_enable_transmit_ready_int(uint8_t uart_ID);
void uart_inc_counter2();
//void uart_transmit_byte(uint8_t uart_ID,unsigned char val);
uint8_t parse_API(uint8_t* Frame_data,uint8_t size,uint8_t* msg_start,uint8_t* msg_size,
 uint8_t* status);
uint8_t get_signal_strength();

// input stream functions
void init_RXstream1();
uint8_t count_RXBuffer1();
uint8_t read_API_Buffer1(uint8_t* Buffer,uint8_t* size,uint8_t max,uint8_t timeout);
uint8_t get_RX1_out();
uint8_t get_RX1_in();

// output stream
uint8_t transmit_API_frame_TX1(uint8_t* Data,uint8_t size,uint8_t ack,uint8_t opt,
 uint8_t timeout);

// xbee 
void uart_set_dest_addr(uint8_t* addr);

#endif
