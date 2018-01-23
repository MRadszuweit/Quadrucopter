#ifndef AVR_TWI
#define AVR_TWI

#include <avr/io.h>
#include <avr/interrupt.h>

#define TWI_TIMEOUT 0x04FF
#define TWI_MASTER_RETRIES 0x05

#define TWI_RWBIT 0x00
#define TWSR_MASK 0xF8
#define FAILED 0x00
#define SUCCESS 0x01

#define TWI_PRESC_1 0x00
#define TWI_PRESC_4 0x01
#define TWI_PRESC_16 0x02
#define TWI_PRESC_64 0x03

/*#define TWI_RSTART 0x10
#define TWI_MT_ARB_LOST 0x38*/

#define TWI_MA_START_OK 0x08
#define TWI_MA_WRITE_OK 0x18
#define TWI_MA_WRITE_ERROR 0x20
#define TWI_MA_TRANSMIT_OK 0x28
#define TWI_MA_TRANSMIT_ERROR 0x30

#define TWI_MA_SLAVE_ADDR_OK 0x40
#define TWI_MA_READ_ERROR 0x48
#define TWI_MA_DATA_OK_ACK 0x50
#define TWI_MA_DATA_OK_NACK 0x58

#define TWI_SL_ADDRESS_RECEIVE_OK 0x60
#define TWI_SL_DATA_RECEIVE_OK 0x80
#define TWI_SL_DATA_RECEIVE_STOP 0xA0

#define TWI_SL_ADDRESS_TRANSMIT_OK 0xA8
#define TWI_SL_DATA_TRANSMIT_OK 0xB8
#define TWI_SL_DATA_TRANSMIT_FINISHED_OK 0xC0

// master transmitter mode 
#define MASTER_TRANSMIT TWCR = ((1<<TWINT) | (1<<TWEN)) & (~((1<<TWSTA) | (1<<TWSTO)));
#define MASTER_TRANSMIT_FINISH TWCR = ((1<<TWINT) | (1<<TWEN) | (1<<TWSTO)) & (~(1<<TWSTA));

// master receiver mode 
#define MASTER_RECEIVE_ACK TWCR = ((1<<TWINT) | (1<<TWEN) | (1<<TWEA)) & (~((1<<TWSTA) | (1<<TWSTO)));
#define MASTER_RECEIVE_NACK TWCR = ((1<<TWINT) | (1<<TWEN)) & (~((1<<TWSTA) | (1<<TWSTO) | (1<<TWEA)));
#define MASTER_RECEIVE_FINISH TWCR = ((1<<TWINT) | (1<<TWSTO) | (1<<TWEN)) & (~(1<<TWSTA));

// slave receiver mode
#define SLAVE_RECEIVE_ACK TWCR = ((1<<TWEN) | (1<<TWINT) | (1<<TWEA)) & (~(1<<TWSTO));

// slave transmitter mode 
#define SLAVE_TRANSMIT_ACK TWCR = ((1<<TWINT) | (1<<TWEN) | (1<<TWEA)) & (~((1<<TWSTA) | (1<<TWSTO)));
#define SLAVE_TRANSMIT_NACK TWCR = ((1<<TWINT) | (1<<TWEN)) & (~((1<<TWSTA) | (1<<TWSTO) | (1<<TWEA)));
#define SLAVE_TRANSMIT_FINISH TWCR = ((1<<TWINT) | (1<<TWEA) | (1<<TWEN)) & (~((1<<TWSTA) | (1<<TWSTO)));

union u16{
	uint16_t t16;
	uint8_t t8[2];
};

void twi_init_master(uint8_t bitrate,uint8_t prescaler);
void twi_init_slave(uint8_t ID);
uint8_t twi_get_errcode();
uint8_t twi_get_status();
void twi_start_slave();

uint8_t twi_slave_transmit(uint8_t* Buffer,uint8_t size);
uint8_t twi_master_receive(uint8_t addr,uint8_t* Buffer,uint8_t* size,uint8_t max);
uint8_t twi_master_transmit(uint8_t addr,uint8_t* Buffer,uint8_t size);
uint8_t twi_slave_receive(uint8_t* Buffer,uint8_t* size,uint8_t max);


#endif 
