	.file	"timer.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
 ;  GNU C (GCC) version 4.8.2 (avr)
 ; 	compiled by GNU C version 4.8.2, GMP version 5.1.3, MPFR version 3.1.2-p3, MPC version 1.0.1
 ;  GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
 ;  options passed:  -fpreprocessed timer.i -mmcu=atmega8
 ;  -auxbase-strip ../timer/timer.o -g -Os -fverbose-asm
 ;  options enabled:  -faggressive-loop-optimizations -fauto-inc-dec
 ;  -fbranch-count-reg -fcaller-saves -fcombine-stack-adjustments -fcommon
 ;  -fcompare-elim -fcprop-registers -fcrossjumping -fcse-follow-jumps
 ;  -fdefer-pop -fdevirtualize -fdwarf2-cfi-asm -fearly-inlining
 ;  -feliminate-unused-debug-types -fexpensive-optimizations
 ;  -fforward-propagate -ffunction-cse -fgcse -fgcse-lm -fgnu-runtime
 ;  -fguess-branch-probability -fhoist-adjacent-loads -fident
 ;  -fif-conversion -fif-conversion2 -findirect-inlining -finline
 ;  -finline-atomics -finline-functions -finline-functions-called-once
 ;  -finline-small-functions -fipa-cp -fipa-profile -fipa-pure-const
 ;  -fipa-reference -fipa-sra -fira-hoist-pressure -fira-share-save-slots
 ;  -fira-share-spill-slots -fivopts -fkeep-static-consts
 ;  -fleading-underscore -fmath-errno -fmerge-constants
 ;  -fmerge-debug-strings -fmove-loop-invariants -fomit-frame-pointer
 ;  -foptimize-register-move -foptimize-sibling-calls -fpartial-inlining
 ;  -fpeephole -fpeephole2 -fprefetch-loop-arrays -freg-struct-return
 ;  -fregmove -freorder-blocks -freorder-functions -frerun-cse-after-loop
 ;  -fsched-critical-path-heuristic -fsched-dep-count-heuristic
 ;  -fsched-group-heuristic -fsched-interblock -fsched-last-insn-heuristic
 ;  -fsched-rank-heuristic -fsched-spec -fsched-spec-insn-heuristic
 ;  -fsched-stalled-insns-dep -fshow-column -fshrink-wrap -fsigned-zeros
 ;  -fsplit-ivs-in-unroller -fsplit-wide-types -fstrict-aliasing
 ;  -fstrict-overflow -fstrict-volatile-bitfields -fsync-libcalls
 ;  -fthread-jumps -ftoplevel-reorder -ftrapping-math -ftree-bit-ccp
 ;  -ftree-builtin-call-dce -ftree-ccp -ftree-ch -ftree-coalesce-vars
 ;  -ftree-copy-prop -ftree-copyrename -ftree-dce -ftree-dominator-opts
 ;  -ftree-dse -ftree-forwprop -ftree-fre -ftree-loop-if-convert
 ;  -ftree-loop-im -ftree-loop-ivcanon -ftree-loop-optimize
 ;  -ftree-parallelize-loops= -ftree-phiprop -ftree-pre -ftree-pta
 ;  -ftree-reassoc -ftree-scev-cprop -ftree-sink -ftree-slp-vectorize
 ;  -ftree-slsr -ftree-sra -ftree-switch-conversion -ftree-tail-merge
 ;  -ftree-ter -ftree-vect-loop-version -ftree-vrp -funit-at-a-time
 ;  -fverbose-asm -fzero-initialized-in-bss

	.stabs	"/home/radszuweit/AVR/Programme/motor/",100,0,2,.Ltext0
	.stabs	"../timer/timer.c",100,0,2,.Ltext0
	.text
