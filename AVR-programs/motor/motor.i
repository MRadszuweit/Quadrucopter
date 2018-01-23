# 1 "motor.c"
# 1 "/home/radszuweit/AVR/Programme/motor//"
# 1 "<command-line>"
# 1 "motor.c"







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
# 9 "motor.c" 2
# 1 "/usr/lib/avr/include/avr/eeprom.h" 1 3
# 428 "/usr/lib/avr/include/avr/eeprom.h" 3
# 1 "/usr/lib/gcc/avr/4.8.2/include/stddef.h" 1 3 4
# 147 "/usr/lib/gcc/avr/4.8.2/include/stddef.h" 3 4
typedef int ptrdiff_t;
# 212 "/usr/lib/gcc/avr/4.8.2/include/stddef.h" 3 4
typedef unsigned int size_t;
# 324 "/usr/lib/gcc/avr/4.8.2/include/stddef.h" 3 4
typedef int wchar_t;
# 429 "/usr/lib/avr/include/avr/eeprom.h" 2 3
# 515 "/usr/lib/avr/include/avr/eeprom.h" 3
uint8_t __eerd_byte_m8 (const uint8_t *__p) __attribute__((__pure__));




uint16_t __eerd_word_m8 (const uint16_t *__p) __attribute__((__pure__));




uint32_t __eerd_dword_m8 (const uint32_t *__p) __attribute__((__pure__));




float __eerd_float_m8 (const float *__p) __attribute__((__pure__));





void __eerd_block_m8 (void *__dst, const void *__src, size_t __n);





void __eewr_byte_m8 (uint8_t *__p, uint8_t __value);




void __eewr_word_m8 (uint16_t *__p, uint16_t __value);




void __eewr_dword_m8 (uint32_t *__p, uint32_t __value);




void __eewr_float_m8 (float *__p, float __value);





void __eewr_block_m8 (const void *__src, void *__dst, size_t __n);





void __eeupd_byte_m8 (uint8_t *__p, uint8_t __value);




void __eeupd_word_m8 (uint16_t *__p, uint16_t __value);




void __eeupd_dword_m8 (uint32_t *__p, uint32_t __value);




void __eeupd_float_m8 (float *__p, float __value);





void __eeupd_block_m8 (const void *__src, void *__dst, size_t __n);
# 10 "motor.c" 2
# 1 "/usr/lib/avr/include/avr/interrupt.h" 1 3
# 11 "motor.c" 2
# 1 "/usr/lib/avr/include/util/delay.h" 1 3
# 43 "/usr/lib/avr/include/util/delay.h" 3
# 1 "/usr/lib/avr/include/util/delay_basic.h" 1 3
# 40 "/usr/lib/avr/include/util/delay_basic.h" 3
static inline void _delay_loop_1(uint8_t __count) __attribute__((always_inline));
static inline void _delay_loop_2(uint16_t __count) __attribute__((always_inline));
# 80 "/usr/lib/avr/include/util/delay_basic.h" 3
void
_delay_loop_1(uint8_t __count)
{
 __asm__ volatile (
  "1: dec %0" "\n\t"
  "brne 1b"
  : "=r" (__count)
  : "0" (__count)
 );
}
# 102 "/usr/lib/avr/include/util/delay_basic.h" 3
void
_delay_loop_2(uint16_t __count)
{
 __asm__ volatile (
  "1: sbiw %0,1" "\n\t"
  "brne 1b"
  : "=w" (__count)
  : "0" (__count)
 );
}
# 44 "/usr/lib/avr/include/util/delay.h" 2 3
# 1 "/usr/lib/avr/include/math.h" 1 3
# 127 "/usr/lib/avr/include/math.h" 3
extern double cos(double __x) __attribute__((__const__));





extern double sin(double __x) __attribute__((__const__));





extern double tan(double __x) __attribute__((__const__));






extern double fabs(double __x) __attribute__((__const__));






extern double fmod(double __x, double __y) __attribute__((__const__));
# 168 "/usr/lib/avr/include/math.h" 3
extern double modf(double __x, double *__iptr);



extern float modff (float __x, float *__iptr);




extern double sqrt(double __x) __attribute__((__const__));





extern double cbrt(double __x) __attribute__((__const__));
# 194 "/usr/lib/avr/include/math.h" 3
extern double hypot (double __x, double __y) __attribute__((__const__));







