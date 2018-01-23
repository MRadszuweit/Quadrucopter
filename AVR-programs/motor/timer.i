# 1 "../timer/timer.c"
# 1 "/home/radszuweit/AVR/Programme/motor//"
# 1 "<command-line>"
# 1 "../timer/timer.c"
# 1 "../timer/timer.h" 1



# 1 "/usr/lib/avr/include/avr/io.h" 1 3
# 99 "/usr/lib/avr/include/avr/io.h" 3
# 1 "/usr/lib/avr/include/avr/sfr_defs.h" 1 3
# 126 "/usr/lib/avr/include/avr/sfr_defs.h" 3
# 1 "/usr/lib/avr/include/inttypes.h" 1 3
# 37 "/usr/lib/avr/include/inttypes.h" 3
# 1 "/usr/lib/gcc/avr/4.8.2/include/stdint.h" 1 3 4
# 9 "/usr/lib/gcc/avr/4.8.2/include/stdint.h" 3 4
# 1 "/usr/lib/avr/include/stdint.h" 1 3 4
# 121 "/usr/lib/avr/include/stdint.h" 3 4
typedef signed int int8_t __attribute__((__mode__(__QI__)));
typedef unsigned int uint8_t __attribute__((__mode__(__QI__)));
typedef signed int int16_t __attribute__ ((__mode__ (__HI__)));
typedef unsigned int uint16_t __attribute__ ((__mode__ (__HI__)));
typedef signed int int32_t __attribute__ ((__mode__ (__SI__)));
typedef unsigned int uint32_t __attribute__ ((__mode__ (__SI__)));

typedef signed int int64_t __attribute__((__mode__(__DI__)));
typedef unsigned int uint64_t __attribute__((__mode__(__DI__)));
# 142 "/usr/lib/avr/include/stdint.h" 3 4
typedef int16_t intptr_t;




typedef uint16_t uintptr_t;
# 159 "/usr/lib/avr/include/stdint.h" 3 4
typedef int8_t int_least8_t;




typedef uint8_t uint_least8_t;




typedef int16_t int_least16_t;




typedef uint16_t uint_least16_t;




typedef int32_t int_least32_t;




typedef uint32_t uint_least32_t;







typedef int64_t int_least64_t;






typedef uint64_t uint_least64_t;
# 213 "/usr/lib/avr/include/stdint.h" 3 4
typedef int8_t int_fast8_t;




typedef uint8_t uint_fast8_t;




typedef int16_t int_fast16_t;




typedef uint16_t uint_fast16_t;




typedef int32_t int_fast32_t;




typedef uint32_t uint_fast32_t;







typedef int64_t int_fast64_t;






typedef uint64_t uint_fast64_t;
# 273 "/usr/lib/avr/include/stdint.h" 3 4
typedef int64_t intmax_t;




typedef uint64_t uintmax_t;
# 10 "/usr/lib/gcc/avr/4.8.2/include/stdint.h" 2 3 4
# 38 "/usr/lib/avr/include/inttypes.h" 2 3
# 77 "/usr/lib/avr/include/inttypes.h" 3
typedef int32_t int_farptr_t;



typedef uint32_t uint_farptr_t;
# 127 "/usr/lib/avr/include/avr/sfr_defs.h" 2 3
# 100 "/usr/lib/avr/include/avr/io.h" 2 3
# 274 "/usr/lib/avr/include/avr/io.h" 3
# 1 "/usr/lib/avr/include/avr/iom8.h" 1 3
# 636 "/usr/lib/avr/include/avr/iom8.h" 3
       
# 637 "/usr/lib/avr/include/avr/iom8.h" 3

       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
# 275 "/usr/lib/avr/include/avr/io.h" 2 3
# 432 "/usr/lib/avr/include/avr/io.h" 3
# 1 "/usr/lib/avr/include/avr/portpins.h" 1 3
# 433 "/usr/lib/avr/include/avr/io.h" 2 3

