#include "timer.h"

const uint32_t MAX = 0x0000FFFF;

static volatile uint8_t tccr1b;
static volatile uint16_t time_Hword = 0x0000;
static volatile uint32_t pwm_TOP = 0xFFFF;
static volatile uint32_t presc1 = F_CPU;

ISR(TIMER1_OVF_vect){
	time_Hword++;	
}

uint16_t init_timer0(uint8_t prescaler){ 
	uint16_t scale;
	switch(prescaler){
		case CLK0_1: scale = 0x0001;break;
		case CLK0_8: scale = 0x0008;break;
		case CLK0_64: scale = 0x0040;break;
		case CLK0_256: scale = 0x0100;break;
		case CLK0_1024: scale = 0x0400;break;
		default: scale = 0x0001;
	}
	//TCCR0 &= ~(0x07);
	//TCCR0 += prescaler;
	TCCR0 = CLK0_1024;
	//return scale;	
	return 0x0400;
}

void start_timer0(void){
	TIMSK |= (1<<TOIE0);
	TCNT0 = 0x00;
	sei();
}

void stop_timer0(void){
	TIMSK &= ~(1<<TOIE0);
}

void timer1_mode(uint8_t mode){
	if (mode==NORMAL_MODE){
		TCCR1A &= ~((1<<WGM10) | (1<<WGM11));
		TCCR1B &= ~((1<<WGM12) | (1<<WGM13));		
	}
}

void timer1_prescaler(uint8_t pre){
	uint8_t mask;
	mask = (1<<CS12) | (1<<CS11) | (1<<CS10);
	mask ^= 0xFF;
	TCCR1B |= pre;
	
	switch(pre){
		case CLK_1: presc1 = (uint32_t)F_CPU;break;
		case CLK_8: presc1 = (uint32_t)F_CPU / 8UL;break;
		case CLK_64: presc1 = (uint32_t)F_CPU / 64UL;break;
		case CLK_256: presc1 = (uint32_t)F_CPU / 256UL;break;
		case CLK_1024: presc1 = (uint32_t)F_CPU / 1024UL;break;
	}
}

void timer1_stop(void){
	tccr1b = TCCR1B;
	TCCR1B &= ~((1<<CS12) | (1<<CS11) | (1<<CS10));
}

void timer1_continue(void){
	TCCR1B = tccr1b;
}

uint16_t get_time_Lword(void){
	uint16_t res;
	uint8_t temp = SREG;
	cli();
	res = TCNT1;
	sei();
	SREG = temp;
}

void set_time_Lword(uint16_t time){	
	uint8_t temp = SREG;
	cli();
	TCNT1 = time;
	sei();
	SREG = temp;
}

void set_output_comp(uint16_t val,uint8_t Timer){
	uint8_t temp = SREG;
	cli();
	if (Timer==TIMER_16A) OCR1A = val;
	if (Timer==TIMER_16B) OCR1B = val;	
	sei();
	SREG = temp;
}

float get_time_sec(void){
	uint8_t temp = SREG;	
	cli();
	union time32 current_time;
	current_time.t16.L = get_time_Lword();
	current_time.t16.H = time_Hword;
	
	float res = (float)current_time.t32/presc1;
	sei();
	SREG = temp;
	return res;
}

void reset_timer32(void){
	set_time_Lword(0);
	time_Hword = 0;
}

void start_timer32(void){
	timer1_mode(NORMAL_MODE);
	timer1_prescaler(CLK_64);
	TIMSK |= (1<<TOIE1);
	reset_timer32();
	sei();
}

uint16_t get_time_Hword(void){
	return time_Hword;
}

void set_pwm_channel_A(volatile uint16_t fracA){
	volatile uint8_t temp = SREG;
	cli();
	/*uint16_t offset = 0x1FFE;
	offset /= 0x05;
	fracA /= 0x05;
	if (fracA<offset) fracA = 0x0; else fracA -= offset;*/
	fracA /= 0x0A;
	
	uint32_t count = pwm_TOP*fracA/0xFFFF;
	OCR1A = (uint16_t)count;
	SREG = temp;
}

void set_pwm_channel_B(uint16_t fracB){
	uint8_t temp = SREG;
	cli();	
	
	uint32_t count = pwm_TOP*fracB/0xFFFF;
	OCR1B = (uint16_t)count;
	SREG = temp;
}

uint8_t x1_less_x2_16(uint16_t x1,uint16_t x2){
	union time16 X1 = (union time16)x1;
	union time16 X2 = (union time16)x2;
	if (X1.t8.H<X2.t8.H) return 0x01;
	else if (X1.t8.H>X2.t8.H) return 0x00;
	else if (X1.t8.L<X2.t8.L) return 0x01;
	else return 0x00;
}

void init_pwm_motor_control(uint16_t freq,uint16_t fracA,uint16_t fracB){		
	uint8_t temp = SREG;
	cli();
	
	uint32_t max = (uint32_t)2*F_CPU / freq;
	uint8_t presc_mode = CLK_1;
	if (max>MAX){
		max >>= 3U;
		presc_mode = CLK_8;
	}
	else goto Init;
	if (max>MAX){
		max >>= 3U;
		presc_mode = CLK_64;
	}
	else goto Init;
	if (max>MAX){
		max >>= 2U;
		presc_mode = CLK_256;
	}
	else goto Init;
	if (max>MAX){
		max >>= 2U;
		presc_mode = CLK_1024;
	}
		
Init:
	pwm_TOP = max;//(1<<COM1A0) | (1<<COM1B0) |
	TCCR1A = (1<<COM1A1) | (1<<COM1B1) | (1<<WGM11);
	TCCR1B = (1<<WGM13) | (1<<WGM12);
	timer1_prescaler(presc_mode);
	
	
	ICR1 = (uint16_t) max;
	set_pwm_channel_A(fracA);
	set_pwm_channel_B(fracB);	
	
	TCNT1 = 0x0000;	
	SREG = temp;
}






