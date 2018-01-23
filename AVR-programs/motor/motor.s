	.file	"motor.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
 ;  GNU C (GCC) version 4.8.2 (avr)
 ; 	compiled by GNU C version 4.8.2, GMP version 5.1.3, MPFR version 3.1.2-p3, MPC version 1.0.1
 ;  GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
 ;  options passed:  -fpreprocessed motor.i -mmcu=atmega8
 ;  -auxbase-strip motor.o -g -Os -fverbose-asm
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
	.stabs	"motor.c",100,0,2,.Ltext0
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
	.stabs	"/usr/lib/avr/include/avr/io.h",130,0,0,0
	.stabs	"/usr/lib/avr/include/avr/sfr_defs.h",130,0,0,0
	.stabs	"/usr/lib/avr/include/inttypes.h",130,0,0,0
	.stabs	"/usr/lib/gcc/avr/4.8.2/include/stdint.h",130,0,0,0
	.stabs	"/usr/lib/avr/include/stdint.h",130,0,0,0
	.stabs	"int8_t:t(5,1)=(0,10)",128,0,121,0
	.stabs	"uint8_t:t(5,2)=(0,11)",128,0,122,0
	.stabs	"int16_t:t(5,3)=(0,1)",128,0,123,0
	.stabs	"uint16_t:t(5,4)=(0,4)",128,0,124,0
	.stabs	"int32_t:t(5,5)=(0,3)",128,0,125,0
	.stabs	"uint32_t:t(5,6)=(0,5)",128,0,126,0
	.stabs	"int64_t:t(5,7)=(0,6)",128,0,128,0
	.stabs	"uint64_t:t(5,8)=(0,7)",128,0,129,0
	.stabs	"intptr_t:t(5,9)=(5,3)",128,0,142,0
	.stabs	"uintptr_t:t(5,10)=(5,4)",128,0,147,0
	.stabs	"int_least8_t:t(5,11)=(5,1)",128,0,159,0
	.stabs	"uint_least8_t:t(5,12)=(5,2)",128,0,164,0
	.stabs	"int_least16_t:t(5,13)=(5,3)",128,0,169,0
	.stabs	"uint_least16_t:t(5,14)=(5,4)",128,0,174,0
	.stabs	"int_least32_t:t(5,15)=(5,5)",128,0,179,0
	.stabs	"uint_least32_t:t(5,16)=(5,6)",128,0,184,0
	.stabs	"int_least64_t:t(5,17)=(5,7)",128,0,192,0
	.stabs	"uint_least64_t:t(5,18)=(5,8)",128,0,199,0
	.stabs	"int_fast8_t:t(5,19)=(5,1)",128,0,213,0
	.stabs	"uint_fast8_t:t(5,20)=(5,2)",128,0,218,0
	.stabs	"int_fast16_t:t(5,21)=(5,3)",128,0,223,0
	.stabs	"uint_fast16_t:t(5,22)=(5,4)",128,0,228,0
	.stabs	"int_fast32_t:t(5,23)=(5,5)",128,0,233,0
	.stabs	"uint_fast32_t:t(5,24)=(5,6)",128,0,238,0
	.stabs	"int_fast64_t:t(5,25)=(5,7)",128,0,246,0
	.stabs	"uint_fast64_t:t(5,26)=(5,8)",128,0,253,0
	.stabs	"intmax_t:t(5,27)=(5,7)",128,0,273,0
	.stabs	"uintmax_t:t(5,28)=(5,8)",128,0,278,0
	.stabn	162,0,0,0
	.stabn	162,0,0,0
	.stabs	"int_farptr_t:t(3,1)=(5,5)",128,0,77,0
	.stabs	"uint_farptr_t:t(3,2)=(5,6)",128,0,81,0
	.stabn	162,0,0,0
	.stabn	162,0,0,0
	.stabs	"/usr/lib/avr/include/avr/fuse.h",130,0,0,0
	.stabs	"__fuse_t:t(6,1)=(6,2)=s2low:(0,11),0,8;high:(0,11),8,8;;",128,0,252,0
	.stabn	162,0,0,0
	.stabn	162,0,0,0
	.stabs	"/usr/lib/avr/include/avr/eeprom.h",130,0,0,0
	.stabs	"/usr/lib/gcc/avr/4.8.2/include/stddef.h",130,0,0,0
	.stabs	"ptrdiff_t:t(8,1)=(0,1)",128,0,147,0
	.stabs	"size_t:t(8,2)=(0,4)",128,0,212,0
	.stabs	"wchar_t:t(8,3)=(0,1)",128,0,324,0
	.stabn	162,0,0,0
	.stabn	162,0,0,0
	.stabs	"../timer/timer.h",130,0,0,0
	.stabs	"time32:T(9,1)=u4t32:(5,6),0,32;t16:(9,2)=s4L:(5,4),0,16;H:(5,4),16,16;;,0,32;;",128,0,0,0
	.stabs	"time16:T(9,3)=u2t16:(5,4),0,16;t8:(9,4)=s2L:(5,2),0,8;H:(5,2),8,8;;,0,16;;",128,0,0,0
	.stabn	162,0,0,0
	.stabs	"../twi/twi.h",130,0,0,0
	.stabs	"u16:T(10,1)=u2t16:(5,4),0,16;t8:(10,2)=ar(10,3)=r(10,3);0;0177777;;0;1;(5,2),0,16;;",128,0,0,0
	.stabn	162,0,0,0
	.stabs	"__vector_1:F(0,47)",36,0,24,__vector_1