.Ltext0:
	.stabs	"gcc2_compiled.",60,0,0,0
	.stabs	"int:t(0,1)=r(0,1);-32768;32767;",128,0,0,0
	.stabs	"char:t(0,2)=r(0,2);0;127;",128,0,0,0
	.stabs	"long int:t(0,3)=@s32;r(0,3);020000000000;017777777777;",128,0,0,0
	.stabs	"unsigned int:t(0,4)=r(0,4);0;0177777;",128,0,0,0
	.stabs	"long unsigned int:t(0,5)=@s32;r(0,5);0;037777777777;",128,0,0,0
	.stabs	"long long int:t(0,6)=@s64;r(0,6);01000000000000000000000;0777777777777777777777;",128,0,0,0
	.stabs	"long long unsigned int:t(0,7)=@s64;r(0,7);0;01777777777777777777777;",128,0,0,0
	.stabs	"short int:t(0,8)=r(0,8);-32768;32767;",128,0,0,0
	.stabs	"short unsigned int:t(0,9)=r(0,9);0;0177777;",128,0,0,0
	.stabs	"signed char:t(0,10)=@s8;r(0,10);-128;127;",128,0,0,0
	.stabs	"unsigned char:t(0,11)=@s8;r(0,11);0;255;",128,0,0,0
	.stabs	"float:t(0,12)=r(0,1);4;0;",128,0,0,0
	.stabs	"double:t(0,13)=r(0,1);4;0;",128,0,0,0
	.stabs	"long double:t(0,14)=r(0,1);4;0;",128,0,0,0
	.stabs	"short _Fract:t(0,15)=r(0,1);1;0;",128,0,0,0
	.stabs	"_Fract:t(0,16)=r(0,1);2;0;",128,0,0,0
	.stabs	"long _Fract:t(0,17)=r(0,1);4;0;",128,0,0,0
	.stabs	"long long _Fract:t(0,18)=r(0,1);8;0;",128,0,0,0
	.stabs	"unsigned short _Fract:t(0,19)=r(0,1);1;0;",128,0,0,0
	.stabs	"unsigned _Fract:t(0,20)=r(0,1);2;0;",128,0,0,0
	.stabs	"unsigned long _Fract:t(0,21)=r(0,1);4;0;",128,0,0,0
	.stabs	"unsigned long long _Fract:t(0,22)=r(0,1);8;0;",128,0,0,0
	.stabs	"_Sat short _Fract:t(0,23)=r(0,1);1;0;",128,0,0,0
	.stabs	"_Sat _Fract:t(0,24)=r(0,1);2;0;",128,0,0,0
	.stabs	"_Sat long _Fract:t(0,25)=r(0,1);4;0;",128,0,0,0
	.stabs	"_Sat long long _Fract:t(0,26)=r(0,1);8;0;",128,0,0,0
	.stabs	"_Sat unsigned short _Fract:t(0,27)=r(0,1);1;0;",128,0,0,0
	.stabs	"_Sat unsigned _Fract:t(0,28)=r(0,1);2;0;",128,0,0,0
	.stabs	"_Sat unsigned long _Fract:t(0,29)=r(0,1);4;0;",128,0,0,0
	.stabs	"_Sat unsigned long long _Fract:t(0,30)=r(0,1);8;0;",128,0,0,0
	.stabs	"short _Accum:t(0,31)=r(0,1);2;0;",128,0,0,0
	.stabs	"_Accum:t(0,32)=r(0,1);4;0;",128,0,0,0
	.stabs	"long _Accum:t(0,33)=r(0,1);8;0;",128,0,0,0
	.stabs	"long long _Accum:t(0,34)=r(0,1);8;0;",128,0,0,0
	.stabs	"unsigned short _Accum:t(0,35)=r(0,1);2;0;",128,0,0,0
	.stabs	"unsigned _Accum:t(0,36)=r(0,1);4;0;",128,0,0,0
	.stabs	"unsigned long _Accum:t(0,37)=r(0,1);8;0;",128,0,0,0
	.stabs	"unsigned long long _Accum:t(0,38)=r(0,1);8;0;",128,0,0,0
	.stabs	"_Sat short _Accum:t(0,39)=r(0,1);2;0;",128,0,0,0
	.stabs	"_Sat _Accum:t(0,40)=r(0,1);4;0;",128,0,0,0
	.stabs	"_Sat long _Accum:t(0,41)=r(0,1);8;0;",128,0,0,0
	.stabs	"_Sat long long _Accum:t(0,42)=r(0,1);8;0;",128,0,0,0
	.stabs	"_Sat unsigned short _Accum:t(0,43)=r(0,1);2;0;",128,0,0,0
	.stabs	"_Sat unsigned _Accum:t(0,44)=r(0,1);4;0;",128,0,0,0
	.stabs	"_Sat unsigned long _Accum:t(0,45)=r(0,1);8;0;",128,0,0,0
	.stabs	"_Sat unsigned long long _Accum:t(0,46)=r(0,1);8;0;",128,0,0,0
	.stabs	"void:t(0,47)=(0,47)",128,0,0,0
	.stabs	"../timer/timer.h",130,0,0,0
	.stabs	"/usr/lib/avr/include/avr/io.h",130,0,0,0
	.stabs	"/usr/lib/avr/include/avr/sfr_defs.h",130,0,0,0
	.stabs	"/usr/lib/avr/include/inttypes.h",130,0,0,0
	.stabs	"/usr/lib/gcc/avr/4.8.2/include/stdint.h",130,0,0,0
	.stabs	"/usr/lib/avr/include/stdint.h",130,0,0,0
	.stabs	"int8_t:t(6,1)=(0,10)",128,0,121,0
	.stabs	"uint8_t:t(6,2)=(0,11)",128,0,122,0
	.stabs	"int16_t:t(6,3)=(0,1)",128,0,123,0
	.stabs	"uint16_t:t(6,4)=(0,4)",128,0,124,0
	.stabs	"int32_t:t(6,5)=(0,3)",128,0,125,0
	.stabs	"uint32_t:t(6,6)=(0,5)",128,0,126,0
	.stabs	"int64_t:t(6,7)=(0,6)",128,0,128,0
	.stabs	"uint64_t:t(6,8)=(0,7)",128,0,129,0
	.stabs	"intptr_t:t(6,9)=(6,3)",128,0,142,0
	.stabs	"uintptr_t:t(6,10)=(6,4)",128,0,147,0
	.stabs	"int_least8_t:t(6,11)=(6,1)",128,0,159,0
	.stabs	"uint_least8_t:t(6,12)=(6,2)",128,0,164,0
	.stabs	"int_least16_t:t(6,13)=(6,3)",128,0,169,0
	.stabs	"uint_least16_t:t(6,14)=(6,4)",128,0,174,0
	.stabs	"int_least32_t:t(6,15)=(6,5)",128,0,179,0
	.stabs	"uint_least32_t:t(6,16)=(6,6)",128,0,184,0
	.stabs	"int_least64_t:t(6,17)=(6,7)",128,0,192,0
	.stabs	"uint_least64_t:t(6,18)=(6,8)",128,0,199,0
	.stabs	"int_fast8_t:t(6,19)=(6,1)",128,0,213,0
	.stabs	"uint_fast8_t:t(6,20)=(6,2)",128,0,218,0
	.stabs	"int_fast16_t:t(6,21)=(6,3)",128,0,223,0
	.stabs	"uint_fast16_t:t(6,22)=(6,4)",128,0,228,0
	.stabs	"int_fast32_t:t(6,23)=(6,5)",128,0,233,0
	.stabs	"uint_fast32_t:t(6,24)=(6,6)",128,0,238,0
	.stabs	"int_fast64_t:t(6,25)=(6,7)",128,0,246,0
	.stabs	"uint_fast64_t:t(6,26)=(6,8)",128,0,253,0
	.stabs	"intmax_t:t(6,27)=(6,7)",128,0,273,0
	.stabs	"uintmax_t:t(6,28)=(6,8)",128,0,278,0
	.stabn	162,0,0,0
	.stabn	162,0,0,0
	.stabs	"int_farptr_t:t(4,1)=(6,5)",128,0,77,0
	.stabs	"uint_farptr_t:t(4,2)=(6,6)",128,0,81,0
	.stabn	162,0,0,0
	.stabn	162,0,0,0
	.stabs	"/usr/lib/avr/include/avr/fuse.h",130,0,0,0
	.stabs	"__fuse_t:t(7,1)=(7,2)=s2low:(0,11),0,8;high:(0,11),8,8;;",128,0,252,0
	.stabn	162,0,0,0
	.stabn	162,0,0,0
	.stabs	"time32:T(1,1)=u4t32:(6,6),0,32;t16:(1,2)=s4L:(6,4),0,16;H:(6,4),16,16;;,0,32;;",128,0,0,0
	.stabs	"time16:T(1,3)=u2t16:(6,4),0,16;t8:(1,4)=s2L:(6,2),0,8;H:(6,2),8,8;;,0,16;;",128,0,0,0
	.stabn	162,0,0,0
	.stabs	"__vector_8:F(0,47)",36,0,10,__vector_8