extern double square(double __x) __attribute__((__const__));






extern double floor(double __x) __attribute__((__const__));






extern double ceil(double __x) __attribute__((__const__));
# 234 "/usr/lib/avr/include/math.h" 3
extern double frexp(double __x, int *__pexp);







extern double ldexp(double __x, int __exp) __attribute__((__const__));





extern double exp(double __x) __attribute__((__const__));





extern double cosh(double __x) __attribute__((__const__));





extern double sinh(double __x) __attribute__((__const__));





extern double tanh(double __x) __attribute__((__const__));







extern double acos(double __x) __attribute__((__const__));







extern double asin(double __x) __attribute__((__const__));






extern double atan(double __x) __attribute__((__const__));
# 298 "/usr/lib/avr/include/math.h" 3
extern double atan2(double __y, double __x) __attribute__((__const__));





extern double log(double __x) __attribute__((__const__));





extern double log10(double __x) __attribute__((__const__));





extern double pow(double __x, double __y) __attribute__((__const__));






extern int isnan(double __x) __attribute__((__const__));
# 333 "/usr/lib/avr/include/math.h" 3
extern int isinf(double __x) __attribute__((__const__));






__attribute__((__const__)) static inline int isfinite (double __x)
{
    unsigned char __exp;
    __asm__ (
 "mov	%0, %C1		\n\t"
 "lsl	%0		\n\t"
 "mov	%0, %D1		\n\t"
 "rol	%0		"
 : "=r" (__exp)
 : "r" (__x) );
    return __exp != 0xff;
}






__attribute__((__const__)) static inline double copysign (double __x, double __y)
{
    __asm__ (
 "bst	%D2, 7	\n\t"
 "bld	%D0, 7	"
 : "=r" (__x)
 : "0" (__x), "r" (__y) );
    return __x;
}
# 376 "/usr/lib/avr/include/math.h" 3
extern int signbit (double __x) __attribute__((__const__));






extern double fdim (double __x, double __y) __attribute__((__const__));
# 392 "/usr/lib/avr/include/math.h" 3
extern double fma (double __x, double __y, double __z) __attribute__((__const__));







extern double fmax (double __x, double __y) __attribute__((__const__));







extern double fmin (double __x, double __y) __attribute__((__const__));






extern double trunc (double __x) __attribute__((__const__));
# 426 "/usr/lib/avr/include/math.h" 3
extern double round (double __x) __attribute__((__const__));
# 439 "/usr/lib/avr/include/math.h" 3
extern long lround (double __x) __attribute__((__const__));
# 453 "/usr/lib/avr/include/math.h" 3
extern long lrint (double __x) __attribute__((__const__));
# 45 "/usr/lib/avr/include/util/delay.h" 2 3
# 84 "/usr/lib/avr/include/util/delay.h" 3
static inline void _delay_us(double __us) __attribute__((always_inline));
static inline void _delay_ms(double __ms) __attribute__((always_inline));
# 141 "/usr/lib/avr/include/util/delay.h" 3
void
_delay_ms(double __ms)
{
 uint16_t __ticks;
 double __tmp ;



 uint32_t __ticks_dc;
 extern void __builtin_avr_delay_cycles(unsigned long);
 __tmp = ((7372800UL) / 1e3) * __ms;
# 161 "/usr/lib/avr/include/util/delay.h" 3
  __ticks_dc = (uint32_t)(ceil(fabs(__tmp)));


 __builtin_avr_delay_cycles(__ticks_dc);
# 186 "/usr/lib/avr/include/util/delay.h" 3
}
# 223 "/usr/lib/avr/include/util/delay.h" 3
void
_delay_us(double __us)
{
 uint8_t __ticks;
 double __tmp ;



 uint32_t __ticks_dc;
 extern void __builtin_avr_delay_cycles(unsigned long);
 __tmp = ((7372800UL) / 1e6) * __us;
# 243 "/usr/lib/avr/include/util/delay.h" 3
  __ticks_dc = (uint32_t)(ceil(fabs(__tmp)));


 __builtin_avr_delay_cycles(__ticks_dc);
# 261 "/usr/lib/avr/include/util/delay.h" 3
}
# 12 "motor.c" 2
# 1 "../timer/timer.h" 1
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
# 13 "motor.c" 2
# 1 "../twi/twi.h" 1
# 24 "../twi/twi.h"
union u16{
 uint16_t t16;
 uint8_t t8[2];
};

