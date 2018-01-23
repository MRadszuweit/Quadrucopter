#ifndef TIMER
#define TIMER

#include <avr/io.h>
#include <avr/interrupt.h>

/////////////////////////////// constants ///////////////////////////////////

#define NO 0x00
#define YES 0x01

#define NORMAL_MODE 0x00
#define FAST_PWM_MODE 0x01
#define CTC_MODE 0x04
#define PWM_MODE 0x09
#define FAST_PWM_MODE_OCR 0x0F			// max=OCRnABC
#define FAST_PWM_MODE_ICR 0x0E			// max=ICRnABC
// other modes for timer 1/3 may follow

#define DISCONNECT 0x00
#define TOGGLE_ON_MATCH 0x01
#define CLEAR_ON_MATCH 0x02
#define SET_ON_MATCH 0x03

#define TIMER0 0x00
#define TIMER1 0x01
#define TIMER2 0x02
#define TIMER3 0x03

#define CHANNEL_A 0x01
#define CHANNEL_B 0x02
#define CHANNEL_C 0x03
#define CHANNEL_ALL 0x04

#define CLK_STOP 0x00

#define CLK0_1 (1<<CS00)
#define CLK0_8 (1<<CS01)
#define CLK0_32 (1<<CS01) | (1<<CS00)
#define CLK0_64 (1<<CS02)
#define CLK0_128 (1<<CS02) | (1<<CS00)
#define CLK0_256 (1<<CS02) | (1<<CS01)
#define CLK0_1024 (1<<CS02) | (1<<CS01) | (1<<CS00)

#define CLK13_1 (1<<CS00)
#define CLK13_8 (1<<CS01)
#define CLK13_64 (1<<CS00) | (1<<CS01)
#define CLK13_256 (1<<CS02)
#define CLK13_1024 (1<<CS00) | (1<<CS02)
#define CLK13_EXT_FALLING (1<<CS01) | (1<<CS02)
#define CLK13_EXT_RISING (1<<CS00) | (1<<CS01) | (1<<CS02)


///////////////////////////////// functions ////////////////////////////////////

void timer0_init(uint8_t mode,uint8_t OC0_mode,uint8_t cmp_val,uint8_t presc);

void timer_init_13(uint8_t timer_ID,uint8_t wgm_mode,uint8_t presc);
void timer_channel_settings_13(uint8_t timer_ID,uint8_t channel,uint8_t out_mode,uint16_t cmp_val);

void timer_channel_cmpval_13(uint8_t timer_ID,uint8_t channel,uint16_t cmp_val);

#endif
