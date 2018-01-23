
#include "uart64.h"

#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)
#define BUFFER_SIZE 0xFF

void clean_RXBuffer1();

/////////////////////////// global variables /////////////////////

static volatile uint8_t errno = 0x00;
static volatile uint8_t signal_strength = 0x00;

static volatile uint8_t RX_Buffer1[BUFFER_SIZE];
static volatile uint8_t RX_buff1_out = 0;
static volatile uint8_t RX_buff1_in = 0;

static volatile uint8_t TX_Buffer1[BUFFER_SIZE];
//static volatile uint8_t TX_buff1_out = 0;
//static volatile uint8_t TX_buff1_in = 0;

static volatile uint8_t Dest_Addr[8] = {0,0,0,0,0,0,0,0};

//static volatile uint8_t use_timer2 = NO;
static volatile uint8_t counter2 = 0x00;
static volatile uint8_t delim_flag = NO;

static volatile uint8_t sreg;

/////////////////// interrupt handlers ////////////////////////////////


ISR(TIMER2_COMP_vect){
	TCNT2 = 0x00;
	counter2++;
}

ISR(USART1_RX_vect){
	char c = UDR1;
	RX_Buffer1[RX_buff1_in] = c;
	if (c==API_DELIM) delim_flag = YES;
	RX_buff1_in++;
	if (RX_buff1_in==BUFFER_SIZE-1){
		PORTE |= (1<<PE2);							// use PE2 as RTS indicator 
		clean_RXBuffer1();
		PORTE &= ~(1<<PE2);
	}	
	//if (PINE && (1<<PE3)) PORTE &= ~(1<<PIN3); else PORTE |= (1<<PE3);
}

/////////////////////// functions /////////////////////////////////////

void uart_enable_receive_int(uint8_t uart_ID){
	if (uart_ID==UART0) UCSR0B |= (1<<RXCIE0);
	if (uart_ID==UART1) UCSR1B |= (1<<RXCIE1);
}

void uart_enable_transmit_complete_int(uint8_t uart_ID){
	if (uart_ID==UART0) UCSR0B |= (1<<TXCIE0);
	if (uart_ID==UART1) UCSR1B |= (1<<TXCIE1);
}

void uart_enable_transmit_ready_int(uint8_t uart_ID){
	if (uart_ID==UART0) UCSR0B |= (1<<UDRIE0);
	if (uart_ID==UART1) UCSR1B |= (1<<UDRIE1);
}

void uart_set_dest_addr(uint8_t* addr){
	uint8_t i;
	for (i=0;i<8;i++) Dest_Addr[i] = addr[i];
}

void uart_inc_counter2(){
	counter2++;
}

uint8_t get_signal_strength(){
	return signal_strength;
}

void start_timer2_1ms(){
	// configured for f_CPU=7.3728 Mhz and prescaler 64
	// counts approx 1ms until match oocurs	
	counter2 = 0x00;
	TCNT2 = 0x00;
	TCCR2 = (1<<WGM21) | (1<<CS21) | (1<<CS20);
	OCR2 = 0x73; // ~115.2	
	TIMSK |= (1<<OCIE2);
	sei();
}

void stop_timer2_1ms(){
	TCCR2 &= ~((1<<CS22) | (1<<CS21) | (1<<CS20));
	TIMSK &= ~(1<<OCIE2);
}

uint16_t char_to_hex16(uint8_t* HexString){				// length must be 4 digits
	sreg = SREG; 										// capital letters
	cli();
	
	uint8_t x;
	uint16_t y; 
	uint16_t res = 0x0000;
	
	x = HexString[0];
	if (x<HEX_A) y = (uint16_t)x-HEX_0; else y = (uint16_t)x-0x37; 
	y <<= 12U;
	res += y;
	
	x = HexString[1];
	if (x<HEX_A) y = (uint16_t)x-HEX_0; else y = (uint16_t)x-0x37; 
	y <<= 8U;
	res += y;
	
	x = HexString[2];
	if (x<HEX_A) y = (uint16_t)x-HEX_0; else y = (uint16_t)x-0x37; 
	y <<= 4U;
	res += y;
	
	x = HexString[3];
	if (x<HEX_A) y = (uint16_t)x-HEX_0; else y = (uint16_t)x-0x37; 	
	res += y;
	
	SREG = sreg;	
	return res;
}

