#ifndef F_CPU
#define F_CPU 7372800UL
#endif

//#define TRANSMIT_OK 0x01

#include "BMA220.h"

//const int nsize = 10;
//volatile int16_t acc_x,acc_y,acc_z;
//union u16 Data;
static volatile uint8_t sreg;
static volatile uint8_t Int_flag = 0x00;
//static volatile uint8_t UART_receive_flag = 0x00;

/*void init_master(void){
	DDRB = (1<<SPI_MOSI) | (1<<SPI_SCK) | (1<<SPI_SS); // MOSI,SS and clock output
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<CPOL) | (1<<CPHA) | (1<<SPR1); 
														/* set as master, 
														enable SPI, clock/16
														* SPI mode 3*/
	/*PORTB |= (1<<SPI_SS) | (1<<SPI_MISO);
}*/

/////////////////// interrupt handlers ////////////////////////////////


#if defined(__AVR_ATmega8__)

ISR(INT0_vect){	
	Int_flag = 0x01;
}

#elif defined(__AVR_ATmega64__)

ISR(INT6_vect){	
	Int_flag = 0x01;
}

#endif


/////////////////////// functions /////////////////////////////////////

uint8_t bma220_new_data_present(void){
	if (Int_flag==0x01){
		Int_flag = 0x00;
		return 0x01;
	}
	else return 0x00;
}

void transmit_byte(uint8_t byte){
	uint8_t mask = 0b10000000;
	do{		
		COMPORT &= ~(1<<SPI_SCK);		
		if (byte & mask) COMPORT |= (1<<SPI_MOSI); else COMPORT &= ~(1<<SPI_MOSI);
		NOP_6;
		COMPORT |= (1<<SPI_SCK);
		NOP_6;		
		mask >>= 1;
	}while(mask>0);
}

uint8_t receive_byte(void){
	volatile uint8_t mask = 0b10000000;
	volatile uint8_t res = 0b00000000;
	do{
		COMPORT &= ~(1<<SPI_SCK);	
		NOP_6;	
		if (COMPORTPIN & (1<<SPI_MISO)) res |= mask; else res &= ~(mask);	
		COMPORT |= (1<<SPI_SCK);
		NOP_6;		
		mask >>= 1;
	}while(mask>0);
	return res;
}

void bma220_write8(uint8_t addr,uint8_t val){
	COMPORT &= ~(1<<SPI_SS);
	addr &= ~(1<<RW_BIT);	
	NOP_12;
	transmit_byte(addr);
	transmit_byte(val);
	COMPORT |= (1<<SPI_SS);	
}

uint8_t bma220_read8(uint8_t addr){
	COMPORT &= ~(1<<SPI_SS);
	addr |= (1<<RW_BIT);	
	NOP_12;
	transmit_byte(addr);
	uint8_t res = receive_byte();
	COMPORT |= (1<<SPI_SS);	
	return res;
}

int16_t bma220_read10bit(uint8_t addr){
	union int2x8_t res;
	COMPORT &= ~(1<<SPI_SS);
	addr |= (1<<RW_BIT);	
	NOP_12;
	transmit_byte(addr);
	res.val8[0] = receive_byte();
	res.val8[1] = receive_byte();
	COMPORT |= (1<<SPI_SS);
	
	if (res.val8[1] & (1<<7)){
		res.val16 >>= 6;
		res.val8[1] |= 0xFC;
		/*res.val8[1]--;
		res.val8[1] ^= 0xFF;
		res.val16 >>= 6;
		res.val16--;
		res.val16 ^= 0xFFFF;*/
	}
	else{
		res.val16 >>= 6;
	}
	return res.val16;
}


/*uint8_t SPI_transceive(uint8_t val){
	SPDR = val;
	while(!(SPSR & (1<<SPIF))){};
	PORTB |= (1<<SPI_SS);
	return SPDR;	
}

void SPI_write(uint8_t addr,uint8_t val){
	PORTB &= ~(1<<SPI_SS);
	addr &= ~(1<<RW_BIT);
	SPI_transceive(addr);
	SPI_transceive(val);
	PORTB |= (1<<SPI_SS);
}

uint8_t SPI_read(uint8_t addr){
	PORTB &= ~(1<<SPI_SS);
	addr |= (1<<RW_BIT);
	SPI_transceive(addr);
	uint8_t res = SPI_transceive(0x00);
	PORTB |= (1<<SPI_SS);
	return res;
}*/

