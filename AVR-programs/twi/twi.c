#include "twi.h"

static volatile uint8_t twi_error_code = 0x01;
//static volatile uint8_t twi_int_flag = 0x00;
static volatile uint8_t sreg;

void twi_init_master(uint8_t bitrate,uint8_t prescaler){
	TWBR = bitrate;
	TWSR &= TWSR_MASK;
	TWSR += prescaler;
	// freq twi clk = F_CPU/(16+2*TWBR*4^prescaler)
	//TWCR &= ~((1 << TWSTO) | (1 << TWEN));
	//TWCR |= (1 << TWEN);
}

uint8_t twi_get_errcode(){
	return twi_error_code;
}

void twi_init_slave(uint8_t ID){
	TWAR = (ID<<1);
}

void twi_start_master(){
	TWCR = ((1<<TWINT) | (1<<TWEN) | (1<<TWSTA)) & (~(1<<TWSTO));	
}

void twi_start_slave(){
	// interrupt based
	TWCR = ((1<<TWINT) | (1<<TWEA) | (1<<TWEN) | (1<<TWIE)) & (~((1<<TWSTA) | (1<<TWSTO)));	
}

void __attribute__((optimize("O0"))) twi_stop_master(){	
	TWCR = ((1<<TWINT) | (1<<TWEN) | (1<<TWSTO)) & (~(1<<TWSTA));
	TWCR &= ~((1<<TWEN) | (1<<TWSTO));
	TWCR |= (1<<TWEN);
}

/*void __attribute__((optimize("O0"))) twi_reset_slave(){	
	TWCR = (1<TWINT) | (1<<TWEA) | (1<<TWEN) | (1<<TWSTA);
}*/

__attribute__((always_inline)) static inline void twi_wait_for_TWINT(void){
	while (!(TWCR & (1<<TWINT))){};
} 

uint8_t twi_timed_wait_for_TWINT(){	
	volatile uint16_t i = 0;
	while ((!(TWCR & (1<<TWINT))) & (i<TWI_TIMEOUT)){
		i++;	
	};
	if (i<TWI_TIMEOUT) return SUCCESS; else return FAILED;
	/*else{
		twi_stop_master();
		SREG = sreg;
		return FAILED;	
	}*/
}

uint8_t twi_check(uint8_t status_code){
	if ((TWSR & TWSR_MASK) != status_code){		
		twi_error_code = TWSR & TWSR_MASK;
		return FAILED;
	}
	else return SUCCESS;
}

uint8_t twi_get_status(){
	return (TWSR & TWSR_MASK);
}

/*uint8_t twi_master_transmit_byte(uint8_t addr,uint8_t* Bytes,uint8_t size){	
	sreg = SREG;
	cli();
	
	// send start
	twi_start_master();
	twi_wait_for_TWINT();
	
	// check start
	if (twi_check(TWI_MA_START_OK)==FAILED){
		SREG = sreg;
		return FAILED;	
	}
			
	// send slave address + write
	addr <<= 1;	
	TWDR = addr & (~(1<<TWI_RWBIT));
	TWCR = (1<<TWINT) | (1<<TWEN);
	twi_wait_for_TWINT();
	
	// check slave Ok
	if (twi_check(TWI_SL_WRITE_OK)==FAILED){		
		SREG = sreg;
		return FAILED;	
	}
	
	//if (PORTD & (1<<PD7)) PORTD &= ~(1<<PD7); else PORTD |= (1<<PD7);
	
	int8_t i;
	for (i=0;i<size;i++){
		// send data
		TWDR = Bytes[i];
		TWCR = (1<<TWINT) | (1<<TWEN);
		twi_wait_for_TWINT();
	
		// check data received
		if (twi_check(TWI_MT_DATA_TRANSMIT_OK)==FAILED){
			SREG = sreg;
			return FAILED;	
		}
	}
	
	// send stop 
	twi_stop();
	
	SREG = sreg;
	return SUCCESS;
}

uint8_t twi_slave_receive_byte(uint8_t* byte){	
	uint8_t status;		
		
	// wait until address is called
	twi_start_slave();
	twi_wait_for_TWINT();	
	
	sreg = SREG;
	cli();			
		
	// check adress Ok
	if (twi_check(TWI_SL_ADDRESS_RECEIVE_OK)==FAILED){
		SREG = sreg;
		return FAILED;			
	}		
	
	uint8_t i = 0;
	do{
		// wait for data
		TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);
		twi_wait_for_TWINT();
	
		// check if data transmission Ok, if yes read data
		status = TWSR & TWSR_MASK;
		if (status==TWI_SL_DATA_OK){
			byte[i] = TWDR;	
			i++;
		}
		else if (status!=TWI_SL_STOP_OK){
			SREG = sreg;
			return FAILED;			
		}
	}while(status!=TWI_SL_STOP_OK);
	
	SREG = sreg;
	return SUCCESS;
}*/