void insert_in_frame(volatile uint8_t* Buffer,uint8_t c,uint8_t* pos,uint8_t* sum){
	if (c==API_DELIM | c==API_ESCAPE | c==API_XON | c==API_XOFF){		
		Buffer[*pos] = API_ESCAPE;
		(*pos)++;
		Buffer[*pos] = c^API_XOR_MASK;
		(*pos)++;
	}
	else {
		Buffer[*pos] = c;
		(*pos)++;
	}
	(*sum) += c;
}

uint8_t compute_check_sum(volatile uint8_t* Buffer,uint8_t start,uint8_t end){
	uint8_t i;
	volatile uint16 sum;
	
	sreg = SREG;
	cli();
	
	sum.t16 = 0x0000;
	for (i=start;i<=end;i++) sum.t16 += Buffer[i];
	uint8_t res = sum.t8[0];
	res = 0xFF-res;
	
	SREG = sreg;
	return res;
} 

uint8_t test_check_sum(uint8_t* Buffer,uint8_t start,uint8_t end,uint8_t cs){
	uint8_t i;
	volatile uint16 sum;
	
	sreg = SREG;
	cli();
	
	sum.t16 = 0x0000;
	for (i=start;i<=end;i++) sum.t16 += Buffer[i];
	uint8_t check_sum = cs+sum.t8[0];	
	
	SREG = sreg;
	if (check_sum==0xFF) return SUCCESS; else return FAILED;	
}

void enable_uart(uint8_t uart_ID,uint8_t frame_size,uint8_t stop_bits
 ,uint8_t parity,uint8_t speed){	
	 
	// ports
	DDRD &= ~(1<<PD2);		// RXD input
	PORTD |= (1<<PD2);
	DDRD |= (1<<PD3);		// TXD output
	
	// baud rate
	volatile uint32 uart_presc;
	uart_presc.t32 = F_CPU+BAUD*8;
	uart_presc.t32 /= (uint32_t) 16*BAUD;
	uart_presc.t32 -= 1UL;		
	
	volatile uint8_t A = 0x00;
	volatile uint8_t B = 0x00;
	volatile uint8_t C = 0x00;
	
	//register UCSRA
	if (speed==UART_DOUBLE_SPEED) A |= (1<<U2X1);
	
	//register UCSRB
	B |= (1<<RXEN1) | (1<<TXEN1);
	if (frame_size>FRAME_8BIT) B |= (1<<UCSZ12);
	
	//register UCSRC
	if (parity==ODD_PARITY) C |= (1<<UPM11) | (1<<UPM10);
	if (parity==EVEN_PARITY) C |= (1<<UPM11);
	if (frame_size==FRAME_9BIT) C |= (1<<UCSZ11) | (1<<UCSZ10);
	else C |= (1<<frame_size);
	
	
	if (uart_ID==UART0){
		UCSR0A = A;
		UCSR0B = B;
		UCSR0C = C;
		UBRR0H = (uint8_t) uart_presc.t8[1];
		UBRR0L = (uint8_t) uart_presc.t8[0];	
	}
	if (uart_ID==UART1){
		UCSR1A = A;
		//UCSR1B = B;
		//UCSR1C = C;
		//UBRR1H = (uint8_t) uart_presc.t8.L1;
		//UBRR1L = (uint8_t) uart_presc.t8.L0;		
		
		UCSR1B = (1<<RXEN1) | (1<<TXEN1);	// enables receiver and transmitter
		UCSR1C = (1<<UCSZ1) | (1<<UCSZ0);	// no parity, one stop bit, 8 bit
		UBRR1H = 0x00;						// manuell seeting: 38400B @ 7372800Hz
		UBRR1L = 0x07;						// and single speed
	}
	
	PORTE &= ~(1<<PE2);						// set RTS low
}

void uart_transmit_byte(uint8_t uart_ID,unsigned char val){
	if (uart_ID==UART0){
		while (!(UCSR0A & (1<<UDRE0))){};
		UDR0 = val;
	}
	if (uart_ID==UART1){
		while (!(UCSR1A & (1<<UDRE1))){};
		UDR1 = val;
	}
}

