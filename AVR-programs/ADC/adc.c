
#include "adc.h"


#if defined(__AVR_ATmega8__)

void ADC_settings(uint8_t sample_mode,uint8_t alignment,uint8_t presc){
	
	// alignment of conversion result
	if (alignment==ADC_LEFT_ALIGNED) ADMUX |= (1<<ADLAR); else ADMUX &= ~(1<<ADLAR);
	// free running or single conversion sample moe
	if (sample_mode==ADC_FREE_RUNNING_MODE) ADCSRA |= (1<<ADFR); else  ADCSRA &= ~(1<<ADFR);
	// set ADC-clock prescaler
	ADCSRA &= ~((1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0));
	ADCSRA |= presc;
}

void ADC_set_reference(uint8_t ref){
	ADMUX &= ~((1<<REFS1) | (1<<REFS0));
	ADMUX |= ref;
}

void ADC_set_channel(uint8_t channel){
	ADMUX &= 0xF0;
	ADMUX |= channel;
}

void ADC_disable(){
	ADCSRA &= ~(1<<ADEN);
}

void ADC_enable(){
	ADCSRA |= (1<<ADEN);
}

// should not be called if ADLAR=0, reads 8bit
uint8_t ADC_read_low_prec(){
	if (ADMUX & (1<<ADLAR)) return ADCH; else return 0x00;
}

// reads 10bit
uint16_t ADC_read_high_prec(){
	t16 res;
	res.val8[0] = ADCL;
	res.val8[1] = ADCH;
	return res.val16;
}

uint16_t ADC_single_conversion_read(){
	ADCSRA |= (1<<ADSC) | (1<<ADEN);
	while(ADCSRA & (1<<ADSC)){};
	return ADC_read_high_prec();
}

uint8_t ADC_start_conversion(){
	ADCSRA |= (1<<ADSC) | (1<<ADEN) | (1<<ADIE);
	PORTD |= (1<<PD4);
	sei();
	sleep_mode();
}
	
#endif


#if defined(__AVR_ATmega64__)

void ADC_settings(uint8_t sample_mode,uint8_t alignment,uint8_t presc){
	
	// alignment of conversion result
	if (alignment==ADC_LEFT_ALIGNED) ADMUX |= (1<<ADLAR); else ADMUX &= ~(1<<ADLAR);
	
	// free running or single conversion sample moe
	if (sample_mode!=ADC_SINGLE_SAMPLE_MODE){
		 ADCSRA |= (1<<ADATE); 
		 switch(sample_mode){
			 case ADC_TRIGGER_FREE_RUNNING: ADCSRB = 0x00;break;
			 case ADC_TRIGGER_ANALOG_COMP: ADCSRB = (1<<ADTS0);break;
			 case ADC_TRIGGER_INT0: ADCSRB = (1<<ADTS1);break;
			 case ADC_TRIGGER_TIMER0_COMP: ADCSRB = (1<<ADTS1) | (1<<ADTS0);break;
			 case ADC_TRIGGER_TIMER0_OVERFLOW: ADCSRB = (1<<ADTS2);break;
			 case ADC_TRIGGER_TIMER1_COMP_B: ADCSRB = (1<<ADTS2) | (1<<ADTS0);break;
			 case ADC_TRIGGER_TIMER1_OVERFLOW: ADCSRB = (1<<ADTS2) | (1<<ADTS1);break;
			 case ADC_TRIGGER_TIMER1_CAPTURE_EVENT: ADCSRB = (1<<ADTS2) | (1<<ADTS1) | (1<<ADTS0);break;
		 }
	}
	else  ADCSRA &= ~(1<<ADATE);
	
	// set ADC-clock prescaler
	ADCSRA &= ~((1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0));
	ADCSRA |= presc;
}

void ADC_set_reference(uint8_t ref){
	ADMUX &= ~((1<<REFS1) | (1<<REFS0));
	ADMUX |= ref;
}

void ADC_set_channel(uint8_t channel){
	ADMUX &= 0xE0;
	ADMUX |= channel;
}

void ADC_disable(){
	ADCSRA &= ~(1<<ADEN);
}

void ADC_enable(){
	ADCSRA |= (1<<ADEN);
}

// should not be called if ADLAR=0, reads 8bit
uint8_t ADC_read_low_prec(){
	if (ADMUX & (1<<ADLAR)) return ADCH; else return 0x00;
}

// reads 10bit
uint16_t ADC_read_high_prec(){
	t16 res;
	res.val8[0] = ADCL;
	res.val8[1] = ADCH;
	return res.val16;
}

uint16_t ADC_single_conversion_high_prec(){
	ADCSRA |= (1<<ADSC) | (1<<ADEN);
	while(ADCSRA & (1<<ADSC)){};
	return ADC_read_high_prec();
}

uint8_t ADC_single_conversion_low_prec(){	
	ADCSRA |= (1<<ADSC) | (1<<ADEN);
	while(ADCSRA & (1<<ADSC)){};
	return ADC_read_low_prec();
}

#endif





