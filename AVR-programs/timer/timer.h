#ifndef TIMER
#define TIMER

#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef F_CPU
#define F_CPU 3686400UL
#endif

#define TIMER_16A 0x01
#define TIMER_16B 0x02

#define NORMAL_MODE 0x00
#define CTC_MODE 0x01
#define PWM_MODE 0x02
#define FAST_PWM_MODE 0x03

#define CLK_1 (1<<CS10)
#define CLK_8 (1<<CS11)
#define CLK_64 ((1<<CS11) | (1<<CS10))
#define CLK_256 (1<<CS12)
#define CLK_1024 ((1<<CS12) | (1<<CS10))

#define CLK0_1 (1<<CS00)
#define CLK0_8 (1<<CS01)
#define CLK0_64 (1<<CS00) | (1<<CS01)
#define CLK0_256 (1<<CS02)
#define CLK0_1024 (1<<CS00) | (1<<CS02)
#define CLK0_EXT_FALLING (1<<CS01) | (1<<CS02)
#define CLK0_EXT_RISING (1<<CS00) | (1<<CS01) | (1<<CS02)

union time32{
	uint32_t t32;
	struct { uint16_t L,H;} t16;
};

union time16{
	uint16_t t16;
	struct { uint8_t L,H;} t8;
};

void timer1_mode(uint8_t mode);
void reset_timer32(void);
void timer1_prescaler(uint8_t pre);
uint16_t get_time_Lword(void);
void set_time_Lword(uint16_t time);
void set_output_comp(uint16_t val,uint8_t Timer);
void timer1_stop(void);
void timer1_continue(void);
void start_timer32(void);
float get_time_sec(void);
//void set_pwm_channel_A(uint16_t fracA);
void set_pwm_channel_B(uint16_t fracB);
void init_pwm_motor_control(uint16_t freq,uint16_t fracA,uint16_t fracB);

#endif