.global	__vector_1
	.type	__vector_1, @function
__vector_1:
	.stabd	46,0,0
	.stabn	68,0,24,.LM0-.LFBB1
.LM0:
.LFBB1:
	push r1	 ; 
	push r0	 ; 
	in r0,__SREG__	 ; ,
	push r0	 ; 
	clr __zero_reg__	 ; 
	push r18	 ; 
	push r19	 ; 
	push r20	 ; 
	push r21	 ; 
	push r22	 ; 
	push r23	 ; 
	push r24	 ; 
	push r25	 ; 
	push r26	 ; 
	push r27	 ; 
	push r30	 ; 
	push r31	 ; 
/* prologue: Signal */
/* frame size = 0 */
/* stack size = 15 */
.L__stack_usage = 15
	.stabn	68,0,25,.LM1-.LFBB1
.LM1:
	in r24,0x4	 ;  temp.0, MEM[(volatile uint16_t *)36B]
	in r25,0x4+1	 ;  temp.0, MEM[(volatile uint16_t *)36B]
	sts temp+1,r25	 ;  temp, temp.0
	sts temp,r24	 ;  temp, temp.0
	.stabn	68,0,26,.LM2-.LFBB1
.LM2:
	lds r22,temp	 ;  temp.1, temp
	lds r23,temp+1	 ;  temp.1, temp
	ldi r24,lo8(MIN_SAVE)	 ; ,
	ldi r25,hi8(MIN_SAVE)	 ; ,
	rcall __eewr_word_m8	 ; 
	.stabn	68,0,27,.LM3-.LFBB1
.LM3:
	sbis 0x15,0	 ; ,
	rjmp .L2	 ; 
	.stabn	68,0,27,.LM4-.LFBB1
.LM4:
	cbi 0x15,0	 ; ,
	rjmp .L1	 ; 
.L2:
	.stabn	68,0,27,.LM5-.LFBB1
.LM5:
	sbi 0x15,0	 ; ,
.L1:
/* epilogue start */
	.stabn	68,0,28,.LM6-.LFBB1
.LM6:
	pop r31	 ; 
	pop r30	 ; 
	pop r27	 ; 
	pop r26	 ; 
	pop r25	 ; 
	pop r24	 ; 
	pop r23	 ; 
	pop r22	 ; 
	pop r21	 ; 
	pop r20	 ; 
	pop r19	 ; 
	pop r18	 ; 
	pop r0	 ; 
	out __SREG__,r0	 ; ,
	pop r0	 ; 
	pop r1	 ; 
	reti
	.size	__vector_1, .-__vector_1
.Lscope1:
	.stabs	"",36,0,0,.Lscope1-.LFBB1
	.stabd	78,0,0
	.stabs	"__vector_2:F(0,47)",36,0,30,__vector_2
.global	__vector_2
	.type	__vector_2, @function
__vector_2:
	.stabd	46,0,0
	.stabn	68,0,30,.LM7-.LFBB2
.LM7:
.LFBB2:
	push r1	 ; 
	push r0	 ; 
	in r0,__SREG__	 ; ,
	push r0	 ; 
	clr __zero_reg__	 ; 
	push r18	 ; 
	push r19	 ; 
	push r20	 ; 
	push r21	 ; 
	push r22	 ; 
	push r23	 ; 
	push r24	 ; 
	push r25	 ; 
	push r26	 ; 
	push r27	 ; 
	push r30	 ; 
	push r31	 ; 
