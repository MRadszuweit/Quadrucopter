#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#define ADC_ADDR 0x02
#define TWI_BUFFER_SIZE 0x20
#define AVE_WINDOW_SIZE 0x40
#define ADC_INTERVAL_US 100

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdint.h>
#include <util/delay.h>
#include <util/atomic.h>
#include "../twi/twi.h"
#include "../ADC/adc.h"
#include "../timer/timer8.h"
#include "../math/math.h"


// ADC0 nick
// ADC1 geir
// ADC2 roll

void record_time();
void update_buffers();

/////////////////////// constants /////////////////////////////////////

/////////////////// global variables //////////////////////////////////

static volatile uint8_t sreg;

//static volatile float cov_measure;
//static volatile float cov_external;

static volatile t32 time;
static volatile t16 geir,roll,nick;
static volatile float32 gain1,gain2;
static volatile float32 mean_roll,mean_nick,mean_geir;
static volatile uint8_t TWI_transmit_buffer[TWI_BUFFER_SIZE];
static volatile uint8_t TWI_receive_buffer[TWI_BUFFER_SIZE];

//static volatile uint16_t Geir[AVE_WINDOW_SIZE];
//static volatile uint16_t Roll[AVE_WINDOW_SIZE];
//static volatile uint16_t Nick[AVE_WINDOW_SIZE];
//static volatile uint8_t window_pos;

/////////////////// interrupt handlers ////////////////////////////////

ISR(TWI_vect){
	t16 conv;
	
	PORTD &= ~(1<<PD4);	
	//if (PIND & (1<<PD7)) PORTD &= ~(1<<PD7); else PORTD |= (1<<PD7);
	ADC_disable();
	PORTD |= (1<<PD7);	
	uint8_t size = 0x00;	
	uint8_t status = twi_get_status();
	if (status==TWI_SL_ADDRESS_RECEIVE_OK){
		twi_slave_receive((uint8_t*)TWI_receive_buffer,&size,TWI_BUFFER_SIZE);
	}
	else if (status==TWI_SL_ADDRESS_TRANSMIT_OK){
		record_time();
		size = 0x0E;
		uint8_t* It = (uint8_t*)TWI_transmit_buffer;
		
		*(It++) = (uint8_t)'G';
		conv.val16 = float32_to_int16(mean_geir);
		*(It++) = conv.val8[1];
		*(It++) = conv.val8[0];
		
		*(It++) = (uint8_t)'R';
		conv.val16 = float32_to_int16(mean_roll);
		*(It++) = conv.val8[1];
		*(It++) = conv.val8[0];
		
		*(It++) = (uint8_t)'N';
		conv.val16 = float32_to_int16(mean_nick);
		*(It++) = conv.val8[1];
		*(It++) = conv.val8[0];			
		
		*(It++) = (uint8_t)'T';
		*(It++) = time.val8[3];
		*(It++) = time.val8[2];
		*(It++) = time.val8[1];
		*(It++) = time.val8[0];		
		
		twi_slave_transmit((uint8_t*)TWI_transmit_buffer,size);
	}
	twi_start_slave();
	ADC_enable();	
	PORTD &= ~(1<<PD7);
}

ISR(ADC_vect){
	
	TWCR = (1<<TWEN) | (1<<TWINT);					// disconnnect slave, no ack
	
	PORTD &= ~(1<<PD4);
	
	ADC_disable();
	uint8_t channel = ADMUX & 0x0F;
	
	//geir.val8[0] = channel;
	//geir.val8[1] = 0x00;
	switch(channel){
		case ADC_CH0: nick.val16 = ADC_read_high_prec();break;
		case ADC_CH1: geir.val16 = ADC_read_high_prec();break;
		case ADC_CH2: roll.val16 = ADC_read_high_prec();break;
	}
	
	//if (channel==0x02) if (PIND & (1<<PD7)) PORTD &= ~(1<<PD7); else PORTD |= (1<<PD7);
	
	if (channel==ADC_CH2){
		update_buffers();	
		channel = ADC_CH0; 
	}
	else channel++;
	ADC_set_channel(channel);	
	
	ADC_enable();	
	twi_start_slave();							// reconnect slave		
}

/////////////////////// functions /////////////////////////////////////

/*void init_buffers(){
	uint8_t i;
	window_pos = 0x00;
	for (i=0;i<AVE_WINDOW_SIZE;i++){
		Geir[i] = 0x0000;
		Roll[i] = 0x0000;
		Nick[i] = 0x0000;	
	}
}*/

uint16_t get_mean(volatile uint16_t* Buffer,uint8_t size){
	uint8_t i;
	
	sreg = SREG;
	cli();
	
	uint32_t sum = 0x00000000;
	for (i=0;i<size;i++) sum += Buffer[i];
	sum >>= 6U;													// only if size=2^6
	SREG = sreg;
	return (uint16_t)sum;
}

uint16_t get_variance(volatile uint16_t* Buffer,uint8_t size){
	uint8_t i;
	uint32_t d;
	
	sreg = SREG;
	cli();
	
	uint16_t mean = get_mean(Buffer,size);
	uint32_t sum = 0x00000000;
	for (i=0;i<size;i++){
		d = (uint32_t)Buffer[i]-mean;
		d *= d;
		sum += d;
	}
	sum /= size-1;
	uint16_t res = sqrt32(sum);	
	SREG = sreg;
	
	return res;
}