.global	__vector_8
	.type	__vector_8, @function
__vector_8:
	.stabd	46,0,0
	.stabn	68,0,10,.LM0-.LFBB1
.LM0:
.LFBB1:
	push r1	 ; 
	push r0	 ; 
	in r0,__SREG__	 ; ,
	push r0	 ; 
	clr __zero_reg__	 ; 
	push r24	 ; 
	push r25	 ; 
/* prologue: Signal */
/* frame size = 0 */
/* stack size = 5 */
.L__stack_usage = 5
	.stabn	68,0,11,.LM1-.LFBB1
.LM1:
	lds r24,time_Hword	 ;  time_Hword.1, time_Hword
	lds r25,time_Hword+1	 ;  time_Hword.1, time_Hword
	adiw r24,1	 ;  time_Hword.1,
	sts time_Hword+1,r25	 ;  time_Hword, time_Hword.1
	sts time_Hword,r24	 ;  time_Hword, time_Hword.1
/* epilogue start */
	.stabn	68,0,12,.LM2-.LFBB1
.LM2:
	pop r25	 ; 
	pop r24	 ; 
	pop r0	 ; 
	out __SREG__,r0	 ; ,
	pop r0	 ; 
	pop r1	 ; 
	reti
	.size	__vector_8, .-__vector_8
.Lscope1:
	.stabs	"",36,0,0,.Lscope1-.LFBB1
	.stabd	78,0,0
	.stabs	"timer1_mode:F(0,47)",36,0,14,timer1_mode
	.stabs	"mode:P(6,2)",64,0,14,24
.global	timer1_mode
	.type	timer1_mode, @function
timer1_mode:
	.stabd	46,0,0
	.stabn	68,0,14,.LM3-.LFBB2
.LM3:
.LFBB2:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	.stabn	68,0,15,.LM4-.LFBB2
.LM4:
	cpse r24,__zero_reg__	 ;  mode,
	rjmp .L2	 ; 
	.stabn	68,0,16,.LM5-.LFBB2
.LM5:
	in r24,0x2f	 ;  D.1711, MEM[(volatile uint8_t *)79B]
	andi r24,lo8(-4)	 ;  D.1711,
	out 0x2f,r24	 ;  MEM[(volatile uint8_t *)79B], D.1711
	.stabn	68,0,17,.LM6-.LFBB2
.LM6:
	in r24,0x2e	 ;  D.1711, MEM[(volatile uint8_t *)78B]
	andi r24,lo8(-25)	 ;  D.1711,
	out 0x2e,r24	 ;  MEM[(volatile uint8_t *)78B], D.1711
.L2:
	ret
	.size	timer1_mode, .-timer1_mode
.Lscope2:
	.stabs	"",36,0,0,.Lscope2-.LFBB2
	.stabd	78,0,0
	.stabs	"timer1_prescaler:F(0,47)",36,0,21,timer1_prescaler
	.stabs	"pre:P(6,2)",64,0,21,24
.global	timer1_prescaler
	.type	timer1_prescaler, @function
timer1_prescaler:
	.stabd	46,0,0
	.stabn	68,0,21,.LM7-.LFBB3
.LM7:
.LFBB3:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	.stabn	68,0,25,.LM8-.LFBB3
.LM8:
	in r25,0x2e	 ;  D.1715, MEM[(volatile uint8_t *)78B]
	or r25,r24	 ;  D.1715, pre
	out 0x2e,r25	 ;  MEM[(volatile uint8_t *)78B], D.1715
	.stabn	68,0,27,.LM9-.LFBB3
.LM9:
	cpi r24,lo8(3)	 ;  pre,
	breq .L7	 ; ,
	brsh .L8	 ; ,
	cpi r24,lo8(1)	 ;  pre,
	breq .L9	 ; ,
	cpi r24,lo8(2)	 ;  pre,
	breq .L10	 ; ,
	ret
.L8:
	cpi r24,lo8(4)	 ;  pre,
	breq .L11	 ; ,
	cpi r24,lo8(5)	 ;  pre,
	breq .L12	 ; ,
	ret
.L9:
	.stabn	68,0,28,.LM10-.LFBB3
.LM10:
	ldi r24,0	 ;  tmp47
	ldi r25,lo8(64)	 ; ,
	ldi r26,lo8(56)	 ; ,
	ldi r27,0	 ; 
	rjmp .L13	 ; 
.L10:
	.stabn	68,0,29,.LM11-.LFBB3
.LM11:
	ldi r24,0	 ;  tmp48
	ldi r25,lo8(8)	 ; ,
	ldi r26,lo8(7)	 ; ,
	ldi r27,0	 ; 
	rjmp .L13	 ; 
.L7:
	.stabn	68,0,30,.LM12-.LFBB3
.LM12:
	ldi r24,0	 ;  tmp49
	ldi r25,lo8(-31)	 ; ,
	ldi r26,0	 ; 
	ldi r27,0	 ; 
	rjmp .L13	 ; 
.L11:
	.stabn	68,0,31,.LM13-.LFBB3
.LM13:
	ldi r24,lo8(64)	 ;  tmp50,
	ldi r25,lo8(56)	 ; ,
	ldi r26,0	 ; 
	ldi r27,0	 ; 
	rjmp .L13	 ; 
.L12:
	.stabn	68,0,32,.LM14-.LFBB3
.LM14:
	ldi r24,lo8(16)	 ;  tmp51,
	ldi r25,lo8(14)	 ; ,
	ldi r26,0	 ; 
	ldi r27,0	 ; 
.L13:
	sts presc1,r24	 ;  presc1, tmp51
	sts presc1+1,r25	 ;  presc1, tmp51
	sts presc1+2,r26	 ;  presc1, tmp51
	sts presc1+3,r27	 ;  presc1, tmp51
	ret
	.size	timer1_prescaler, .-timer1_prescaler
