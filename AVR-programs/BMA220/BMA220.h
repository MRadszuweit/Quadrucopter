#ifndef BMA_220
#define BMA_220

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
//#include <string.h>
//#include "../usart/usart.h"
//#include "../timer/timer.h"
//#include "../twi/twi.h"

// SPI pins on ATMEGA8
#if defined(__AVR_ATmega8__)

#define COMPORT PORTB
#define COMPORTDDR DDRB
#define COMPORTPIN PINB
#define SPI_SS PB2
#define SPI_MOSI PB3 
#define SPI_MISO PB4
#define SPI_SCK PB5

#elif (__AVR_ATmega64__)

#define COMPORT PORTD
#define COMPORTDDR DDRD
#define COMPORTPIN PIND
#define SPI_SS PD7
#define SPI_MOSI PD5 
#define SPI_MISO PD4
#define SPI_SCK PD6

#else

#warning "device not known"

#endif 

// BMA220 definitions
#define BMA220_PRESCALER 2
#define RW_BIT 7			// read/write bit, read: 1, write 0
#define BMA220_ID 0x02

// ext. interrupts
#define INT_0 0
#define INT_1 1
#define INT_2 2
#define INT_3 3
#define INT_4 4
#define INT_5 5
#define INT_6 6
#define INT_7 7

//range
#define RANGE_2g 0x00
#define RANGE_4g 0x08
#define RANGE_8g 0x10

//periodic_wakeup
#define PAUSE_20ms 0x01
#define PAUSE_80ms 0x03
#define PAUSE_320ms 0x05
#define PAUSE_2560ms 0x07

//bandwidth (moving average filter)
#define BW_25Hz 0x00
#define BW_50Hz 0x01
#define BW_100Hz 0x02
#define BW_190Hz 0x03
#define BW_375Hz 0x04
#define BW_750Hz 0x05
#define BW_1500Hz 0x06

//shadow bit (MSB only reading possible)
#define BLOCK 0x00				// after reading LSB, further read blocked until
								// MSB read
#define NO_BLOCK 0x01			// no block

//0x15 bits
#define SPI4 7				// SPI 4-wire mode
#define SDB 3				// read only LSB
#define NOP_3 _delay_loop_1(1*BMA220_PRESCALER)
#define NOP_6 _delay_loop_1(2*BMA220_PRESCALER)
#define NOP_9 _delay_loop_1(3*BMA220_PRESCALER)
#define NOP_12 _delay_loop_1(4*BMA220_PRESCALER)

// types
union int2x8_t{
	uint16_t val16;
	uint8_t val8[2];
};

/*union u16{
	uint16_t t16;
	uint8_t t8[2];
};*/

// functions
void bma220_initSPI4(void);
uint8_t bma220_getID(void);
void bma220_set_periodic_wakeup(uint8_t period);
void bma220_clear_periodic_wakeup(void);
void bma220_set_range(uint8_t range);
void bma220_set_bandwidth(uint8_t freq);
void bma220_read10bit_mode(uint8_t mode);
void bma220_sleep(void);
void bma220_wakeup(void);
void bma220_soft_reset(void);
void bma220_reset_int_flag(void);
void bma220_enable_newdata_int(uint8_t source);
void bma220_disable_newdata_int(uint8_t source);
int16_t bma220_acc_x_high_prec(void);
int16_t bma220_acc_y_high_prec(void);
int16_t bma220_acc_z_high_prec(void);
uint8_t bma220_acc_x_low_prec(void);
uint8_t bma220_acc_y_low_prec(void);
uint8_t bma220_acc_z_low_prec(void);
uint8_t bma220_new_data_present(void);
void bma220_get_acc_data(int16_t* Data,uint8_t int_source);

#endif
