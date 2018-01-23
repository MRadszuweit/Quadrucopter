#ifndef XBEE
#define XBEE

#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <time.h>
#include <math.h>
#include <math/math.h>

// important: Start process as root ! 

#define STRINGIZE(x) #x
#define DEF_BAUD B57600
#define TIMEOUT 50
#define BUFFER_SIZE 512

#define NO 0
#define YES 1

#define OK "OK\r"
#define ERROR "ERROR\n"
#define ASCII_START 2
#define ASCII_END 3

#define TRANSMIT_FAILED 0
#define TRANSMIT_OK 1
#define CLEARED 0
#define SET 1
#define FAILED 0
#define SUCCESS 1

#define DATA_READ 2
#define DATA_SEND 3
#define MAX_BUFFER 2048

#define BAUD_1200 0x00
#define BAUD_2400 0x01
#define BAUD_4800 0x02
#define BAUD_9600 0x03
#define BAUD_19200 0x04
#define BAUD_38400 0x05
#define BAUD_57600 0x06
#define BAUD_115200 0x07

#define NO_PARITY 0x00
#define EVEN_PARITY 0x01
#define ODD_PARITY 0x02
#define FORCED_H_PARITY 0x03
#define FORCED_L_PARITY 0x04

#define API_OFF 0x00
#define API_ON 0x01 
#define API_ESC 0x02
#define API_ESCAPE 0x7D
#define API_DELIM 0x7E
#define API_XON 0x11
#define API_XOFF 0x13
#define API_XOR_MASK 0x20
#define API_OK 0x00
#define API_ERROR 0x01
#define API_INVALID_COMMAND 0x02
#define API_INVALID_PARAMETER 0x03
#define API_QUIET 0x00
#define API_RESPONSE 0x01
#define API_NO_CHANGE 0x00
#define API_APPLY_CHANGES 0x02

#define API_TRANSMIT_REQUEST_64BIT 0x00
#define API_AT_COMMAND 0x08
#define API_REMOTE_AT_COMMAND 0x17
#define API_RECEIVE_TRANSMISSION 0x80
#define API_AT_COMMAND_RESPONSE 0x88
#define API_AT_REMOTE_COMMAND_RESPONSE 0x97


#define PL_10dB 0x00
#define PL_12dB 0x01
#define PL_14dB 0x02
#define PL_16dB 0x03
#define PL_18dB 0x04

#define IO_DISABLED 0x00
#define IO_BUTTON 0x01
#define IO_FLOW 0x01
#define IO_RSSI 0x01
#define IO_INDICATOR 0x01
#define IO_ADC 0x02
#define IO_PWM 0x02
#define IO_DIG_INPUT 0x03
#define IO_LOW 0x04
#define IO_HIGH 0x05



typedef union INT16{
	int16_t t16;
	uint8_t t8[2];
}int16;

typedef union INT32{
	int32_t t32;
	uint16_t t16[2];
	uint8_t t8[4];
}int32;


/*typedef struct IO_INFO{
	unsigned short digital_mask;
	unsigned char Analag_mask;
	
}*/


#endif