.Lscope3:
	.stabs	"",36,0,0,.Lscope3-.LFBB3
	.stabd	78,0,0
	.stabs	"timer1_stop:F(0,47)",36,0,36,timer1_stop
.global	timer1_stop
	.type	timer1_stop, @function
timer1_stop:
	.stabd	46,0,0
	.stabn	68,0,36,.LM15-.LFBB4
.LM15:
.LFBB4:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	.stabn	68,0,37,.LM16-.LFBB4
.LM16:
	in r24,0x2e	 ;  tccr1b.2, MEM[(volatile uint8_t *)78B]
	sts tccr1b,r24	 ;  tccr1b, tccr1b.2
	.stabn	68,0,38,.LM17-.LFBB4
.LM17:
	in r24,0x2e	 ;  D.1719, MEM[(volatile uint8_t *)78B]
	andi r24,lo8(-8)	 ;  D.1719,
	out 0x2e,r24	 ;  MEM[(volatile uint8_t *)78B], D.1719
	ret
	.size	timer1_stop, .-timer1_stop
.Lscope4:
	.stabs	"",36,0,0,.Lscope4-.LFBB4
	.stabd	78,0,0
	.stabs	"timer1_continue:F(0,47)",36,0,41,timer1_continue
.global	timer1_continue
	.type	timer1_continue, @function
timer1_continue:
	.stabd	46,0,0
	.stabn	68,0,41,.LM18-.LFBB5
.LM18:
.LFBB5:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	.stabn	68,0,42,.LM19-.LFBB5
.LM19:
	lds r24,tccr1b	 ;  tccr1b.3, tccr1b
	out 0x2e,r24	 ;  MEM[(volatile uint8_t *)78B], tccr1b.3
	ret
	.size	timer1_continue, .-timer1_continue
.Lscope5:
	.stabs	"",36,0,0,.Lscope5-.LFBB5
	.stabd	78,0,0
	.stabs	"get_time_Lword:F(6,4)",36,0,45,get_time_Lword
.global	get_time_Lword
	.type	get_time_Lword, @function
get_time_Lword:
	.stabd	46,0,0
	.stabn	68,0,45,.LM20-.LFBB6
.LM20:
.LFBB6:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	.stabn	68,0,47,.LM21-.LFBB6
.LM21:
	in r18,__SREG__	 ;  temp, MEM[(volatile uint8_t *)95B]
	.stabn	68,0,48,.LM22-.LFBB6
.LM22:
/* #APP */
 ;  48 "../timer/timer.c" 1
	cli
 ;  0 "" 2
	.stabn	68,0,49,.LM23-.LFBB6
.LM23:
/* #NOAPP */
	in r24,0x2c	 ;  res, MEM[(volatile uint16_t *)76B]
	in r25,0x2c+1	 ;  res, MEM[(volatile uint16_t *)76B]
	.stabn	68,0,50,.LM24-.LFBB6
.LM24:
/* #APP */
 ;  50 "../timer/timer.c" 1
	sei
 ;  0 "" 2
	.stabn	68,0,51,.LM25-.LFBB6
.LM25:
/* #NOAPP */
	out __SREG__,r18	 ;  MEM[(volatile uint8_t *)95B], temp
	.stabn	68,0,52,.LM26-.LFBB6
.LM26:
	ret
	.size	get_time_Lword, .-get_time_Lword
	.stabs	"res:r(6,4)",64,0,46,24
	.stabs	"temp:r(6,2)",64,0,47,18
	.stabn	192,0,0,.LFBB6-.LFBB6
	.stabn	224,0,0,.Lscope6-.LFBB6
.Lscope6:
	.stabs	"",36,0,0,.Lscope6-.LFBB6
	.stabd	78,0,0
	.stabs	"set_time_Lword:F(0,47)",36,0,54,set_time_Lword
	.stabs	"time:P(6,4)",64,0,54,24
.global	set_time_Lword
	.type	set_time_Lword, @function
set_time_Lword:
	.stabd	46,0,0
	.stabn	68,0,54,.LM27-.LFBB7
.LM27:
.LFBB7:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	.stabn	68,0,55,.LM28-.LFBB7
.LM28:
	in r18,__SREG__	 ;  temp, MEM[(volatile uint8_t *)95B]
	.stabn	68,0,56,.LM29-.LFBB7
.LM29:
/* #APP */
 ;  56 "../timer/timer.c" 1
	cli
 ;  0 "" 2
	.stabn	68,0,57,.LM30-.LFBB7
.LM30:
/* #NOAPP */
	out 0x2c+1,r25	 ;  MEM[(volatile uint16_t *)76B], time
	out 0x2c,r24	 ;  MEM[(volatile uint16_t *)76B], time
	.stabn	68,0,58,.LM31-.LFBB7
.LM31:
/* #APP */
 ;  58 "../timer/timer.c" 1
	sei
 ;  0 "" 2
	.stabn	68,0,59,.LM32-.LFBB7
.LM32:
/* #NOAPP */
	out __SREG__,r18	 ;  MEM[(volatile uint8_t *)95B], temp
	ret
	.size	set_time_Lword, .-set_time_Lword
	.stabs	"temp:r(6,2)",64,0,55,18
	.stabn	192,0,0,.LFBB7-.LFBB7
	.stabn	224,0,0,.Lscope7-.LFBB7
.Lscope7:
	.stabs	"",36,0,0,.Lscope7-.LFBB7
	.stabd	78,0,0
	.stabs	"set_output_comp:F(0,47)",36,0,62,set_output_comp
	.stabs	"val:P(6,4)",64,0,62,24
	.stabs	"Timer:P(6,2)",64,0,62,22
.global	set_output_comp
	.type	set_output_comp, @function
set_output_comp:
	.stabd	46,0,0
	.stabn	68,0,62,.LM33-.LFBB8
.LM33:
.LFBB8:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	.stabn	68,0,63,.LM34-.LFBB8
.LM34:
	in r18,__SREG__	 ;  temp, MEM[(volatile uint8_t *)95B]
	.stabn	68,0,64,.LM35-.LFBB8