/* prologue: Signal */
/* frame size = 0 */
/* stack size = 15 */
.L__stack_usage = 15
	.stabn	68,0,31,.LM8-.LFBB2
.LM8:
	in r24,0x4	 ;  temp.2, MEM[(volatile uint16_t *)36B]
	in r25,0x4+1	 ;  temp.2, MEM[(volatile uint16_t *)36B]
	sts temp+1,r25	 ;  temp, temp.2
	sts temp,r24	 ;  temp, temp.2
	.stabn	68,0,32,.LM9-.LFBB2
.LM9:
	lds r22,temp	 ;  temp.3, temp
	lds r23,temp+1	 ;  temp.3, temp
	ldi r24,lo8(MAX_SAVE)	 ; ,
	ldi r25,hi8(MAX_SAVE)	 ; ,
	rcall __eewr_word_m8	 ; 
	.stabn	68,0,33,.LM10-.LFBB2
.LM10:
	sbis 0x15,0	 ; ,
	rjmp .L6	 ; 
	.stabn	68,0,33,.LM11-.LFBB2
.LM11:
	cbi 0x15,0	 ; ,
	rjmp .L5	 ; 
.L6:
	.stabn	68,0,33,.LM12-.LFBB2
.LM12:
	sbi 0x15,0	 ; ,
.L5:
/* epilogue start */
	.stabn	68,0,34,.LM13-.LFBB2
.LM13:
	pop r31	 ; 
	pop r30	 ; 
	pop r27	 ; 
	pop r26	 ; 
	pop r25	 ; 
	pop r24	 ; 
	pop r23	 ; 
	pop r22	 ; 
	pop r21	 ; 
	pop r20	 ; 
	pop r19	 ; 
	pop r18	 ; 
	pop r0	 ; 
	out __SREG__,r0	 ; ,
	pop r0	 ; 
	pop r1	 ; 
	reti
	.size	__vector_2, .-__vector_2
.Lscope2:
	.stabs	"",36,0,0,.Lscope2-.LFBB2
	.stabd	78,0,0
	.stabs	"get_ADC:F(5,4)",36,0,36,get_ADC
.global	get_ADC
	.type	get_ADC, @function
get_ADC:
	.stabd	46,0,0
	.stabn	68,0,36,.LM14-.LFBB3
.LM14:
.LFBB3:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	.stabn	68,0,37,.LM15-.LFBB3
.LM15:
	sbi 0x6,6	 ; ,
.L10:
	.stabn	68,0,38,.LM16-.LFBB3
.LM16:
	sbic 0x6,6	 ; ,
	rjmp .L10	 ; 
	.stabn	68,0,39,.LM17-.LFBB3
.LM17:
	in r24,0x4	 ;  res, MEM[(volatile uint16_t *)36B]
	in r25,0x4+1	 ;  res, MEM[(volatile uint16_t *)36B]
	.stabn	68,0,41,.LM18-.LFBB3
.LM18:
	ret
	.size	get_ADC, .-get_ADC
	.stabs	"res:r(5,4)",64,0,39,24
	.stabn	192,0,0,.LFBB3-.LFBB3
	.stabn	224,0,0,.Lscope3-.LFBB3
.Lscope3:
	.stabs	"",36,0,0,.Lscope3-.LFBB3
	.stabd	78,0,0
	.section	.text.startup,"ax",@progbits
	.stabs	"main:F(0,1)",36,0,43,main
.global	main
	.type	main, @function
main:
	.stabd	46,0,0
	.stabn	68,0,43,.LM19-.LFBB4
.LM19:
.LFBB4:
	push r28	 ; 
	push r29	 ; 
	 ; SP -= 6	 ; 
	rcall .
	rcall .
	rcall .
	in r28,__SP_L__	 ; 
	in r29,__SP_H__	 ; 
/* prologue: function */
/* frame size = 6 */
/* stack size = 8 */
.L__stack_usage = 8
	.stabn	68,0,45,.LM20-.LFBB4
.LM20:
	ldi r24,lo8(-13)	 ;  tmp66,
	out 0x11,r24	 ;  MEM[(volatile uint8_t *)49B], tmp66
	.stabn	68,0,46,.LM21-.LFBB4