void bma220_initSPI4(void){
	COMPORTDDR |= (1<<SPI_MOSI) | (1<<SPI_SCK) | (1<<SPI_SS); // MOSI,SS and clock output
	COMPORTDDR &= ~(1<<SPI_MISO); 							 // MISO input
	COMPORT |= (1<<SPI_SS) | (1<<SPI_SCK) | (1<<SPI_MOSI);
	COMPORT &= ~(1<<SPI_MISO);
	
	uint8_t addr = 0x15;
	uint8_t val = (1<<SPI4);
	bma220_write8(addr,val);
}

uint8_t bma220_getID(void){		
	sreg = SREG;
	cli();
	uint8_t res = bma220_read8(0x00);	
	SREG = sreg;
	return res;	
}

void bma220_set_periodic_wakeup(uint8_t period){
	uint8_t reg = bma220_read8(0x15);	
	reg = (reg & 0b11111000)+period;
	bma220_write8(0x15,reg);
}

void bma220_clear_periodic_wakeup(void){
	uint8_t reg = bma220_read8(0x15);	
	reg &= ~(1<<0);
	bma220_write8(0x15,reg);
}

void bma220_read10bit_mode(uint8_t mode){
	sreg = SREG;
	cli();
	uint8_t reg = bma220_read8(0x15);	
	if (mode) reg |= (1<<3); else reg &= ~(1<<3);
	bma220_write8(0x15,reg);
	SREG = sreg;
}

void bma220_set_range(uint8_t range){
	sreg = SREG;
	cli();
	uint8_t reg = bma220_read8(0x14);	
	reg &= ~((1<<3) | (1<<4));
	reg += range;
	bma220_write8(0x14,reg);
	SREG = sreg;
}

void bma220_set_bandwidth(uint8_t freq){
	sreg = SREG;
	cli();
	uint8_t reg = bma220_read8(0x14);	
	reg &= ~((1<<0) | (1<<1) | (1<<2));
	reg += freq;
	bma220_write8(0x14,reg);
	SREG = sreg;
}

void bma220_enable_newdata_int(uint8_t source){
	sreg = SREG;
	cli();	
	Int_flag = 0x00;	
	
#if defined(__AVR_ATmega8__)
	switch(source){
		case INT_0:
			MCUCR |= (1<<ISC00) | (1<<ISC01);
			GICR |= (1<<INT0);
			break;
		case INT_1:
			MCUCR |= (1<<ISC10) | (1<<ISC11);
			GICR |= (1<<INT1);
			break;
	}
#elif (__AVR_ATmega64__)
	switch(source){
		case INT_0:
			DDRD &= ~(1<<PD0);
			EICRA |= (1<<ISC01) | (1<<ISC00);
			EIMSK |= (1<<INT0);
			break;
		case INT_1:
			DDRD &= ~(1<<PD1);
			EICRA |= (1<<ISC11) | (1<<ISC10);
			EIMSK |= (1<<INT1);
			break;
		case INT_2:
			DDRD &= ~(1<<PD2);
			EICRA |= (1<<ISC21) | (1<<ISC20);
			EIMSK |= (1<<INT2);
			break;
		case INT_3:
			DDRD &= ~(1<<PD3);
			EICRA |= (1<<ISC31) | (1<<ISC30);
			EIMSK |= (1<<INT3);
			break;
		case INT_4:
			DDRE &= ~(1<<PE4);
			EICRB |= (1<<ISC41) | (1<<ISC40);
			EIMSK |= (1<<INT4);
			break;
		case INT_5:
			DDRE &= ~(1<<PE5);
			EICRB |= (1<<ISC51) | (1<<ISC50);
			EIMSK |= (1<<INT5);
			break;
		case INT_6:
			DDRE &= ~(1<<PE6);
			EICRB |= (1<<ISC61) | (1<<ISC60);
			EIMSK |= (1<<INT6);
			break;
		case INT_7:
			DDRE &= ~(1<<PE7);
			EICRB |= (1<<ISC71) | (1<<ISC70);
			EIMSK |= (1<<INT7);
			break;
	}
#endif

	uint8_t reg = bma220_read8(0x15);	
	reg |= (1<<5);
	bma220_write8(0x15,reg);	
	
	SREG = sreg;
}

