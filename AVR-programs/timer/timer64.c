#include "timer64.h"


// wgm_mode must hae the bit structure (0,0,0,0,WGM3,WGM2,WGM1,WGM0)
void set_wave_form_generation_mode_13(uint8_t timer_ID,uint8_t wgm_mode){
	const uint8_t wgm_mask_A = ((1<<WGM11) | (1<<WGM10));
	const uint8_t wgm_mask_B = ((1<<WGM13) | (1<<WGM12));
	
	volatile uint8_t wgm_A = wgm_mode & wgm_mask_A;
	volatile uint8_t wgm_B = ((wgm_mode>>2) & wgm_mask_A)<<3;
		
	if (timer_ID==TIMER1){
		TCCR1A &= ~(wgm_mask_A);
		TCCR1A |= wgm_A;
		TCCR1B &= ~(wgm_mask_B);
		TCCR1B |= wgm_B;
	}
	else if (timer_ID==TIMER3){
		TCCR3A &= ~(wgm_mask_A);
		TCCR3A |= wgm_A;
		TCCR3B &= ~(wgm_mask_B);
		TCCR3B |= wgm_B;	
	}	
} 

void set_timer_output_mode_13(uint8_t timer_ID,uint8_t channel,uint8_t out_mode){
	volatile uint8_t mode = 0x00;
	switch(out_mode){
		case DISCONNECT: mode = 0x00;break;
		case TOGGLE_ON_MATCH: mode = 0x01; break;
		case CLEAR_ON_MATCH: mode = 0x02; break;
		case SET_ON_MATCH: mode = 0x03;break;
	}
	
	volatile uint8_t mask = 0b00000011;
	switch(channel){
		case CHANNEL_A: 
			mode <<= COM1A0;
			mask <<= COM1A0;
			break;
		case CHANNEL_B: 
			mode <<= COM1B0;
			mask <<= COM1B0;
			break;
		case CHANNEL_C:
			mode <<= COM1C0;
			mask <<= COM1C0;
			break; 
	}
	
	if (timer_ID==TIMER1){
		TCCR1A &= ~mask;
		TCCR1A |= mode;
	}
	else if (timer_ID==TIMER3){
		TCCR3A &= ~mask;
		TCCR3A |= mode;
	}
}
	

void timer0_init(uint8_t mode,uint8_t OC0_mode,uint8_t cmp_val,uint8_t presc){
	const uint8_t presc_mask = (1<<CS02) | (1<<CS01) | (1<<CS00);
	
	switch(mode){
		case NORMAL_MODE: TCCR0 &= ~((1<<WGM00) | (1<<WGM01));break;
		case CTC_MODE: TCCR0 &= ~(1<<WGM00);TCCR0 |= (1<<WGM01);break; 
		case PWM_MODE: TCCR0 |= (1<<WGM00); TCCR0 &= ~(1<<WGM01);break;
		case FAST_PWM_MODE: TCCR0 |= ((1<<WGM00) | (1<<WGM01));break;
	}
	
	if (OC0_mode!=DISCONNECT) DDRB |= (1<<PB4);
	
	switch(OC0_mode){
		case DISCONNECT:  TCCR0 &= ~((1<<COM00) | (1<<COM01));break;
		case CLEAR_ON_MATCH: TCCR0 &= ~(1<<COM00);TCCR0 |= (1<<COM01);break; 
		case TOGGLE_ON_MATCH: TCCR0 |= (1<<COM00); TCCR0 &= ~(1<<COM01);break;
		case SET_ON_MATCH: TCCR0 |= ((1<<COM00) | (1<<COM01));break;
	}
	
	TCCR0 &= ~presc_mask;
	TCCR0 |= presc;
	
	OCR0 = cmp_val;
}

void timer_init_13(uint8_t timer_ID,uint8_t wgm_mode,uint8_t presc){
	const uint8_t presc_mask = ((1<<CS10) | (1<<CS11) | (CS12));
	
	set_wave_form_generation_mode_13(timer_ID,wgm_mode);
	
	if (timer_ID==TIMER1){
		TCCR1B &= ~presc_mask;
		TCCR1B |= presc; 
	}
	else if (timer_ID==TIMER3){
		TCCR3B &= ~presc_mask;
		TCCR3B |= presc; 
	}
}

void timer_channel_settings_13(uint8_t timer_ID,uint8_t channel
 ,uint8_t out_mode,uint16_t cmp_val){
	
	volatile uint8_t temp = SREG;
	cli();
	
	if (out_mode!=DISCONNECT){
		if (timer_ID==TIMER1){		
			switch(channel){
				case CHANNEL_A:  DDRB |= (1<<PB5);break;
				case CHANNEL_B:  DDRB |= (1<<PB6);break;
				case CHANNEL_C:  DDRB |= (1<<PB7);		// take care of OC2, they share 
			}											// the pin -> modulation
		}
		else if (timer_ID==TIMER3){		
			switch(channel){
				case CHANNEL_A:  DDRE |= (1<<PE3);break;
				case CHANNEL_B:  DDRE |= (1<<PE4);break;
				case CHANNEL_C:  DDRE |= (1<<PE5);		
			}	
		}		
	}
	
	set_timer_output_mode_13(timer_ID,channel,out_mode);
	
	switch(channel){
		case CHANNEL_A: 
			if (timer_ID==TIMER1) OCR1A = cmp_val;
			if (timer_ID==TIMER3) OCR3A = cmp_val;
			break;
		case CHANNEL_B: 
			if (timer_ID==TIMER1) OCR1B = cmp_val;
			if (timer_ID==TIMER3) OCR3B = cmp_val;
			break;
		case CHANNEL_C: 
			if (timer_ID==TIMER1) OCR1C = cmp_val;
			if (timer_ID==TIMER3) OCR3C = cmp_val;		
	}
	
	SREG = temp;
}													

void timer_channel_cmpval_13(uint8_t timer_ID,uint8_t channel,uint16_t cmp_val){
	
	volatile uint8_t temp = SREG;
	cli();
	
	switch(channel){
		case CHANNEL_A: 
			if (timer_ID==TIMER1) OCR1A = cmp_val;
			if (timer_ID==TIMER3) OCR3A = cmp_val;
			break;
		case CHANNEL_B: 
			if (timer_ID==TIMER1) OCR1B = cmp_val;
			if (timer_ID==TIMER3) OCR3B = cmp_val;
			break;
		case CHANNEL_C: 
			if (timer_ID==TIMER1) OCR1C = cmp_val;
			if (timer_ID==TIMER3) OCR3C = cmp_val;		
	}
	
	SREG = temp;
	
}