.LM35:
/* #APP */
 ;  64 "../timer/timer.c" 1
	cli
 ;  0 "" 2
	.stabn	68,0,65,.LM36-.LFBB8
.LM36:
/* #NOAPP */
	cpi r22,lo8(1)	 ;  Timer,
	brne .L19	 ; ,
	.stabn	68,0,65,.LM37-.LFBB8
.LM37:
	out 0x2a+1,r25	 ;  MEM[(volatile uint16_t *)74B], val
	out 0x2a,r24	 ;  MEM[(volatile uint16_t *)74B], val
	rjmp .L20	 ; 
.L19:
	.stabn	68,0,66,.LM38-.LFBB8
.LM38:
	cpi r22,lo8(2)	 ;  Timer,
	brne .L20	 ; ,
	.stabn	68,0,66,.LM39-.LFBB8
.LM39:
	out 0x28+1,r25	 ;  MEM[(volatile uint16_t *)72B], val
	out 0x28,r24	 ;  MEM[(volatile uint16_t *)72B], val
.L20:
	.stabn	68,0,67,.LM40-.LFBB8
.LM40:
/* #APP */
 ;  67 "../timer/timer.c" 1
	sei
 ;  0 "" 2
	.stabn	68,0,68,.LM41-.LFBB8
.LM41:
/* #NOAPP */
	out __SREG__,r18	 ;  MEM[(volatile uint8_t *)95B], temp
	ret
	.size	set_output_comp, .-set_output_comp
	.stabs	"temp:r(6,2)",64,0,63,18
	.stabn	192,0,0,.LFBB8-.LFBB8
	.stabn	224,0,0,.Lscope8-.LFBB8
.Lscope8:
	.stabs	"",36,0,0,.Lscope8-.LFBB8
	.stabd	78,0,0
.global	__floatunsisf
.global	__divsf3
	.stabs	"get_time_sec:F(0,12)",36,0,71,get_time_sec
.global	get_time_sec
	.type	get_time_sec, @function
get_time_sec:
	.stabd	46,0,0
	.stabn	68,0,71,.LM42-.LFBB9
.LM42:
.LFBB9:
	push r8	 ; 
	push r9	 ; 
	push r10	 ; 
	push r11	 ; 
	push r12	 ; 
	push r13	 ; 
	push r14	 ; 
	push r15	 ; 
	push r28	 ; 
/* prologue: function */
/* frame size = 0 */
/* stack size = 9 */
.L__stack_usage = 9
	.stabn	68,0,72,.LM43-.LFBB9
.LM43:
	in r28,__SREG__	 ;  temp, MEM[(volatile uint8_t *)95B]
	.stabn	68,0,73,.LM44-.LFBB9
.LM44:
/* #APP */
 ;  73 "../timer/timer.c" 1
	cli
 ;  0 "" 2
	.stabn	68,0,75,.LM45-.LFBB9
.LM45:
/* #NOAPP */
	rcall get_time_Lword	 ; 
	movw r20,r24	 ;  current_time,
	.stabn	68,0,76,.LM46-.LFBB9
.LM46:
	lds r24,time_Hword	 ;  time_Hword.4, time_Hword
	lds r25,time_Hword+1	 ;  time_Hword.4, time_Hword
	movw r22,r24	 ;  current_time, time_Hword.4
	.stabn	68,0,78,.LM47-.LFBB9
.LM47:
	lds r8,presc1	 ;  presc1.5, presc1
	lds r9,presc1+1	 ;  presc1.5, presc1
	lds r10,presc1+2	 ;  presc1.5, presc1
	lds r11,presc1+3	 ;  presc1.5, presc1
	movw r24,r22	 ; , current_time
	movw r22,r20	 ; , current_time
	rcall __floatunsisf	 ; 
	movw r12,r22	 ;  D.1732,
	movw r14,r24	 ;  D.1732,
	movw r24,r10	 ; , presc1.5
	movw r22,r8	 ; , presc1.5
	rcall __floatunsisf	 ; 
	movw r18,r22	 ;  D.1732,
	movw r20,r24	 ;  D.1732,
	movw r24,r14	 ; , D.1732
	movw r22,r12	 ; , D.1732
	rcall __divsf3	 ; 
	.stabn	68,0,79,.LM48-.LFBB9
.LM48:
/* #APP */
 ;  79 "../timer/timer.c" 1
	sei
 ;  0 "" 2
	.stabn	68,0,80,.LM49-.LFBB9
.LM49:
/* #NOAPP */
	out __SREG__,r28	 ;  MEM[(volatile uint8_t *)95B], temp
/* epilogue start */
	.stabn	68,0,82,.LM50-.LFBB9
.LM50:
	pop r28	 ; 
	pop r15	 ; 
	pop r14	 ; 
	pop r13	 ; 
	pop r12	 ; 
	pop r11	 ; 
	pop r10	 ; 
	pop r9	 ; 
	pop r8	 ; 
	ret
	.size	get_time_sec, .-get_time_sec
	.stabs	"temp:r(6,2)",64,0,72,28
	.stabs	"current_time:r(1,1)",64,0,74,20
	.stabn	192,0,0,.LFBB9-.LFBB9
	.stabn	224,0,0,.Lscope9-.LFBB9
.Lscope9:
	.stabs	"",36,0,0,.Lscope9-.LFBB9
	.stabd	78,0,0
	.stabs	"reset_timer32:F(0,47)",36,0,84,reset_timer32
.global	reset_timer32
	.type	reset_timer32, @function
reset_timer32:
	.stabd	46,0,0
	.stabn	68,0,84,.LM51-.LFBB10
.LM51:
.LFBB10:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	.stabn	68,0,85,.LM52-.LFBB10
.LM52:
	ldi r24,0	 ; 
	ldi r25,0	 ; 
	rcall set_time_Lword	 ; 
	.stabn	68,0,86,.LM53-.LFBB10
.LM53:
	sts time_Hword+1,__zero_reg__	 ;  time_Hword,
	sts time_Hword,__zero_reg__	 ;  time_Hword,
	ret
	.size	reset_timer32, .-reset_timer32
.Lscope10:
	.stabs	"",36,0,0,.Lscope10-.LFBB10
	.stabd	78,0,0
	.stabs	"start_timer32:F(0,47)",36,0,89,start_timer32