void update_buffers(){
	sreg = SREG;
	cli();
	
	// moving average filter
	/*Geir[window_pos] = geir.val16;
	Roll[window_pos] = roll.val16;
	Nick[window_pos] = nick.val16;
	if (window_pos+1==AVE_WINDOW_SIZE){
		//mean_geir.val16 = get_mean(Geir,AVE_WINDOW_SIZE);
		//mean_roll.val16 = get_mean(Roll,AVE_WINDOW_SIZE);
		//mean_nick.val16 = get_mean(Nick,AVE_WINDOW_SIZE);		
		window_pos = 0;
	}
	else window_pos++;*/	
	
	// kalman filter 
	float32 G = init_float32(geir.val16);
	mult_float32((float32*)&G,(float32*)&gain1);
	mult_float32((float32*)&mean_geir,(float32*)&gain2);
	add_float32((float32*)&mean_geir,(float32*)&G);	
	
	float32 R = init_float32(roll.val16);
	mult_float32((float32*)&R,(float32*)&gain1);
	mult_float32((float32*)&mean_roll,(float32*)&gain2);
	add_float32((float32*)&mean_roll,(float32*)&R);
	
	float32 N = init_float32(nick.val16);
	mult_float32((float32*)&N,(float32*)&gain1);
	mult_float32((float32*)&mean_nick,(float32*)&gain2);
	add_float32((float32*)&mean_nick,(float32*)&N);
	
	/*float y;
	float cov_pred = covariance+cov_external;
	float gain = cov_pred/(cov_pred+cov_measure);	
	covariance = (1.-gain)*cov_pred;
	
	y = (float)geir.val16-mean_geir;
	mean_geir += gain*y;	
	
	y = (float)roll.val16-mean_roll;
	mean_roll += gain*y;	
	
	y = (float)nick.val16-mean_nick;
	mean_nick += gain*y;*/	
	
	//PORTD &= ~(1<<PD7);
	
	ADCSRA |= (1<<ADIF);					// clear ADC interrupt
	SREG = sreg;
}


void init_ports(){
	
	DDRB = (1<<PB1);
	PORTB = ~(1<<PB1);	
	
	DDRC = 0x00;		
	PORTC = (1<<PC3) | (1<<PC4) | (1<<PC5);		
	
	DDRD = 0xFF;
	PORTD = 0x00;
}	

void sleep_mode_ADC(){
	MCUCR |= (1<<SM0);								// ADC noise reduction mode 
	MCUCR &= ~((1<<SM1) | (1<<SM2));
}

void record_time(){
	sreg = SREG;
	cli();
	stop_timer2();
	time.val32 += TCNT2;
	start_timer2(TIMER2_CLK_1024);
	SREG = sreg;
}

void init_ADC(){
	
	sreg = SREG;
	cli();
	
	geir.val16 = 0x0000;
	roll.val16 = 0x0000;
	nick.val16 = 0x0000;
	
	mean_geir = init_float32(0);
	mean_roll = init_float32(0);
	mean_nick = init_float32(0);
	
	//cov_measure = 5000.0;//25.0;				// variance estimated 5
	//cov_external = 10.0;
	
	// exact for smaller r
	/*uint16_t r = 1000;
	uint16_t q = 10;	
	uint16_t covariance = sqrt32((uint32_t)q*(q+4*r));
	covariance -= q;
	covariance >>= 1;
	uint16_t h = covariance+r;
	gain1 = init_float32_rational(covariance,h);*/
	
	// approx for large r
	uint32_t r = 1000000;
	uint32_t q = 10;	
	uint16_t h = sqrt32(r);
	uint16_t SQRTq = sqrt32(q);
	
	h += SQRTq;
	gain1 = init_float32_rational(SQRTq,h);	
	gain2 = init_float32(1);
	sub_float32((float32*)&gain2,(float32*)&gain1);
	
	SREG = sreg;
	
	// ADC settings
	ADC_set_reference(ADC_REF_INTERNAL_Vcc);
	ADC_enable(ADC_SINGLE_SAMPLE_MODE,ADC_RIGHT_ALIGNED,ADC_PRESC_128);
	
	// enable conversion interrupt
	ADCSRA |= (1<<ADIE);
	ADC_set_channel(0x00);	
	
	// enable ADC
	ADC_enable();	
}	

int main(){
	
	//init_buffers();
	
	init_ports();
	
	init_ADC();
	
	sleep_mode_ADC();
	
	init_timer2(TIMER2_NORMAL,TIMER2_DISCONNECT,TIMER2_CLK_STOP);
	
	sei();
	
	twi_init_slave(ADC_ADDR);
	twi_start_slave();
	
	time.val32 = 0;	
	
	uint8_t i = 0x00;
	start_timer2(TIMER2_CLK_1024);
	while(1){	
		//if (PIND & (1<<PD7)) PORTD &= ~(1<<PD7); else PORTD |= (1<<PD7);
		ADC_start_conversion();
		ADC_start_conversion();
		ADC_start_conversion();
		
		record_time();
		_delay_us(ADC_INTERVAL_US);								
	}
}