uint8_t twi_master_transmit(uint8_t addr,uint8_t* Buffer,uint8_t size){	
	int8_t i,status,res;
	
	sreg = SREG;
	cli();
	
	res = SUCCESS;
	addr <<= 1;	
	i = 0;
	do{
		// send start
		twi_start_master();
		if (twi_timed_wait_for_TWINT()==FAILED) goto MRETRY;	
	
		// check start
		if (twi_check(TWI_MA_START_OK)==FAILED) goto MRETRY;	
			
		// send slave address + write
		TWDR = addr & (~(1<<TWI_RWBIT));
		MASTER_TRANSMIT
		if (twi_timed_wait_for_TWINT()==FAILED) goto MRETRY;	
		status = TWSR & TWSR_MASK;		
		
		// check slave response
		if (status!=TWI_MA_WRITE_OK){
	MRETRY:	twi_stop_master();
			i++;
		}	
	}while(status!=TWI_MA_WRITE_OK && i<TWI_MASTER_RETRIES);
	if (i==TWI_MASTER_RETRIES) {res = 0xF1;goto MA_TRA_ERROR;};

	// send size
	TWDR = size;
	MASTER_TRANSMIT
	if (twi_timed_wait_for_TWINT()==FAILED) goto MA_TRA_ERROR;
	if (twi_check(TWI_MA_TRANSMIT_OK)==FAILED){res = 0xF2;goto MA_TRA_ERROR;}
	
	// send data
	i = 0;
	do{
		TWDR = *Buffer;
		Buffer++;
		MASTER_TRANSMIT
		if (twi_timed_wait_for_TWINT()==FAILED) goto MA_TRA_ERROR;
		if (twi_check(TWI_MA_TRANSMIT_OK)==FAILED){res = 0xF3;goto MA_TRA_ERROR;}
		i++;
	}while(i<size);
	
	// send stop 
	MASTER_TRANSMIT_FINISH
	
#if defined(__AVR_ATmega64__)
	if (PINE & (1<<PE3)) PORTE &= ~(1<<PE3); else PORTE |= (1<<PE3);	
#endif
	
	SREG = sreg;
	return SUCCESS;
	
MA_TRA_ERROR:
	twi_stop_master();
	SREG = sreg;
	return res;
}

// first byte is size
uint8_t twi_slave_receive(uint8_t* Buffer,uint8_t* size,uint8_t max){	
	uint8_t i;
	
	sreg = SREG;
	cli();	
	
	// acquire size
	SLAVE_RECEIVE_ACK
	if (twi_timed_wait_for_TWINT()==FAILED) {SREG = sreg;return FAILED;}
	if (twi_check(TWI_SL_DATA_RECEIVE_OK)==FAILED) {SREG = sreg;return 0xF1;}	
	*size = TWDR;
	
	// read data
	i = 0;
	do{
		// wait for data
		SLAVE_RECEIVE_ACK
		if (twi_timed_wait_for_TWINT()==FAILED) {SREG = sreg;return FAILED;}
		if (twi_check(TWI_SL_DATA_RECEIVE_STOP)==SUCCESS) break;
		else if (twi_check(TWI_SL_DATA_RECEIVE_OK)==FAILED) {SREG = sreg;return 0xF2;}
		// read data				
		*Buffer = TWDR;	
		if (i<max-1) Buffer++;
		i++;
	}while(i<*size);
	
#if defined(__AVR_ATmega8__)
	if (PINB & (1<<PB1)) PORTB &= ~(1<<PB1); else PORTB |= (1<<PB1);			
#endif	
	
	SREG = sreg;
	if (*size==i) return SUCCESS; else return FAILED;
}