.global	start_timer32
	.type	start_timer32, @function
start_timer32:
	.stabd	46,0,0
	.stabn	68,0,89,.LM54-.LFBB11
.LM54:
.LFBB11:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	.stabn	68,0,90,.LM55-.LFBB11
.LM55:
	ldi r24,0	 ; 
	rcall timer1_mode	 ; 
	.stabn	68,0,91,.LM56-.LFBB11
.LM56:
	ldi r24,lo8(3)	 ; ,
	rcall timer1_prescaler	 ; 
	.stabn	68,0,92,.LM57-.LFBB11
.LM57:
	in r24,0x39	 ;  D.1739, MEM[(volatile uint8_t *)89B]
	ori r24,lo8(4)	 ;  D.1739,
	out 0x39,r24	 ;  MEM[(volatile uint8_t *)89B], D.1739
	.stabn	68,0,93,.LM58-.LFBB11
.LM58:
	rcall reset_timer32	 ; 
	.stabn	68,0,94,.LM59-.LFBB11
.LM59:
/* #APP */
 ;  94 "../timer/timer.c" 1
	sei
 ;  0 "" 2
/* #NOAPP */
	ret
	.size	start_timer32, .-start_timer32
.Lscope11:
	.stabs	"",36,0,0,.Lscope11-.LFBB11
	.stabd	78,0,0
	.stabs	"get_time_Hword:F(6,4)",36,0,97,get_time_Hword
.global	get_time_Hword
	.type	get_time_Hword, @function
get_time_Hword:
	.stabd	46,0,0
	.stabn	68,0,97,.LM60-.LFBB12
.LM60:
.LFBB12:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	.stabn	68,0,98,.LM61-.LFBB12
.LM61:
	lds r24,time_Hword	 ;  D.1742, time_Hword
	lds r25,time_Hword+1	 ;  D.1742, time_Hword
	.stabn	68,0,99,.LM62-.LFBB12
.LM62:
	ret
	.size	get_time_Hword, .-get_time_Hword
.Lscope12:
	.stabs	"",36,0,0,.Lscope12-.LFBB12
	.stabd	78,0,0
	.stabs	"set_pwm_channel_A:F(0,47)",36,0,101,set_pwm_channel_A
	.stabs	"fracA:p(0,48)=B(6,4)",160,0,101,2
.global	set_pwm_channel_A
	.type	set_pwm_channel_A, @function
set_pwm_channel_A:
	.stabd	46,0,0
	.stabn	68,0,101,.LM63-.LFBB13
.LM63:
.LFBB13:
	push r28	 ; 
	push r29	 ; 
	 ; SP -= 3	 ; 
	rcall .
	push __zero_reg__
	in r28,__SP_L__	 ; 
	in r29,__SP_H__	 ; 
/* prologue: function */
/* frame size = 3 */
/* stack size = 5 */
.L__stack_usage = 5
	std Y+3,r25	 ;  fracA, fracA
	std Y+2,r24	 ;  fracA, fracA
	.stabn	68,0,102,.LM64-.LFBB13
.LM64:
	in r24,__SREG__	 ;  temp.6, MEM[(volatile uint8_t *)95B]
	std Y+1,r24	 ;  temp, temp.6
	.stabn	68,0,103,.LM65-.LFBB13
.LM65:
/* #APP */
 ;  103 "../timer/timer.c" 1
	cli
 ;  0 "" 2
	.stabn	68,0,108,.LM66-.LFBB13
.LM66:
/* #NOAPP */
	ldd r24,Y+2	 ;  fracA.7, fracA
	ldd r25,Y+3	 ;  fracA.7, fracA
	ldi r22,lo8(10)	 ; ,
	ldi r23,0	 ; 
	rcall __udivmodhi4
	std Y+3,r23	 ;  fracA, tmp53
	std Y+2,r22	 ;  fracA, tmp53
	.stabn	68,0,110,.LM67-.LFBB13
.LM67:
	lds r18,pwm_TOP	 ;  pwm_TOP.9, pwm_TOP
	lds r19,pwm_TOP+1	 ;  pwm_TOP.9, pwm_TOP
	lds r20,pwm_TOP+2	 ;  pwm_TOP.9, pwm_TOP
	lds r21,pwm_TOP+3	 ;  pwm_TOP.9, pwm_TOP
	ldd r26,Y+2	 ;  fracA.10, fracA
	ldd r27,Y+3	 ;  fracA.10, fracA
	rcall __muluhisi3
	ldi r18,lo8(-1)	 ; ,
	ldi r19,lo8(-1)	 ; ,
	ldi r20,0	 ; 
	ldi r21,0	 ; 
	rcall __udivmodsi4
	.stabn	68,0,111,.LM68-.LFBB13
.LM68:
	out 0x2a+1,r19	 ;  MEM[(volatile uint16_t *)74B], count
	out 0x2a,r18	 ;  MEM[(volatile uint16_t *)74B], count
	.stabn	68,0,112,.LM69-.LFBB13
.LM69:
	ldd r24,Y+1	 ;  temp.11, temp
	out __SREG__,r24	 ;  MEM[(volatile uint8_t *)95B], temp.11
/* epilogue start */
	.stabn	68,0,113,.LM70-.LFBB13
.LM70:
	 ; SP += 3	 ; 
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop r29	 ; 
	pop r28	 ; 
	ret
	.size	set_pwm_channel_A, .-set_pwm_channel_A
	.stabs	"temp:(0,49)=B(6,2)",128,0,102,1
	.stabn	192,0,0,.LFBB13-.LFBB13
	.stabn	224,0,0,.Lscope13-.LFBB13
.Lscope13:
	.stabs	"",36,0,0,.Lscope13-.LFBB13
	.stabd	78,0,0
	.stabs	"set_pwm_channel_B:F(0,47)",36,0,115,set_pwm_channel_B
	.stabs	"fracB:P(6,4)",64,0,115,24
.global	set_pwm_channel_B
	.type	set_pwm_channel_B, @function
set_pwm_channel_B:
	.stabd	46,0,0
	.stabn	68,0,115,.LM71-.LFBB14
.LM71:
.LFBB14:
	push r28	 ; 