# 1 "/usr/lib/avr/include/avr/common.h" 1 3
# 435 "/usr/lib/avr/include/avr/io.h" 2 3

# 1 "/usr/lib/avr/include/avr/version.h" 1 3
# 437 "/usr/lib/avr/include/avr/io.h" 2 3


# 1 "/usr/lib/avr/include/avr/fuse.h" 1 3
# 248 "/usr/lib/avr/include/avr/fuse.h" 3
typedef struct
{
    unsigned char low;
    unsigned char high;
} __fuse_t;
# 440 "/usr/lib/avr/include/avr/io.h" 2 3


# 1 "/usr/lib/avr/include/avr/lock.h" 1 3
# 443 "/usr/lib/avr/include/avr/io.h" 2 3
# 5 "../timer/timer.h" 2
# 1 "/usr/lib/avr/include/avr/interrupt.h" 1 3
# 6 "../timer/timer.h" 2
# 21 "../timer/timer.h"
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

void set_pwm_channel_B(uint16_t fracB);
void init_pwm_motor_control(uint16_t freq,uint16_t fracA,uint16_t fracB);
# 2 "../timer/timer.c" 2

const uint32_t MAX = 0x0000FFFF;

static volatile uint8_t tccr1b;
static volatile uint16_t time_Hword = 0x0000;
static volatile uint32_t pwm_TOP = 0xFFFF;
static volatile uint32_t presc1 = 3686400UL;

void __vector_8 (void) __attribute__ ((signal,used, externally_visible)) ; void __vector_8 (void){
 time_Hword++;
}

void timer1_mode(uint8_t mode){
 if (mode==0x00){
  (*(volatile uint8_t *)((0x2F) + 0x20)) &= ~((1<<0) | (1<<1));
  (*(volatile uint8_t *)((0x2E) + 0x20)) &= ~((1<<3) | (1<<4));
 }
}

void timer1_prescaler(uint8_t pre){
 uint8_t mask;
 mask = (1<<2) | (1<<1) | (1<<0);
 mask ^= 0xFF;
 (*(volatile uint8_t *)((0x2E) + 0x20)) |= pre;

 switch(pre){
  case (1<<0): presc1 = (uint32_t)3686400UL;break;
  case (1<<1): presc1 = (uint32_t)3686400UL / 8UL;break;
  case ((1<<1) | (1<<0)): presc1 = (uint32_t)3686400UL / 64UL;break;
  case (1<<2): presc1 = (uint32_t)3686400UL / 256UL;break;
  case ((1<<2) | (1<<0)): presc1 = (uint32_t)3686400UL / 1024UL;break;
 }
}

void timer1_stop(void){
 tccr1b = (*(volatile uint8_t *)((0x2E) + 0x20));
 (*(volatile uint8_t *)((0x2E) + 0x20)) &= ~((1<<2) | (1<<1) | (1<<0));
}

void timer1_continue(void){
 (*(volatile uint8_t *)((0x2E) + 0x20)) = tccr1b;
}

uint16_t get_time_Lword(void){
 uint16_t res;
 uint8_t temp = (*(volatile uint8_t *)((0x3F) + 0x20));
 __asm__ __volatile__ ("cli" ::: "memory");
 res = (*(volatile uint16_t *)((0x2C) + 0x20));
 __asm__ __volatile__ ("sei" ::: "memory");
 (*(volatile uint8_t *)((0x3F) + 0x20)) = temp;
}

void set_time_Lword(uint16_t time){
 uint8_t temp = (*(volatile uint8_t *)((0x3F) + 0x20));
 __asm__ __volatile__ ("cli" ::: "memory");
 (*(volatile uint16_t *)((0x2C) + 0x20)) = time;
 __asm__ __volatile__ ("sei" ::: "memory");
 (*(volatile uint8_t *)((0x3F) + 0x20)) = temp;
}