void twi_init_master(uint8_t bitrate,uint8_t prescaler);
void twi_init_slave(uint8_t ID);
uint8_t twi_master_transmit_byte(uint8_t addr,uint8_t* Bytes,uint8_t size);
uint8_t twi_slave_receive_byte(uint8_t* byte);
# 14 "motor.c" 2



uint16_t MIN_SAVE __attribute__((section(".eeprom"))) = 0x0000;
uint16_t MAX_SAVE __attribute__((section(".eeprom"))) = 0xFFFF;

uint8_t trans;
static volatile uint8_t sreg;
static volatile uint16_t temp,scale;

void __vector_1 (void) __attribute__ ((signal,used, externally_visible)) ; void __vector_1 (void){
 temp = (*(volatile uint16_t *)((0x04) + 0x20));
 __eewr_word_m8(&MIN_SAVE,temp);
 if ((*(volatile uint8_t *)((0x15) + 0x20)) & (1<<0)) (*(volatile uint8_t *)((0x15) + 0x20)) &= ~(1<<0); else (*(volatile uint8_t *)((0x15) + 0x20)) |= (1<<0);
}

void __vector_2 (void) __attribute__ ((signal,used, externally_visible)) ; void __vector_2 (void){
 temp = (*(volatile uint16_t *)((0x04) + 0x20));
 __eewr_word_m8(&MAX_SAVE,temp);
 if ((*(volatile uint8_t *)((0x15) + 0x20)) & (1<<0)) (*(volatile uint8_t *)((0x15) + 0x20)) &= ~(1<<0); else (*(volatile uint8_t *)((0x15) + 0x20)) |= (1<<0);
}

uint16_t get_ADC(void){
 (*(volatile uint8_t *)((0x06) + 0x20)) |= (1<<6);
 while ((*(volatile uint8_t *)((0x06) + 0x20)) & (1<<6)){};
 uint16_t res = (*(volatile uint16_t *)((0x04) + 0x20));
 return res;
}

int main(){

    (*(volatile uint8_t *)((0x11) + 0x20)) = ~((1<<2) | (1<<3));
    (*(volatile uint8_t *)((0x12) + 0x20)) = 0xFF;

    (*(volatile uint8_t *)((0x14) + 0x20)) = (1<<0);
    (*(volatile uint8_t *)((0x15) + 0x20)) = ~(1<<0);

    (*(volatile uint8_t *)((0x17) + 0x20)) = (1<<1) | (1<<2);
    (*(volatile uint8_t *)((0x18) + 0x20)) = 0xFF;


    (*(volatile uint8_t *)((0x35) + 0x20)) |= (1<<0) | (1<<1) | (1<<2) | (1<<3);
 (*(volatile uint8_t *)((0x3B) + 0x20)) |= (1<<6) | (1<<7);


    (*(volatile uint8_t *)((0x07) + 0x20)) = (1<<5) | (1<<1) | (1<<0);

    (*(volatile uint8_t *)((0x06) + 0x20)) = (1<<7) | (1<<2) | (1<<1) | (1<<0);


    twi_init_slave(0x02);

    volatile uint16_t min_load = __eerd_word_m8(&MIN_SAVE);
    volatile uint16_t max_load = __eerd_word_m8(&MAX_SAVE);
    scale = (max_load-min_load)/0xFF;

    init_pwm_motor_control(0x32,min_load,min_load);


 union u16 speed;
 speed.t16 = 0x8000;
 temp = 0x8000;

 __asm__ __volatile__ ("sei" ::: "memory");

    while(1){
  if (twi_slave_receive_byte(speed.t8)==0x01){
   if ((*(volatile uint8_t *)((0x12) + 0x20)) & (1<<7)) (*(volatile uint8_t *)((0x12) + 0x20)) &= ~(1<<7); else (*(volatile uint8_t *)((0x12) + 0x20)) |= (1<<7);
  };

  sreg = (*(volatile uint8_t *)((0x3F) + 0x20));
  __asm__ __volatile__ ("cli" ::: "memory");
  temp = get_ADC();
  set_pwm_channel_A(speed);
  (*(volatile uint8_t *)((0x3F) + 0x20)) = sreg;

     _delay_ms(1);
    }
}
