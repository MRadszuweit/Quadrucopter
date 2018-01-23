#ifndef F_CPU
#define F_CPU 7372800UL
#endif

#define SLAVE
//#define MASTER

#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "../timer/timer.h"
#include "../twi/twi.h"

#define SL_ADDRESS 0x02

#define STANDBY 40000
#define FULLSPEED 65535

uint16_t MIN_SAVE EEMEM = 0x0000;
uint16_t MAX_SAVE EEMEM = 0xFFFF;

uint8_t trans;
static volatile uint8_t sreg;
static volatile uint16_t temp,scale;
static volatile uint16_t current_speed = 0x0;
static volatile uint16_t new_speed = 0;
static volatile uint16_t Presc0 = 0x1;

static volatile int16_t increment = 0x0;

//static volatile uint8_t timer0_count = 0x00;
//static volatile uint8_t timer0_max = 0x00;
static volatile uint8_t timer0_intflag = 0x00;

ISR(INT0_vect){		
	temp = ADC;
	eeprom_write_word(&MIN_SAVE,temp);
	if (PORTC & (1<<PC0)) PORTC &= ~(1<<PC0); else PORTC |= (1<<PC0);
}

ISR(INT1_vect){	
	temp = ADC;
	eeprom_write_word(&MAX_SAVE,temp);
	if (PORTC & (1<<PC0)) PORTC &= ~(1<<PC0); else PORTC |= (1<<PC0);
}

ISR(TIMER0_OVF_vect){	
	timer0_intflag = 0x01;
	if (PORTD & (1<<PD6)) PORTD &= ~(1<<PD6); else PORTD |= (1<<PD6);
}

uint16_t get_ADC(void){
	ADCSRA |= (1<<ADSC);
	while (ADCSRA & (1<<ADSC)){};
	uint16_t res = ADC;
	return res;
}

int main(){

    DDRD = ~((1<<PD2) | (1<<PD3)); // PortD as output excpt pin 2,3
    PORTD = 0xFF; // pullups activated
    
    DDRC = (1<<PC0); // all pins input except pin PC0
    PORTC = ~(1<<PC0); // pullups activated except PC0
    
    DDRB = (1<<PB1) | (1<<PB2); 
    PORTB = 0xFF;
    
    // init switches
    MCUCR |= (1<<ISC00) | (1<<ISC01) | (1<<ISC10) | (1<<ISC11);
	GICR |= (1<<INT0) | (1<<INT1);	
    
    // start ADC
    ADMUX = (1<<ADLAR) | (1<<MUX1) | (1<<MUX0);  // channel 3,left adjusted,
    											 // ext. ref
    ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); // | (1<<ADSC) 
    										// presc. 128
    										
    twi_init_slave(SL_ADDRESS);
    						
    volatile uint16_t min_load = eeprom_read_word(&MIN_SAVE); 
    volatile uint16_t max_load = eeprom_read_word(&MAX_SAVE);
    scale = (max_load-min_load)/0xFF;
    						
    init_pwm_motor_control(0x32,min_load,min_load);		// 50Hz
	
	uint8_t dt = 0x64;	
	uint8_t active = 0x00;
	union u16 speed;
	speed.t16 = STANDBY;	
	set_pwm_channel_A(speed.t16);	
	current_speed = speed.t16;
	Presc0 = init_timer0(CLK0_1024);
	
	sei();	
	
    while(1){	
		if (twi_slave_receive_byte(speed.t8)==SUCCESS){
			sreg = SREG;
			cli();			
			int32_t z;				
			new_speed = speed.t16;
			if (new_speed>current_speed){
				z = (int32_t)new_speed;
				z -= (int32_t)current_speed;
			}
			else{
				z = (int32_t)current_speed;
				z -= (int32_t)new_speed;
			}			
			z *= (int32_t) Presc0;
			z /= (int32_t) dt;							// dt: 1/100 sec.
			int32_t n = F_CPU / 0x6400;					// f/(100*256)
			z /= n;
			increment = (int16_t)z;
			if (PORTD & (1<<PD7)) PORTD &= ~(1<<PD7); else PORTD |= (1<<PD7);			
			SREG = sreg;
			start_timer0();
			active = 0x01;
		};
		while (active){
			if (timer0_intflag==0x01){
				sreg = SREG;
				cli();
				if (new_speed>current_speed){
					if ((new_speed-current_speed)<increment){
						increment = 0x0;
						current_speed = new_speed;
						stop_timer0();
						active = 0x00;					
					}
					else current_speed += increment;				
				}
				else{
					if ((current_speed-new_speed)<increment){
						increment = 0x0;
						current_speed = new_speed;
						stop_timer0();
						active = 0x00;
					}
					else current_speed -= increment;					
				}
				set_pwm_channel_A(current_speed);
				timer0_intflag = 0x00;
				SREG = sreg;	
			}
		}
			
		/*sreg = SREG;
		cli();
		temp = get_ADC();					
		set_pwm_channel_A(speed.t16);	
		SREG = sreg;		*/
										  
	    _delay_ms(1);
    }
}
