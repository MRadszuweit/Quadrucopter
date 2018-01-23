#ifndef AVR_ADC
#define AVR_ADC

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define ADC_REF_INTERNAL_256mV ((1<<REFS1) | (1<<REFS0))
#define ADC_REF_INTERNAL_Vcc (1<<REFS0)
#define ADC_REF_EXTERNAL 0x00

#define ADC_IO_ANALOG_MIN 0x0020
#define ADC_IO_ANALOG_MAX 0x03FF
#define ADC_IO_ANALOG_DIFF 0x03DF

#define ADC_LEFT_ALIGNED 0x01
#define ADC_RIGHT_ALIGNED 0x00

#define ADC_SINGLE_SAMPLE_MODE 0x00
#define ADC_FREE_RUNNING_MODE 0x01

#define ADC_TRIGGER_FREE_RUNNING 0x01
#define ADC_TRIGGER_ANALOG_COMP 0x02
#define ADC_TRIGGER_INT0 0x03
#define ADC_TRIGGER_TIMER0_COMP 0x04
#define ADC_TRIGGER_TIMER0_OVERFLOW 0x05
#define ADC_TRIGGER_TIMER1_COMP_B 0x06
#define ADC_TRIGGER_TIMER1_OVERFLOW 0x07
#define ADC_TRIGGER_TIMER1_CAPTURE_EVENT 0x08

#define ADC_PRESC_2 (1<<ADPS0)
#define ADC_PRESC_4 (1<<ADPS1)
#define ADC_PRESC_8 ((1<<ADPS1) | (1<<ADPS0))
#define ADC_PRESC_16 (1<<ADPS2)
#define ADC_PRESC_32 ((1<<ADPS2) | (1<<ADPS0))
#define ADC_PRESC_64 ((1<<ADPS2) | (1<<ADPS1))
#define ADC_PRESC_128 ((1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0))

#define ADC_CH0 0x00
#define ADC_CH1 0x01
#define ADC_CH2 0x02
#define ADC_CH3 0x03
#define ADC_CH4 0x04
#define ADC_CH5 0x05
#define ADC_CH6 0x06
#define ADC_CH7 0x07

typedef union UIT16{
	uint16_t val16;
	uint8_t val8[2];
}t16;

void ADC_settings(uint8_t sample_mode,uint8_t alignment,uint8_t presc);
void ADC_set_reference(uint8_t ref);
void ADC_set_channel(uint8_t channel);
void ADC_disable();
void ADC_enable();

uint8_t ADC_read_low_prec();
uint16_t ADC_read_high_prec();

uint8_t ADC_start_conversion();

#if defined(__AVR_ATmega8__)
uint16_t ADC_single_conversion_read();
#endif

#if defined(__AVR_ATmega64__)
uint16_t ADC_single_conversion_high_prec();
uint8_t ADC_single_conversion_low_prec();
#endif

#endif 
