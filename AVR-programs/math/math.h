#ifndef AVR_MATH
#define AVR_MATH

#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega64__)
	#include <avr/io.h>
#else
	#include <stdint.h>
#endif 



/*typedef union UIN32{
	uint32_t val32;
	struct HL16{
		uint16_t L;
		uint16_t H;
	}val16;
}uin32;

typedef struct RATIONAL16{
	int16_t enu;
	uint16_t den;
}rational16;*/

typedef union UIN32{
	uint32_t val32;
	uint16_t val16[2];
}uin32;

typedef struct FLOAT32{
	uint16_t mantisse;
	int8_t exponent;
	int8_t sign;
}float32;


#define MASK_BIT32 0x80000000
#define MASK_BIT16 0x8000

uint16_t sqrt32 (uint32_t q);

float32 init_float32(int16_t val);
int16_t float32_to_int16(volatile float32 val);
void mult_float32(volatile float32* A,volatile float32* B);
void div_float32(volatile float32* A,volatile float32* B);
void add_float32(volatile float32* A,volatile float32* B);
void sub_float32(volatile float32* A,volatile float32* B);
float32 init_float32_rational(int16_t enume,int16_t denom);
float32 clone_float32(volatile float32* x);

#endif
