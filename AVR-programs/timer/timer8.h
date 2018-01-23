#ifndef TIMER8
#define TIMER8

#include <avr/io.h>
#include <avr/interrupt.h>

#define NO 0x00
#define YES 0x01

#define TIMER2_PRESC_MASK (1<<CS22) | (1<<CS21) | (1<<CS20)
#define TIMER2_CLK_STOP 0x00
#define TIMER2_CLK_1 (1<<CS20)
#define TIMER2_CLK_8 (1<<CS21)
#define TIMER2_CLK_32 (1<<CS21) | (1<<CS20)
#define TIMER2_CLK_64 (1<<CS22)
#define TIMER2_CLK_128 (1<<CS22) | (1<<CS20)
#define TIMER2_CLK_256 (1<<CS22) | (1<<CS21)
#define TIMER2_CLK_1024 (1<<CS22) | (1<<CS21) | (1<<CS20)

#define TIMER2_MODE_MASK (1<<WGM21) | (1<<WGM20)
#define TIMER2_NORMAL 0x00
#define TIMER2_PWM (1<<WGM20)
#define TIMER2_CTC (1<<WGM21)
#define TIMER2_FAST_PWM (1<<WGM21) | (1<<WGM20)

#define TIMER2_OUT_MASK (1<<COM21) | (1<<COM20)
#define TIMER2_DISCONNECT 0x00
#define TIMER2_TOGGLE_ON_MATCH (1<<COM20)
#define TIMER2 CLEAR_ON_MATCH (1<<COM21)
#define TIMER2_SET_ON_MATCH (1<<COM21) | (1<<COM20)

typedef union UINT32{
	uint32_t val32;
	uint16_t val16[2];
	uint8_t val8[4];
}t32;

///////////////////////////////// functions ////////////////////////////////////

void init_timer2(uint8_t mode,uint8_t output,uint8_t presc);
void timer2_set_comp_value(uint8_t comp);
void start_timer2(uint8_t presc);
void stop_timer2();
void timer2_external_clock(uint8_t mode);

#endif
