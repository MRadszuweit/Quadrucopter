#include "timer8.h"

void init_timer2(uint8_t mode,uint8_t output,uint8_t presc){
	TCCR2 = 0x00;
	TCCR2 |= mode;
	TCCR2 |= output;
	TCCR2 |= presc;	
}

void timer2_set_comp_value(uint8_t comp){
	OCR2 = comp;
}

void start_timer2(uint8_t presc){
	TCNT2 = 0x00;
	TCCR2 &= ~TIMER2_PRESC_MASK;
	TCCR2 |= presc;
}

void stop_timer2(){
	TCCR2 &= ~TIMER2_PRESC_MASK;
}

void timer2_external_clock(uint8_t mode){
	if (mode==YES) ASSR |= (1<<AS2); else ASSR &= ~(1<<AS2);
}