/* prologue: function */
/* frame size = 0 */
/* stack size = 1 */
.L__stack_usage = 1
	.stabn	68,0,116,.LM72-.LFBB14
.LM72:
	in r28,__SREG__	 ;  temp, MEM[(volatile uint8_t *)95B]
	.stabn	68,0,117,.LM73-.LFBB14
.LM73:
/* #APP */
 ;  117 "../timer/timer.c" 1
	cli
 ;  0 "" 2
	.stabn	68,0,119,.LM74-.LFBB14
.LM74:
/* #NOAPP */
	lds r18,pwm_TOP	 ;  pwm_TOP.12, pwm_TOP
	lds r19,pwm_TOP+1	 ;  pwm_TOP.12, pwm_TOP
	lds r20,pwm_TOP+2	 ;  pwm_TOP.12, pwm_TOP
	lds r21,pwm_TOP+3	 ;  pwm_TOP.12, pwm_TOP
	movw r26,r24	 ; , fracB
	rcall __muluhisi3
	ldi r18,lo8(-1)	 ; ,
	ldi r19,lo8(-1)	 ; ,
	ldi r20,0	 ; 
	ldi r21,0	 ; 
	rcall __udivmodsi4
	.stabn	68,0,120,.LM75-.LFBB14
.LM75:
	out 0x28+1,r19	 ;  MEM[(volatile uint16_t *)72B], count
	out 0x28,r18	 ;  MEM[(volatile uint16_t *)72B], count
	.stabn	68,0,121,.LM76-.LFBB14
.LM76:
	out __SREG__,r28	 ;  MEM[(volatile uint8_t *)95B], temp
/* epilogue start */
	.stabn	68,0,122,.LM77-.LFBB14
.LM77:
	pop r28	 ; 
	ret
	.size	set_pwm_channel_B, .-set_pwm_channel_B
	.stabs	"temp:r(6,2)",64,0,116,28
	.stabn	192,0,0,.LFBB14-.LFBB14
	.stabn	224,0,0,.Lscope14-.LFBB14
.Lscope14:
	.stabs	"",36,0,0,.Lscope14-.LFBB14
	.stabd	78,0,0
	.stabs	"x1_less_x2_16:F(6,2)",36,0,124,x1_less_x2_16
.global	x1_less_x2_16
	.type	x1_less_x2_16, @function
x1_less_x2_16:
	.stabd	46,0,0
	.stabn	68,0,124,.LM78-.LFBB15
.LM78:
.LFBB15:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	.stabn	68,0,125,.LM79-.LFBB15
.LM79:
	mov r18,r24	 ;  X1, x1
	.stabn	68,0,127,.LM80-.LFBB15
.LM80:
	cp r25,r23	 ;  X1, X2
	brlo .L30	 ; ,
	.stabn	68,0,128,.LM81-.LFBB15
.LM81:
	cp r23,r25	 ;  X2, X1
	brlo .L31	 ; ,
	.stabn	68,0,129,.LM82-.LFBB15
.LM82:
	ldi r24,lo8(1)	 ;  D.1755,
	cp r18,r22	 ;  X1, X2
	brlo .L28	 ; ,
	rjmp .L31	 ; 
.L30:
	.stabn	68,0,127,.LM83-.LFBB15
.LM83:
	ldi r24,lo8(1)	 ;  D.1755,
	ret
.L31:
	.stabn	68,0,128,.LM84-.LFBB15
.LM84:
	ldi r24,0	 ;  D.1755
.L28:
	.stabn	68,0,131,.LM85-.LFBB15
.LM85:
	ret
	.size	x1_less_x2_16, .-x1_less_x2_16
.Lscope15:
	.stabs	"",36,0,0,.Lscope15-.LFBB15
	.stabd	78,0,0
	.stabs	"init_pwm_motor_control:F(0,47)",36,0,133,init_pwm_motor_control
	.stabs	"freq:P(6,4)",64,0,133,24
	.stabs	"fracA:P(6,4)",64,0,133,16
	.stabs	"fracB:P(6,4)",64,0,133,12
.global	init_pwm_motor_control
	.type	init_pwm_motor_control, @function
init_pwm_motor_control:
	.stabd	46,0,0
	.stabn	68,0,133,.LM86-.LFBB16
.LM86:
.LFBB16:
	push r12	 ; 
	push r13	 ; 
	push r15	 ; 
	push r16	 ; 
	push r17	 ; 
	push r28	 ; 
	push r29	 ; 
	 ; SP -= 4	 ; 
	rcall .
	rcall .
	in r28,__SP_L__	 ; 
	in r29,__SP_H__	 ; 
/* prologue: function */
/* frame size = 4 */
/* stack size = 11 */
.L__stack_usage = 11
	movw r16,r22	 ;  fracA, fracA
	movw r12,r20	 ;  fracB, fracB
	.stabn	68,0,134,.LM87-.LFBB16
.LM87:
	in r15,__SREG__	 ;  temp, MEM[(volatile uint8_t *)95B]
	.stabn	68,0,135,.LM88-.LFBB16
.LM88:
/* #APP */
 ;  135 "../timer/timer.c" 1
	cli
 ;  0 "" 2
	.stabn	68,0,137,.LM89-.LFBB16
.LM89:
/* #NOAPP */
	movw r18,r24	 ;  D.1760, freq
	ldi r20,0	 ;  D.1760
	ldi r21,0	 ;  D.1760
	ldi r22,0	 ; 
	ldi r23,lo8(-128)	 ; ,
	ldi r24,lo8(112)	 ; ,
	ldi r25,0	 ; 
	rcall __udivmodsi4
	.stabn	68,0,139,.LM90-.LFBB16
.LM90:
	cp r18,__zero_reg__	 ;  max
	cpc r19,__zero_reg__	 ;  max
	ldi r24,1	 ; ,
	cpc r20,r24	 ;  max,
	cpc r21,__zero_reg__	 ;  max
	brlo .L34	 ; ,
	.stabn	68,0,140,.LM91-.LFBB16
.LM91:
	ldi r22,3	 ; ,
	1:
	lsr r21	 ;  max
	ror r20	 ;  max
	ror r19	 ;  max
	ror r18	 ;  max
	dec r22	 ; 
	brne 1b
	.stabn	68,0,144,.LM92-.LFBB16