.LM21:
	ldi r24,lo8(-1)	 ;  tmp68,
	out 0x12,r24	 ;  MEM[(volatile uint8_t *)50B], tmp68
	.stabn	68,0,48,.LM22-.LFBB4
.LM22:
	ldi r25,lo8(1)	 ;  tmp70,
	out 0x14,r25	 ;  MEM[(volatile uint8_t *)52B], tmp70
	.stabn	68,0,49,.LM23-.LFBB4
.LM23:
	ldi r25,lo8(-2)	 ;  tmp72,
	out 0x15,r25	 ;  MEM[(volatile uint8_t *)53B], tmp72
	.stabn	68,0,51,.LM24-.LFBB4
.LM24:
	ldi r25,lo8(6)	 ;  tmp74,
	out 0x17,r25	 ;  MEM[(volatile uint8_t *)55B], tmp74
	.stabn	68,0,52,.LM25-.LFBB4
.LM25:
	out 0x18,r24	 ;  MEM[(volatile uint8_t *)56B], tmp68
	.stabn	68,0,55,.LM26-.LFBB4
.LM26:
	in r24,0x35	 ;  D.1824, MEM[(volatile uint8_t *)85B]
	ori r24,lo8(15)	 ;  D.1824,
	out 0x35,r24	 ;  MEM[(volatile uint8_t *)85B], D.1824
	.stabn	68,0,56,.LM27-.LFBB4
.LM27:
	in r24,0x3b	 ;  D.1824, MEM[(volatile uint8_t *)91B]
	ori r24,lo8(-64)	 ;  D.1824,
	out 0x3b,r24	 ;  MEM[(volatile uint8_t *)91B], D.1824
	.stabn	68,0,59,.LM28-.LFBB4
.LM28:
	ldi r24,lo8(35)	 ;  tmp82,
	out 0x7,r24	 ;  MEM[(volatile uint8_t *)39B], tmp82
	.stabn	68,0,61,.LM29-.LFBB4
.LM29:
	ldi r24,lo8(-121)	 ;  tmp84,
	out 0x6,r24	 ;  MEM[(volatile uint8_t *)38B], tmp84
	.stabn	68,0,64,.LM30-.LFBB4
.LM30:
	ldi r24,lo8(2)	 ; ,
	rcall twi_init_slave	 ; 
	.stabn	68,0,66,.LM31-.LFBB4
.LM31:
	ldi r24,lo8(MIN_SAVE)	 ; ,
	ldi r25,hi8(MIN_SAVE)	 ; ,
	rcall __eerd_word_m8	 ; 
	std Y+6,r25	 ;  min_load, min_load.4
	std Y+5,r24	 ;  min_load, min_load.4
	.stabn	68,0,67,.LM32-.LFBB4
.LM32:
	ldi r24,lo8(MAX_SAVE)	 ; ,
	ldi r25,hi8(MAX_SAVE)	 ; ,
	rcall __eerd_word_m8	 ; 
	std Y+4,r25	 ;  max_load, max_load.5
	std Y+3,r24	 ;  max_load, max_load.5
	.stabn	68,0,68,.LM33-.LFBB4
.LM33:
	ldd r24,Y+3	 ;  D.1825, max_load
	ldd r25,Y+4	 ;  D.1825, max_load
	ldd r18,Y+5	 ;  min_load.7, min_load
	ldd r19,Y+6	 ;  min_load.7, min_load
	sub r24,r18	 ;  D.1825, min_load.7
	sbc r25,r19	 ;  D.1825, min_load.7
	ldi r22,lo8(-1)	 ; ,
	ldi r23,0	 ; 
	rcall __udivmodhi4
	sts scale+1,r23	 ;  scale, tmp86
	sts scale,r22	 ;  scale, tmp86
	.stabn	68,0,70,.LM34-.LFBB4
.LM34:
	ldd r20,Y+5	 ;  min_load.9, min_load
	ldd r21,Y+6	 ;  min_load.9, min_load
	ldd r22,Y+5	 ;  min_load.10, min_load
	ldd r23,Y+6	 ;  min_load.10, min_load
	ldi r24,lo8(50)	 ; ,
	ldi r25,0	 ; 
	rcall init_pwm_motor_control	 ; 
	.stabn	68,0,74,.LM35-.LFBB4
.LM35:
	ldi r24,0	 ;  tmp89
	ldi r25,lo8(-128)	 ; ,
	std Y+2,r25	 ;  speed.t16, tmp89
	std Y+1,r24	 ;  speed.t16, tmp89
	.stabn	68,0,75,.LM36-.LFBB4
