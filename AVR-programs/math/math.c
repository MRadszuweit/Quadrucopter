#include "math.h"

float32 init_float32(int16_t val){
	int8_t exp;
	float32 res;
	
	if (val<0){
		res.sign = -1;
		res.mantisse = (int16_t)(-val);
	}
	else{
		res.sign = 1;
		res.mantisse = val;
	}
	
	exp = 0;	
	while ((exp>-15) & ((res.mantisse & MASK_BIT16)==0)){
		res.mantisse <<= 1;
		exp--;
	}
	res.exponent = exp;
	return res;
}

float32 init_float32_rational(int16_t enume,int16_t denom){
	float32 res = init_float32(enume);
	float32 h = init_float32(denom);
	div_float32(&res,&h);
	return res;
}

int16_t float32_to_int16(volatile float32 val){
	uint8_t exp;
	
	if (val.exponent<0){
		exp = (int8_t)(-val.exponent);
		val.mantisse >>= exp;
	}
	if ((val.mantisse & MASK_BIT16)!=0) return 0;
	if (val.sign==-1) return (int16_t)(-val.mantisse);
	else return (int16_t)val.mantisse;
}

void mult_float32(volatile float32* A,volatile float32* B){				// A *= B
	uin32 man;
	
	man.val32 = (uint32_t)A->mantisse*B->mantisse;	
	if ((man.val32 & MASK_BIT32)==0){
		man.val32 <<= 1;
		A->exponent--;
	}
	
	A->mantisse = man.val16[1];
	A->exponent += B->exponent+16;
	A->sign *= B->sign;	
}

void div_float32(volatile float32* A,volatile float32* B){				// A /= B
	uin32 man; 
	
	man.val16[0] = 0;
	man.val16[1] = A->mantisse;
	
	man.val32 /= (uint16_t)B->mantisse; 
	if (man.val16[1]!=0){
		man.val32 >>= 1;
		A->exponent++;
	}
	
	A->mantisse = man.val16[0];
	A->exponent -= B->exponent+16;
	A->sign *= B->sign;
}

void add_float32(volatile float32* A,volatile float32* B){				// A += B
	
	int8_t d = B->exponent-A->exponent;
	if (d<-14) return;
	if (d>14){
		A->mantisse = B->mantisse;
		A->exponent = B->exponent;
		A->sign = B->sign;
		return;
	}
	
	uint32_t MA = A->mantisse;
	uint32_t MB = B->mantisse;
	if (d>0){
		MB <<= (uint8_t)d;			
	}
	else{
		A->exponent = B->exponent;	
		d *= -1;
		MA <<= (uint8_t)d;		
	}
	
	int32_t a = (int32_t)MA*A->sign;
	int32_t b = (int32_t)MB*B->sign;
	a += b;
	if (a<0){
		MA = (int32_t)(-a);
		A->sign = -1;
	}
	else{
		MA = a;
		A->sign = 1;
	}
	
	uint8_t shift = 16;
	uint32_t mask = MASK_BIT32;
	
	while ((shift>0) & ((MA & mask)==0)){
		mask >>= 1;
		shift--;
	}
	MA >>= shift;
	A->exponent += shift;
	A->mantisse = MA;	
}

void sub_float32(volatile float32* A,volatile float32* B){				// A -= B	
	B->sign *= -1;
	add_float32(A,B);
	B->sign *= -1;	
}

float32 clone_float32(volatile float32* x){
	float32 res;
	res.mantisse = x->mantisse;
	res.exponent = x->exponent;
	res.sign = x->sign;
	return res;
}

uint16_t sqrt32 (uint32_t q)
{	
	
    uint16_t r, mask;
    uint16_t i = 8*sizeof (r) -1;
 
    r = mask = 1 << i;
 
    for (; i > 0; i--)
    {
        mask >>= 1;
 
        if (q < (uint32_t) r*r)
            r -= mask;
        else
            r += mask;
    }
 
    if (q < (uint32_t) r*r)
        r -= 1;

    return r;
}