uint8_t transmit_API_frame_TX1(uint8_t* Data,uint8_t size,uint8_t ack,uint8_t opt,
 uint8_t timeout){
	uint8_t i,len,v,pos;
	uint8_t sum,dummy;	
	
	if (PINE & (1<<PE7)) return FAILED;				// watch CTS from xbee (PE7)

	PORTB |= (1<<PB4);
													
	// create frame
	sum = 0x00;
	pos = 0;
	len = size+11;
	TX_Buffer1[pos++] = API_DELIM;
	TX_Buffer1[pos++] = 0x00;
	insert_in_frame(TX_Buffer1,len,&pos,&dummy);
	
	// start here with checksum
	insert_in_frame(TX_Buffer1,API_TRANSMIT_REQUEST_64BIT,&pos,&sum);
	insert_in_frame(TX_Buffer1,ack,&pos,&sum);		
	
	for (i=0;i<8;i++){
		v = Dest_Addr[i];
		insert_in_frame(TX_Buffer1,v,&pos,&sum);	
	}	
	
	insert_in_frame(TX_Buffer1,opt,&pos,&sum);		
	for (i=0;i<size;i++) insert_in_frame(TX_Buffer1,Data[i],&pos,&sum);	
	
	// insert checksum		
	sum = (uint8_t)0xFF-sum;
	insert_in_frame(TX_Buffer1,sum,&pos,&dummy);	
	
	// transmit
	start_timer2_1ms();
	for (i=0;i<pos;i++){			
		while (!(UCSR1A & (1<<UDRE1)) | (PINE & (1<<PE7))){		
			if (counter2>timeout){
				PORTB &= ~(1<<PB4);
				return FAILED;
			}
		};		
		UDR1 = TX_Buffer1[i];
		counter2 = 0x00;
		TCNT2 = 0x00;
	}
	stop_timer2_1ms();
	PORTB &= ~(1<<PB4);
	return SUCCESS;
}

void clean_RXBuffer1(){
	uint8_t i;
	sreg = SREG;
	uint8_t Dat[2];
	cli();
	
	//handle buffer overflow: delete at least 8 oldest chars but try 
	//to next delim	
	if (RX_buff1_out==0){
		for (i=8;i<RX_buff1_in;i++){			
			if (RX_Buffer1[i]==API_DELIM) break;			
		}
		RX_buff1_out = i;
	}
	
	// shift chars
	for (i=RX_buff1_out;i<RX_buff1_in;i++) RX_Buffer1[i-RX_buff1_out] = RX_Buffer1[i];
	
	//if (PINE & (1<<PE4)) PORTE &= ~(1<<PE4); else PORTE |= (1<<PE4);
	
	RX_buff1_in = RX_buff1_in-RX_buff1_out;
	RX_buff1_out = 0x00;
	SREG = sreg;
}

void reset_RXBuffer1(){
	sreg = SREG;
	cli();
	RX_buff1_in = 0x00;
	RX_buff1_out = 0x00;
	SREG = sreg;
}

uint8_t RXBuffer1_immediate_read(uint8_t* X){
	PORTC |= (1<<PC7);
	uint8_t status;
	sreg = SREG;
	cli();
	if (RX_buff1_in<=RX_buff1_out) status = FAILED;
	else{
		*X = RX_Buffer1[RX_buff1_out];
		RX_buff1_out++;
		if (*X==API_DELIM) delim_flag = NO;
		status = SUCCESS;
	}
	
	SREG = sreg;
	PORTC &= ~(1<<PC7);
	return status;
}

uint8_t RXBuffer1_timed_read(uint8_t* Buffer,uint8_t max,uint8_t timeout,uint8_t escaped){	
	uint8_t counter = 0x00;	
	uint8_t xor = NO;	
	
	start_timer2_1ms();
	
	if (escaped==YES){
		while(counter<max && counter2<timeout){
			if (RXBuffer1_immediate_read(Buffer)==SUCCESS){
				if (xor==YES){		
		  			(*Buffer) ^= API_XOR_MASK;
					xor = NO;
				}
				else if ((*Buffer)==API_ESCAPE) xor = YES;
				if (xor==NO){
					Buffer++;
					counter++;
				}
			}
			
			//if (SREG & (1<<7)) PORTG |= (1<<PG1); else PORTG &= ~(1<<PG1);
			sei();
		}
	}
	else{
		while(counter<max && counter2<timeout){
			if (RXBuffer1_immediate_read(Buffer)==SUCCESS){
				Buffer++;
				counter++;			
			}
		}
	}	
	
	stop_timer2_1ms();	
	return counter;
}