void bma220_disable_newdata_int(uint8_t source){	

	#if defined(__AVR_ATmega8__)
	switch(source){
		case INT_0: GICR &= ~(1<<INT0);break;
		case INT_1: GICR &= ~(1<<INT1);break;
	}
#elif (__AVR_ATmega64__)
	switch(source){
		case INT_0: EIMSK &= ~(1<<INT0);break;
		case INT_1: EIMSK &= ~(1<<INT1);break;
		case INT_2: EIMSK &= ~(1<<INT2);break;
		case INT_3: EIMSK &= ~(1<<INT3);break;			
		case INT_4:	EIMSK &= ~(1<<INT4);break;			
		case INT_5:	EIMSK &= ~(1<<INT5);break;			
		case INT_6:	EIMSK &= ~(1<<INT6);break;		
		case INT_7:	EIMSK &= ~(1<<INT7);break;			
	}
#endif

	sreg = SREG;
	cli();
	
	uint8_t reg = bma220_read8(0x15);	
	reg &= ~(1<<5);
	bma220_write8(0x15,reg);
	SREG = sreg;
}

void bma220_sleep(void){
	bma220_write8(0x0A,0x01);
}

void bma220_wakeup(void){
	bma220_write8(0x0A,0x00);
	_delay_ms(1);
}

void bma220_soft_reset(void){
	bma220_write8(0x0A,0x02);
	_delay_us(100);
	uint8_t reg = bma220_read8(0x15);
	reg |= (1<<SPI4);
	bma220_write8(0x15,reg);
}

void bma220_reset_int_flag(void){
	bma220_write8(0x0A,0x40);
}

int16_t bma220_acc_x_high_prec(void){
	sreg = SREG;
	cli();
	int16_t res = bma220_read10bit(0x02);
	SREG = sreg;
	return res;
}

int16_t bma220_acc_y_high_prec(void){
	sreg = SREG;
	cli();
	int16_t res = bma220_read10bit(0x04);
	SREG = sreg;
	return res;
}

int16_t bma220_acc_z_high_prec(void){
	sreg = SREG;
	cli();
	int16_t res = bma220_read10bit(0x06);
	SREG = sreg;
	return res;
}

uint8_t bma220_acc_x_low_prec(void){
	bma220_read10bit_mode(NO_BLOCK);
	sreg = SREG;
	cli();
	uint8_t res = bma220_read8(0x03);
	bma220_read10bit_mode(BLOCK);
	SREG = sreg;
	return res;
}

uint8_t bma220_acc_y_low_prec(void){
	bma220_read10bit_mode(NO_BLOCK);
	sreg = SREG;
	cli();
	uint8_t res = bma220_read8(0x05);
	bma220_read10bit_mode(BLOCK);
	SREG = sreg;
	return res;
}

uint8_t bma220_acc_z_low_prec(void){
	bma220_read10bit_mode(NO_BLOCK);
	sreg = SREG;
	cli();
	uint8_t res = bma220_read8(0x07);
	bma220_read10bit_mode(BLOCK);
	SREG = sreg;
	return res;
}

void bma220_get_acc_data(int16_t* Data,uint8_t int_source){
	bma220_disable_newdata_int(int_source);	
    Data[0] = bma220_acc_x_high_prec();     
    Data[1] = bma220_acc_y_high_prec();       
    Data[2] = bma220_acc_z_high_prec();
    bma220_enable_newdata_int(int_source);
}

/*uint8_t SPI_init_sensor(void){
	uint8_t addr = 0x15;
	uint8_t val = (1<<SPI4);
	SPI_write(addr,val);	
	addr = 0x00;
	uint8_t res = SPI_read(addr);
	return res;
}*/

