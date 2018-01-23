# 1 "../twi/twi.c"
# 1 "/home/radszuweit/AVR/Programme/motor//"
# 1 "<command-line>"
# 1 "../twi/twi.c"
# 1 "../twi/twi.h" 1



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
# 5 "../twi/twi.h" 2
# 1 "/usr/lib/avr/include/avr/interrupt.h" 1 3
# 6 "../twi/twi.h" 2
# 24 "../twi/twi.h"
union u16{
 uint16_t t16;
 uint8_t t8[2];
};

void twi_init_master(uint8_t bitrate,uint8_t prescaler);
void twi_init_slave(uint8_t ID);
uint8_t twi_master_transmit_byte(uint8_t addr,uint8_t* Bytes,uint8_t size);
uint8_t twi_slave_receive_byte(uint8_t* byte);
# 2 "../twi/twi.c" 2

static volatile uint8_t twi_error_code;

void twi_init_master(uint8_t bitrate,uint8_t prescaler){
 (*(volatile uint8_t *)((0x00) + 0x20)) = bitrate;
 (*(volatile uint8_t *)((0x01) + 0x20)) &= 0xF8;
 (*(volatile uint8_t *)((0x01) + 0x20)) += prescaler;

}

void twi_init_slave(uint8_t ID){
 (*(volatile uint8_t *)((0x02) + 0x20)) = (ID<<1);
}

void twi_start(void){
 (*(volatile uint8_t *)((0x36) + 0x20)) = (1<<7) | (1<<2) | (1<<5);
}

void twi_stop(void){
 (*(volatile uint8_t *)((0x36) + 0x20)) = (1<<7) | (1<<2) | (1<<4);
}

inline void twi_wait_for_TWINT(void){
 while (!((*(volatile uint8_t *)((0x36) + 0x20)) & (1<<7))){};
}

uint8_t twi_check(uint8_t status_code){
 if (((*(volatile uint8_t *)((0x01) + 0x20)) & 0xF8) != status_code){
  twi_stop();
  twi_error_code = (*(volatile uint8_t *)((0x01) + 0x20));
  return 0x00;
 }
 else return 0x01;
}

uint8_t twi_master_transmit_byte(uint8_t addr,uint8_t* Bytes,uint8_t size){
 uint8_t sreg = (*(volatile uint8_t *)((0x3F) + 0x20));
 __asm__ __volatile__ ("cli" ::: "memory");


 twi_start();
 twi_wait_for_TWINT();


 if (twi_check(0x08)==0x00){
  (*(volatile uint8_t *)((0x3F) + 0x20)) = sreg;
  return 0x00;
 }


 addr <<= 1;
 (*(volatile uint8_t *)((0x03) + 0x20)) = addr & (~(1<<0x00));
 (*(volatile uint8_t *)((0x36) + 0x20)) = (1<<7) | (1<<2);
 twi_wait_for_TWINT();


 if (twi_check(0x18)==0x00){
  (*(volatile uint8_t *)((0x3F) + 0x20)) = sreg;
  return 0x00;
 }

 if ((*(volatile uint8_t *)((0x12) + 0x20)) & (1<<7)) (*(volatile uint8_t *)((0x12) + 0x20)) &= ~(1<<7); else (*(volatile uint8_t *)((0x12) + 0x20)) |= (1<<7);

 int8_t i;
 for (i=0;i<size;i++){

  (*(volatile uint8_t *)((0x03) + 0x20)) = Bytes[i];
  (*(volatile uint8_t *)((0x36) + 0x20)) = (1<<7) | (1<<2);
  twi_wait_for_TWINT();


  if (twi_check(0x28)==0x00){
   (*(volatile uint8_t *)((0x3F) + 0x20)) = sreg;
   return 0x00;
  }
 }


 twi_stop();

 (*(volatile uint8_t *)((0x3F) + 0x20)) = sreg;
 return 0x01;
}

uint8_t twi_slave_receive_byte(uint8_t* byte){
 uint8_t status;
 uint8_t sreg = (*(volatile uint8_t *)((0x3F) + 0x20));

 __asm__ __volatile__ ("cli" ::: "memory");


 (*(volatile uint8_t *)((0x36) + 0x20)) = (1<<2) | (1<<7) | (1<<6);
 twi_wait_for_TWINT();


 if (twi_check(0x60)==0x00){
  (*(volatile uint8_t *)((0x3F) + 0x20)) = sreg;
  return 0x00;
 }

 uint8_t i = 0;
 do{

  (*(volatile uint8_t *)((0x36) + 0x20)) = (1<<2) | (1<<7) | (1<<6);
  twi_wait_for_TWINT();


  status = (*(volatile uint8_t *)((0x01) + 0x20)) & 0xF8;
  if (status==0x80){
   byte[i] = (*(volatile uint8_t *)((0x03) + 0x20));
   i++;
  }
  else if (status!=0xA0){
   (*(volatile uint8_t *)((0x3F) + 0x20)) = sreg;
   return 0x00;
  }
 }while(status!=0xA0);

 (*(volatile uint8_t *)((0x3F) + 0x20)) = sreg;
 return 0x01;
}