.LM92:
	cp r18,__zero_reg__	 ;  max
	cpc r19,__zero_reg__	 ;  max
	ldi r30,1	 ; ,
	cpc r20,r30	 ;  max,
	cpc r21,__zero_reg__	 ;  max
	brlo .L35	 ; ,
	.stabn	68,0,145,.LM93-.LFBB16
.LM93:
	ldi r25,3	 ; ,
	1:
	lsr r21	 ;  max
	ror r20	 ;  max
	ror r19	 ;  max
	ror r18	 ;  max
	dec r25	 ; 
	brne 1b
	.stabn	68,0,149,.LM94-.LFBB16
.LM94:
	cp r18,__zero_reg__	 ;  max
	cpc r19,__zero_reg__	 ;  max
	ldi r31,1	 ; ,
	cpc r20,r31	 ;  max,
	cpc r21,__zero_reg__	 ;  max
	brlo .L36	 ; ,
	.stabn	68,0,150,.LM95-.LFBB16
.LM95:
	ldi r24,2	 ; ,
	1:
	lsr r21	 ;  max
	ror r20	 ;  max
	ror r19	 ;  max
	ror r18	 ;  max
	dec r24	 ; 
	brne 1b
	.stabn	68,0,151,.LM96-.LFBB16
.LM96:
	ldi r24,lo8(4)	 ;  presc_mode,
	rjmp .L33	 ; 
.L34:
	.stabn	68,0,138,.LM97-.LFBB16
.LM97:
	ldi r24,lo8(1)	 ;  presc_mode,
	rjmp .L33	 ; 
.L35:
	.stabn	68,0,141,.LM98-.LFBB16
.LM98:
	ldi r24,lo8(2)	 ;  presc_mode,
	rjmp .L33	 ; 
.L36:
	.stabn	68,0,146,.LM99-.LFBB16
.LM99:
	ldi r24,lo8(3)	 ;  presc_mode,
.L33:
	.stabn	68,0,160,.LM100-.LFBB16
.LM100:
	sts pwm_TOP,r18	 ;  pwm_TOP, max
	sts pwm_TOP+1,r19	 ;  pwm_TOP, max
	sts pwm_TOP+2,r20	 ;  pwm_TOP, max
	sts pwm_TOP+3,r21	 ;  pwm_TOP, max
	.stabn	68,0,161,.LM101-.LFBB16
.LM101:
	ldi r25,lo8(-94)	 ;  tmp56,
	out 0x2f,r25	 ;  MEM[(volatile uint8_t *)79B], tmp56
	.stabn	68,0,162,.LM102-.LFBB16
.LM102:
	ldi r25,lo8(24)	 ;  tmp58,
	out 0x2e,r25	 ;  MEM[(volatile uint8_t *)78B], tmp58
	.stabn	68,0,163,.LM103-.LFBB16
.LM103:
	std Y+1,r18	 ; ,
	std Y+2,r19	 ; ,
	std Y+3,r20	 ; ,
	std Y+4,r21	 ; ,
	rcall timer1_prescaler	 ; 
	.stabn	68,0,166,.LM104-.LFBB16
.LM104:
	ldd r18,Y+1	 ; ,
	ldd r19,Y+2	 ; ,
	out 0x26+1,r19	 ;  MEM[(volatile uint16_t *)70B], max
	out 0x26,r18	 ;  MEM[(volatile uint16_t *)70B], max
	.stabn	68,0,167,.LM105-.LFBB16
.LM105:
	movw r24,r16	 ; , fracA
	rcall set_pwm_channel_A	 ; 
	.stabn	68,0,168,.LM106-.LFBB16
.LM106:
	movw r24,r12	 ; , fracB
	rcall set_pwm_channel_B	 ; 
	.stabn	68,0,170,.LM107-.LFBB16
.LM107:
	out 0x2c+1,__zero_reg__	 ;  MEM[(volatile uint16_t *)76B],
	out 0x2c,__zero_reg__	 ;  MEM[(volatile uint16_t *)76B],
	.stabn	68,0,171,.LM108-.LFBB16
.LM108:
	out __SREG__,r15	 ;  MEM[(volatile uint8_t *)95B], temp
/* epilogue start */
	.stabn	68,0,172,.LM109-.LFBB16
.LM109:
	 ; SP += 4	 ; 
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop r29	 ; 
	pop r28	 ; 
	pop r17	 ; 
	pop r16	 ; 
	pop r15	 ; 
	pop r13	 ; 
	pop r12	 ; 
	ret
	.size	init_pwm_motor_control, .-init_pwm_motor_control
	.stabs	"temp:r(6,2)",64,0,134,15
	.stabs	"max:r(6,6)",64,0,137,18
	.stabs	"presc_mode:r(6,2)",64,0,138,24
	.stabn	192,0,0,.LFBB16-.LFBB16
	.stabn	224,0,0,.Lscope16-.LFBB16
.Lscope16:
	.stabs	"",36,0,0,.Lscope16-.LFBB16
	.stabd	78,0,0
	.data
	.type	presc1, @object
	.size	presc1, 4
presc1:
	.byte	0
	.byte	64
	.byte	56
	.byte	0
	.type	pwm_TOP, @object
	.size	pwm_TOP, 4
pwm_TOP:
	.byte	-1
	.byte	-1
	.byte	0
	.byte	0
	.local	time_Hword
	.comm	time_Hword,2,1
	.local	tccr1b
	.comm	tccr1b,1,1
.global	MAX
	.section	.rodata
	.type	MAX, @object
	.size	MAX, 4
MAX:
	.byte	-1
	.byte	-1
	.byte	0
	.byte	0
	.stabs	"tccr1b:S(0,49)",40,0,5,tccr1b
	.stabs	"time_Hword:S(0,48)",38,0,6,time_Hword
	.stabs	"pwm_TOP:S(0,50)=B(6,6)",38,0,7,pwm_TOP
	.stabs	"presc1:S(0,50)",38,0,8,presc1
	.stabs	"MAX:G(0,51)=k(6,6)",32,0,3,0
	.text
	.stabs	"",100,0,0,.Letext0
.Letext0:
	.ident	"GCC: (GNU) 4.8.2"
.global __do_copy_data
.global __do_clear_bss