/*void send_acceleration_data(uint8_t int_source){
	
	//if (int_source==INT_0) GICR &= ~(1<<INT0);
	//if (int_source==INT_1) GICR &= ~(1<<INT1);
	* 
	
	//PORTD &= ~(1<<PD5);
		
	uint8_t len,ack,dummy;	
	char Val[nsize];
	char Buffer[32];
	float time;
			
	bma220_disable_newdata_int();	
	
    acc_x = bma220_acc_x_high_prec();     
    acc_y = bma220_acc_y_high_prec();       
    acc_z = bma220_acc_z_high_prec();
        
    Buffer[0] = 't';
    Buffer[1] = '\0';        
                    
    time = get_time_sec();
    single_to_hex_string(Val,*(uint32_t*)&time);
    string_append(Buffer,Val,nsize);        
        
    Val[0] = 'x';
    Val[1] = '\0';
    string_append(Buffer,Val,nsize);
        
    to_string16(Val,acc_x);
    string_append(Buffer,Val,nsize);        
        
    Val[0] = 'y';
    Val[1] = '\0';
    string_append(Buffer,Val,nsize);
       
    to_string16(Val,acc_y);
    string_append(Buffer,Val,nsize);
        
    Val[0] = 'z';
    Val[1] = '\0';
    string_append(Buffer,Val,nsize);
        
    to_string16(Val,acc_z);
    string_append(Buffer,Val,nsize);
        
    len = string_length(Buffer,0x40);
    
    //uart_disable_receive_int();
    
    uint8_t sreg = SREG;
    cli();
    uart_string_transmit(Buffer,len+1);
    ack = uart_receive_byte(0xFFFF);
    
    //if (UCSRA & (1<<RXC)) dummy = UDR;
    SREG = sreg;
    
    if (ack==0x03){
		if (PORTD & (1<<PD6)) PORTD &= ~(1<<PD6); else PORTD |= (1<<PD6);
	}        
    //if (PORTD & (1<<PD5)) PORTD &= ~(1<<PD5); else PORTD |= (1<<PD5);  
    
    bma220_enable_newdata_int(int_source);
    //uart_enable_receive_int();
       
    //if (int_source==INT_0) GICR |= (1<<INT0);
	//if (int_source==INT_1) GICR |= (1<<INT1);   
	if (PORTD & (1<<PD5)) PORTD &= ~(1<<PD5); else PORTD |= (1<<PD5);  
	UCSRA &= ~(1<<RXC);
}

uint8_t receive_value(uint16_t* Data){
	union u16 data;
	uint8_t ack;
	uint16_t i = 0;
	uint16_t timeout = 0x00;
	uint8_t sreg = SREG;
	cli();
		
	data.t8[1] = UDR;	
	//while (!(UCSRA & (1<<RXC)) && i<timeout){i++;};
	data.t8[0] = UDR;
	
	uart_transmit_byte(data.t8[0]);		// high byte
	uart_transmit_byte(data.t8[1]);		// low byte
	
	ack = uart_receive_byte(0xFFFF);
	if (ack==TRANSMIT_OK){ 		
		if (PORTD & (1<<PD4)) PORTD &= ~(1<<PD4); else PORTD |= (1<<PD4);	
	}
	*Data = data.t16;
	UART_receive_flag = 0x00;
	SREG = sreg;
	return ack;
}



ISR(USART_RXC_vect){
	UART_receive_flag = 0x01;	
}*/

/*int main(){
	
    DDRD = ~(1<<PD2); // Set all the pins of PortD except PD2 as output
    PORTD = 0xFF; // Turns off LEDs
    
    DDRC = 0xFF;	// Port C output
    PORTC = 0xFF;	// activate pull-ups
     
    bma220_initSPI4();
    _delay_ms(1000);
    
    uint8_t Chip_ID = bma220_getID();
    //PORTD = 0xFF-Chip_ID;
    
    _delay_ms(1000);
    
    enable_async_usart();
    uart_enable_receive_int();
    
    start_timer32();
    
    bma220_enable_newdata_int(INT_0);
    
    twi_init_master(0x02,0x00);
         
    Data.t16 = 0x8000;
    
    sei();
        
    while (1) {		
		if (Inf_flag){
			send_acceleration_data(INT_0);					
		}
		if (UART_receive_flag){
			if (receive_value(&(Data.t16))==TRANSMIT_OK) twi_master_transmit_byte(0x02,Data.t8,2);
		}
				
		
	
		//if (PORTD & (1<<PD7)) PORTD &= ~(1<<PD7); else PORTD |= (1<<PD7);
		_delay_ms(1);
    }

}*/