.LM36:
	sts temp+1,r25	 ;  temp, tmp89
	sts temp,r24	 ;  temp, tmp89
	.stabn	68,0,77,.LM37-.LFBB4
.LM37:
/* #APP */
 ;  77 "motor.c" 1
	sei
 ;  0 "" 2
/* #NOAPP */
.L14:
.LBB5:
	.stabn	68,0,80,.LM38-.LFBB4
.LM38:
	movw r24,r28	 ; ,
	adiw r24,1	 ; ,
	rcall twi_slave_receive_byte	 ; 
	cpi r24,lo8(1)	 ; ,
	brne .L12	 ; ,
	.stabn	68,0,81,.LM39-.LFBB4
.LM39:
	sbis 0x12,7	 ; ,
	rjmp .L13	 ; 
	.stabn	68,0,81,.LM40-.LFBB4
.LM40:
	cbi 0x12,7	 ; ,
	rjmp .L12	 ; 
.L13:
	.stabn	68,0,81,.LM41-.LFBB4
.LM41:
	sbi 0x12,7	 ; ,
.L12:
	.stabn	68,0,84,.LM42-.LFBB4
.LM42:
	in r24,__SREG__	 ;  sreg.11, MEM[(volatile uint8_t *)95B]
	sts sreg,r24	 ;  sreg, sreg.11
	.stabn	68,0,85,.LM43-.LFBB4
.LM43:
/* #APP */
 ;  85 "motor.c" 1
	cli
 ;  0 "" 2
	.stabn	68,0,86,.LM44-.LFBB4
.LM44:
/* #NOAPP */
	rcall get_ADC	 ; 
	sts temp+1,r25	 ;  temp, temp.12
	sts temp,r24	 ;  temp, temp.12
	.stabn	68,0,87,.LM45-.LFBB4
.LM45:
	ldd r24,Y+1	 ; , speed
	ldd r25,Y+2	 ; , speed
	rcall set_pwm_channel_A	 ; 
	.stabn	68,0,88,.LM46-.LFBB4
.LM46:
	lds r24,sreg	 ;  sreg.13, sreg
	out __SREG__,r24	 ;  MEM[(volatile uint8_t *)95B], sreg.13
.LBB6:
.LBB7:
	.stabs	"/usr/lib/avr/include/util/delay.h",132,0,0,.Ltext1
.Ltext1:
	.stabn	68,0,164,.LM47-.LFBB4
.LM47:
	ldi r24,lo8(1843)	 ; ,
	ldi r25,hi8(1843)	 ; ,
	1: sbiw r24,1	 ; 
	brne 1b
	rjmp .L14	 ; 
.LBE7:
.LBE6:
.LBE5:
	.size	main, .-main
	.stabs	"min_load:(0,48)=B(5,4)",128,0,66,5
	.stabs	"max_load:(0,48)",128,0,67,3
	.stabs	"speed:(10,1)",128,0,73,1
	.stabn	192,0,0,.LFBB4-.LFBB4
	.stabn	224,0,0,.Lscope4-.LFBB4
.Lscope4:
	.stabs	"",36,0,0,.Lscope4-.LFBB4
	.stabd	78,0,0
	.local	scale
	.comm	scale,2,1
	.local	temp
	.comm	temp,2,1
	.local	sreg
	.comm	sreg,1,1
	.comm	trans,1,1
.global	MAX_SAVE
	.section	.eeprom,"aw",@progbits
	.type	MAX_SAVE, @object
	.size	MAX_SAVE, 2
MAX_SAVE:
	.word	-1
.global	MIN_SAVE
	.type	MIN_SAVE, @object
	.size	MIN_SAVE, 2
MIN_SAVE:
	.zero	2
	.stabs	"sreg:S(0,49)=B(5,2)",40,0,21,sreg
	.stabs	"temp:S(0,48)",40,0,22,temp
	.stabs	"scale:S(0,48)",40,0,22,scale
	.stabs	"MIN_SAVE:G(5,4)",32,0,17,0
	.stabs	"MAX_SAVE:G(5,4)",32,0,18,0
	.stabs	"trans:G(5,2)",32,0,20,0
	.text
	.stabs	"",100,0,0,.Letext0
.Letext0:
	.ident	"GCC: (GNU) 4.8.2"
.global __do_clear_bss