uint8_t count_RXBuffer1(){
	return RX_buff1_in-RX_buff1_out;
}

uint8_t get_RX1_in(){
	return RX_buff1_in;
}

uint8_t get_RX1_out(){
	return RX_buff1_out;
}

uint8_t get_uart64_errno(){
	return errno;
}

void init_RXstream1(){
	reset_RXBuffer1();
	uart_enable_receive_int(UART1);
}

uint8_t read_RX1Buffer_until_delim(uint8_t timeout){	
	uint8_t c;
	
	uint8_t status = FAILED;
	TIMSK |= (1<<OCIE2);
	start_timer2_1ms();
	
	while(counter2<timeout){
		if (RXBuffer1_immediate_read(&c)==SUCCESS){
			status = SUCCESS;
			break;
		}
	}
	
	stop_timer2_1ms();
	TIMSK &= ~(1<<OCIE2);
	return status;
}

void recover_special_chars(uint8_t* Buffer,uint8_t start,volatile uint8_t* end){	// end=size
	uint8_t j;
	uint8_t i =0;
	while (i<*end){
		if (Buffer[i]==API_ESCAPE){	
			for (j=i;j<(*end)-1;j++) Buffer[j] = Buffer[j+1];
			Buffer[i] ^= API_XOR_MASK;
			(*end)--;
		}
		i++;
	}	
}

uint8_t read_API_Buffer1(uint8_t* Buffer,uint8_t* size,uint8_t max,uint8_t timeout){
	uint8_t i,c,status,counter,length,pos,xor;
	uint8_t Data[2];
	
	*size = 0;
	uint8_t old = RX_buff1_out;	
	
	PORTE |= (1<<PE4);
	//PORTB |= (1<<PB4);
	
	// wait until delim arrived
	if(read_RX1Buffer_until_delim(timeout>>1)==FAILED){
		errno = ERR_BUFFER_EMPTY;
		goto READ_ERRROR;
	}
	
	// read frame length
	status = RXBuffer1_timed_read(Buffer,0x02,timeout,YES);
	if (status<0x02){
		errno = ERR_BROKEN_FRAME;
		goto READ_ERRROR;
	}
	if (Buffer[0]!=0x00){
		errno = ERR_FRAME_TOO_LARGE;
		goto READ_ERRROR;
	}
	length = Buffer[1];	
	if (length==0){
		errno = ERR_INCONSISTENT_LENGTH;
		goto READ_ERRROR;
	}
	
	// read $length characters
	status = RXBuffer1_timed_read(Buffer,length+1,timeout,YES);
	if (status<length+1){
		errno = ERR_INCONSISTENT_LENGTH;
		goto READ_ERRROR;
	}
	*size = length;
	
	//PORTB &= ~(1<<PB4);
	
	// test checksum	
	if (test_check_sum(Buffer,0,length-1,Buffer[length])==FAILED){
		errno = ERR_CHECK_SUM;		
	}
	
	PORTE &= ~(1<<PE4);		
	return SUCCESS;
	
READ_ERRROR:
	
	//RX_buff1_out = old;
	PORTE &= ~(1<<PE4);	
	//PORTB &= ~(1<<PB4);
	return FAILED;	
}

uint8_t parse_API(uint8_t* Frame_data,uint8_t size,uint8_t* msg_start,uint8_t* msg_size,
 uint8_t* status){	
	if (size==0) goto FINALIZE;
	uint8_t start = 0x00;
	uint8_t msg_type = Frame_data[0];
	switch(msg_type){
		case API_RECEIVE_TRANSMISSION: 
			signal_strength = Frame_data[9];
			start = 0x0B;			
			break;
		case API_TRANSMIT_STATUS: 
			start = 0x02;
			*status = Frame_data[2];
			break;
		case API_AT_COMMAND_RESPONSE:
			start = 0x05;
			*status = Frame_data[4];
		default:
			errno = ERR_UNKNOWN_MESSAGE;
	}
	if (start<size){
		*msg_start = start;
		*msg_size = size-start;
		Frame_data[size]='\0';
		return msg_type;
	}
	else{
FINALIZE:
		errno = ERR_BROKEN_FRAME;
		return API_BROKEN_FRAME;
	}
}

//uint8_t str_comp(