void set_output_comp(uint16_t val,uint8_t Timer){
 uint8_t temp = (*(volatile uint8_t *)((0x3F) + 0x20));
 __asm__ __volatile__ ("cli" ::: "memory");
 if (Timer==0x01) (*(volatile uint16_t *)((0x2A) + 0x20)) = val;
 if (Timer==0x02) (*(volatile uint16_t *)((0x28) + 0x20)) = val;
 __asm__ __volatile__ ("sei" ::: "memory");
 (*(volatile uint8_t *)((0x3F) + 0x20)) = temp;
}

float get_time_sec(void){
 uint8_t temp = (*(volatile uint8_t *)((0x3F) + 0x20));
 __asm__ __volatile__ ("cli" ::: "memory");
 union time32 current_time;
 current_time.t16.L = get_time_Lword();
 current_time.t16.H = time_Hword;

 float res = (float)current_time.t32/presc1;
 __asm__ __volatile__ ("sei" ::: "memory");
 (*(volatile uint8_t *)((0x3F) + 0x20)) = temp;
 return res;
}

void reset_timer32(void){
 set_time_Lword(0);
 time_Hword = 0;
}

void start_timer32(void){
 timer1_mode(0x00);
 timer1_prescaler(((1<<1) | (1<<0)));
 (*(volatile uint8_t *)((0x39) + 0x20)) |= (1<<2);
 reset_timer32();
 __asm__ __volatile__ ("sei" ::: "memory");
}

uint16_t get_time_Hword(void){
 return time_Hword;
}

void set_pwm_channel_A(volatile uint16_t fracA){
 volatile uint8_t temp = (*(volatile uint8_t *)((0x3F) + 0x20));
 __asm__ __volatile__ ("cli" ::: "memory");




 fracA /= 0x0A;

 uint32_t count = pwm_TOP*fracA/0xFFFF;
 (*(volatile uint16_t *)((0x2A) + 0x20)) = (uint16_t)count;
 (*(volatile uint8_t *)((0x3F) + 0x20)) = temp;
}

void set_pwm_channel_B(uint16_t fracB){
 uint8_t temp = (*(volatile uint8_t *)((0x3F) + 0x20));
 __asm__ __volatile__ ("cli" ::: "memory");

 uint32_t count = pwm_TOP*fracB/0xFFFF;
 (*(volatile uint16_t *)((0x28) + 0x20)) = (uint16_t)count;
 (*(volatile uint8_t *)((0x3F) + 0x20)) = temp;
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
 uint8_t temp = (*(volatile uint8_t *)((0x3F) + 0x20));
 __asm__ __volatile__ ("cli" ::: "memory");

 uint32_t max = (uint32_t)2*3686400UL / freq;
 uint8_t presc_mode = (1<<0);
 if (max>MAX){
  max >>= 3U;
  presc_mode = (1<<1);
 }
 else goto Init;
 if (max>MAX){
  max >>= 3U;
  presc_mode = ((1<<1) | (1<<0));
 }
 else goto Init;
 if (max>MAX){
  max >>= 2U;
  presc_mode = (1<<2);
 }
 else goto Init;
 if (max>MAX){
  max >>= 2U;
  presc_mode = ((1<<2) | (1<<0));
 }

Init:
 pwm_TOP = max;
 (*(volatile uint8_t *)((0x2F) + 0x20)) = (1<<7) | (1<<5) | (1<<1);
 (*(volatile uint8_t *)((0x2E) + 0x20)) = (1<<4) | (1<<3);
 timer1_prescaler(presc_mode);


 (*(volatile uint16_t *)((0x26) + 0x20)) = (uint16_t) max;
 set_pwm_channel_A(fracA);
 set_pwm_channel_B(fracB);

 (*(volatile uint16_t *)((0x2C) + 0x20)) = 0x0000;
 (*(volatile uint8_t *)((0x3F) + 0x20)) = temp;
}