// first byte is packet size, interrupt based
uint8_t twi_slave_transmit(uint8_t* Buffer,uint8_t size){	
	uint8_t i,res;		
		
	sreg = SREG;
	cli();		
	
#if defined(__AVR_ATmega8__)
	PORTB |= (1<<PB1);		
#endif	
	
	// check adress Ok
	if (twi_check(TWI_SL_ADDRESS_TRANSMIT_OK)==FAILED) {res = 0xF1;goto SLA_TRA_ERROR;}	

	// transmit size
	TWDR = size;
	SLAVE_TRANSMIT_ACK
	if (twi_timed_wait_for_TWINT()==FAILED) goto SLA_TRA_ERROR;
	if (twi_check(TWI_SL_DATA_TRANSMIT_OK)==FAILED) {res = 0xF2;goto SLA_TRA_ERROR;}	

	// send bytes
	i = 0;
	while(i<size-1){
		TWDR = Buffer[i];	
		SLAVE_TRANSMIT_ACK
		if (twi_timed_wait_for_TWINT()==FAILED) goto SLA_TRA_ERROR;
		if (twi_check(TWI_SL_DATA_TRANSMIT_OK)==FAILED) {res = 0xF3;goto SLA_TRA_ERROR;}	
		i++;
	}

	// send last byte
	TWDR = Buffer[i];
	SLAVE_TRANSMIT_NACK
	if (twi_timed_wait_for_TWINT()==FAILED) goto SLA_TRA_ERROR;
	if (twi_check(TWI_SL_DATA_TRANSMIT_FINISHED_OK)==FAILED) {res = 0xF4;goto SLA_TRA_ERROR;}

	// goto normal state
	SLAVE_TRANSMIT_FINISH	
	
#if defined(__AVR_ATmega8__)
	PORTB &= ~(1<<PB1);		
#endif	
	
	SREG = sreg;
	return SUCCESS;
	
SLA_TRA_ERROR:

#if defined(__AVR_ATmega8__)
	PORTB &= ~(1<<PB1);	
#endif	

	twi_start_slave();
	SREG = sreg;
	if (res==SUCCESS) res = FAILED;
	return res;
}

// first byte is packet size
uint8_t twi_master_receive(uint8_t addr,uint8_t* Buffer,uint8_t* size,uint8_t max){	
	uint8_t i,status,res;	
	
	sreg = SREG;
	cli();	
		
	res = SUCCESS;
	addr <<= 1;	
	i = 0;
	
#if defined(__AVR_ATmega64__)
	PORTE |= (1<<PE3);
	//PORTE |= (1<<PE4);
#endif
	
	do{
		// issue start condition
		twi_start_master();
		if (twi_timed_wait_for_TWINT()==FAILED) goto RETRY;
			
		// check if start Ok
		if (twi_check(TWI_MA_START_OK)==FAILED) goto RETRY;
	
		// send slave address + read
		TWDR = addr | (1<<TWI_RWBIT);
		TWCR |= (1<<TWINT) | (1<<TWEN);
		if (twi_timed_wait_for_TWINT()==FAILED) goto RETRY;		
		
		// ckeck slave response
		status = TWSR & TWSR_MASK;		
		if (status!=TWI_MA_SLAVE_ADDR_OK){			
	RETRY:	//MASTER_RECEIVE_FINISH		
			twi_stop_master();
			i++;			
		}		
	}while(status!=TWI_MA_SLAVE_ADDR_OK && i<TWI_MASTER_RETRIES);
	if (i==TWI_MASTER_RETRIES) {res = 0xF2;goto RETURN_ERROR;}
	
	//#if defined(__AVR_ATmega64__)
	//PORTE &= ~(1<<PE4);
	//#endif
	
	// acquire data packet size
	MASTER_RECEIVE_ACK
	if (twi_timed_wait_for_TWINT()==FAILED) goto RETURN_ERROR;	
	if (twi_check(TWI_MA_DATA_OK_ACK)==FAILED) {res = 0xF3;goto RETURN_ERROR;}
	*size = TWDR;
	
	// receive data
	i = 0;
	while(i<*size-1){
		MASTER_RECEIVE_ACK
		if (twi_timed_wait_for_TWINT()==FAILED) goto RETURN_ERROR;	
		if (twi_check(TWI_MA_DATA_OK_ACK)==FAILED) {res = 0xF4;goto RETURN_ERROR;}		
		*Buffer = TWDR;
		if (i<max-1) Buffer++;
		i++;
	}
	
	// receive last byte
	MASTER_RECEIVE_NACK
	if (twi_timed_wait_for_TWINT()==FAILED) goto RETURN_ERROR;	
	if (twi_check(TWI_MA_DATA_OK_NACK)==FAILED) {res = 0xF5;goto RETURN_ERROR;}			
	*Buffer = TWDR;
	Buffer++;
	
	// send stop condition
	MASTER_RECEIVE_FINISH		

#if defined(__AVR_ATmega64__)
	PORTE &= ~(1<<PE3);
#endif
	
	SREG = sreg;
	return SUCCESS;
	
RETURN_ERROR:

#if defined(__AVR_ATmega64__)
	PORTE &= ~(1<<PE3);
#endif

	if (res==SUCCESS) res = FAILED;
	//MASTER_RECEIVE_FINISH
	twi_stop_master();
	SREG = sreg;
	return res;
}		
	
