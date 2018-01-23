	.file	"twi.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
 ;  GNU C (GCC) version 4.8.2 (avr)
 ; 	compiled by GNU C version 4.8.2, GMP version 5.1.3, MPFR version 3.1.2-p3, MPC version 1.0.1
 ;  GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
 ;  options passed:  -fpreprocessed twi.i -mmcu=atmega8
 ;  -auxbase-strip ../twi/twi.o -g -Os -fverbose-asm
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
	.stabs	"../twi/twi.c",100,0,2,.Ltext0
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
	.stabs	"../twi/twi.h",130,0,0,0
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
	.stabs	"u16:T(1,1)=u2t16:(6,4),0,16;t8:(1,2)=ar(1,3)=r(1,3);0;0177777;;0;1;(6,2),0,16;;",128,0,0,0
	.stabn	162,0,0,0
	.stabs	"twi_init_master:F(0,47)",36,0,5,twi_init_master
	.stabs	"bitrate:P(6,2)",64,0,5,24
	.stabs	"prescaler:P(6,2)",64,0,5,22
.global	twi_init_master
	.type	twi_init_master, @function
twi_init_master:
	.stabd	46,0,0
	.stabn	68,0,5,.LM0-.LFBB1
.LM0:
.LFBB1:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	.stabn	68,0,6,.LM1-.LFBB1
.LM1:
	out 0,r24	 ;  MEM[(volatile uint8_t *)32B], bitrate
	.stabn	68,0,7,.LM2-.LFBB1
.LM2:
	in r24,0x1	 ;  D.1612, MEM[(volatile uint8_t *)33B]
	andi r24,lo8(-8)	 ;  D.1612,
	out 0x1,r24	 ;  MEM[(volatile uint8_t *)33B], D.1612
	.stabn	68,0,8,.LM3-.LFBB1
.LM3:
	in r24,0x1	 ;  D.1612, MEM[(volatile uint8_t *)33B]
	add r22,r24	 ;  D.1612, D.1612
	out 0x1,r22	 ;  MEM[(volatile uint8_t *)33B], D.1612
	ret
	.size	twi_init_master, .-twi_init_master
.Lscope1:
	.stabs	"",36,0,0,.Lscope1-.LFBB1
	.stabd	78,0,0
	.stabs	"twi_init_slave:F(0,47)",36,0,12,twi_init_slave
	.stabs	"ID:P(6,2)",64,0,12,24
.global	twi_init_slave
	.type	twi_init_slave, @function
twi_init_slave:
	.stabd	46,0,0
	.stabn	68,0,12,.LM4-.LFBB2
.LM4:
.LFBB2:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	.stabn	68,0,13,.LM5-.LFBB2
.LM5:
	lsl r24	 ;  D.1615
	out 0x2,r24	 ;  MEM[(volatile uint8_t *)34B], D.1615
	ret
	.size	twi_init_slave, .-twi_init_slave
.Lscope2:
	.stabs	"",36,0,0,.Lscope2-.LFBB2
	.stabd	78,0,0
	.stabs	"twi_start:F(0,47)",36,0,16,twi_start
.global	twi_start
	.type	twi_start, @function
twi_start:
	.stabd	46,0,0
	.stabn	68,0,16,.LM6-.LFBB3
.LM6:
.LFBB3:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	.stabn	68,0,17,.LM7-.LFBB3
.LM7:
	ldi r24,lo8(-92)	 ;  tmp43,
	out 0x36,r24	 ;  MEM[(volatile uint8_t *)86B], tmp43
	ret
	.size	twi_start, .-twi_start
.Lscope3:
	.stabs	"",36,0,0,.Lscope3-.LFBB3
	.stabd	78,0,0
	.stabs	"twi_stop:F(0,47)",36,0,20,twi_stop
.global	twi_stop
	.type	twi_stop, @function
twi_stop:
	.stabd	46,0,0
	.stabn	68,0,20,.LM8-.LFBB4
.LM8:
.LFBB4:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	.stabn	68,0,21,.LM9-.LFBB4
.LM9:
	ldi r24,lo8(-108)	 ;  tmp43,
	out 0x36,r24	 ;  MEM[(volatile uint8_t *)86B], tmp43
	ret
	.size	twi_stop, .-twi_stop
.Lscope4:
	.stabs	"",36,0,0,.Lscope4-.LFBB4
	.stabd	78,0,0
	.stabs	"twi_wait_for_TWINT:F(0,47)",36,0,24,twi_wait_for_TWINT
.global	twi_wait_for_TWINT
	.type	twi_wait_for_TWINT, @function
twi_wait_for_TWINT:
	.stabd	46,0,0
	.stabn	68,0,24,.LM10-.LFBB5
.LM10:
.LFBB5:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
.L7:
	.stabn	68,0,25,.LM11-.LFBB5
.LM11:
	in __tmp_reg__,0x36	 ; 
	sbrs __tmp_reg__,7	 ; 
	rjmp .L7	 ; 
/* epilogue start */
	.stabn	68,0,26,.LM12-.LFBB5
.LM12:
	ret
	.size	twi_wait_for_TWINT, .-twi_wait_for_TWINT
.Lscope5:
	.stabs	"",36,0,0,.Lscope5-.LFBB5
	.stabd	78,0,0
	.stabs	"twi_check:F(6,2)",36,0,28,twi_check
	.stabs	"status_code:P(6,2)",64,0,28,24
.global	twi_check
	.type	twi_check, @function
twi_check:
	.stabd	46,0,0
	.stabn	68,0,28,.LM13-.LFBB6
.LM13:
.LFBB6:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	.stabn	68,0,29,.LM14-.LFBB6
.LM14:
	in r18,0x1	 ;  D.1627, MEM[(volatile uint8_t *)33B]
	andi r18,lo8(-8)	 ;  D.1627,
	ldi r19,0	 ;  D.1628
	ldi r25,0	 ;  D.1628
	cp r18,r24	 ;  D.1628, D.1628
	cpc r19,r25	 ;  D.1628, D.1628
	breq .L11	 ; ,
.LBB8:
.LBB9:
	.stabn	68,0,21,.LM15-.LFBB6
.LM15:
	ldi r24,lo8(-108)	 ;  tmp55,
	out 0x36,r24	 ;  MEM[(volatile uint8_t *)86B], tmp55
.LBE9:
.LBE8:
	.stabn	68,0,31,.LM16-.LFBB6
.LM16:
	in r24,0x1	 ;  twi_error_code.0, MEM[(volatile uint8_t *)33B]
	sts twi_error_code,r24	 ;  twi_error_code, twi_error_code.0
	.stabn	68,0,32,.LM17-.LFBB6
.LM17:
	ldi r24,0	 ;  D.1626
	ret
.L11:
	.stabn	68,0,34,.LM18-.LFBB6
.LM18:
	ldi r24,lo8(1)	 ;  D.1626,
	.stabn	68,0,35,.LM19-.LFBB6
.LM19:
	ret
	.size	twi_check, .-twi_check
.Lscope6:
	.stabs	"",36,0,0,.Lscope6-.LFBB6
	.stabd	78,0,0
	.stabs	"twi_master_transmit_byte:F(6,2)",36,0,37,twi_master_transmit_byte
	.stabs	"addr:P(6,2)",64,0,37,17
	.stabs	"Bytes:P(0,48)=*(6,2)",64,0,37,28
	.stabs	"size:P(6,2)",64,0,37,16
.global	twi_master_transmit_byte
	.type	twi_master_transmit_byte, @function
twi_master_transmit_byte:
	.stabd	46,0,0
	.stabn	68,0,37,.LM20-.LFBB7
.LM20:
.LFBB7:
	push r13	 ; 
	push r14	 ; 
	push r15	 ; 
	push r16	 ; 
	push r17	 ; 
	push r28	 ; 
	push r29	 ; 
/* prologue: function */
/* frame size = 0 */
/* stack size = 7 */
.L__stack_usage = 7
	mov r17,r24	 ;  addr, addr
	movw r28,r22	 ;  Bytes, Bytes
	mov r16,r20	 ;  size, size
	.stabn	68,0,38,.LM21-.LFBB7
.LM21:
	in r15,__SREG__	 ;  sreg, MEM[(volatile uint8_t *)95B]
	.stabn	68,0,39,.LM22-.LFBB7
.LM22:
/* #APP */
 ;  39 "../twi/twi.c" 1
	cli
 ;  0 "" 2
/* #NOAPP */
.LBB10:
.LBB11:
	.stabn	68,0,17,.LM23-.LFBB7
.LM23:
	ldi r24,lo8(-92)	 ;  tmp68,
	out 0x36,r24	 ;  MEM[(volatile uint8_t *)86B], tmp68
.LBE11:
.LBE10:
	.stabn	68,0,43,.LM24-.LFBB7
.LM24:
	rcall twi_wait_for_TWINT	 ; 
	.stabn	68,0,46,.LM25-.LFBB7
.LM25:
	ldi r24,lo8(8)	 ; ,
	rcall twi_check	 ; 
	tst r24	 ; 
	breq .L22	 ; ,
	.stabn	68,0,52,.LM26-.LFBB7
.LM26:
	lsl r17	 ;  addr
	.stabn	68,0,53,.LM27-.LFBB7
.LM27:
	out 0x3,r17	 ;  MEM[(volatile uint8_t *)35B], addr
	.stabn	68,0,54,.LM28-.LFBB7
.LM28:
	ldi r24,lo8(-124)	 ;  tmp72,
	out 0x36,r24	 ;  MEM[(volatile uint8_t *)86B], tmp72
	.stabn	68,0,55,.LM29-.LFBB7
.LM29:
	rcall twi_wait_for_TWINT	 ; 
	.stabn	68,0,58,.LM30-.LFBB7
.LM30:
	ldi r24,lo8(24)	 ; ,
	rcall twi_check	 ; 
	tst r24	 ; 
	breq .L22	 ; ,
	.stabn	68,0,63,.LM31-.LFBB7
.LM31:
	sbis 0x12,7	 ; ,
	rjmp .L16	 ; 
	.stabn	68,0,63,.LM32-.LFBB7
.LM32:
	cbi 0x12,7	 ; ,
	rjmp .L17	 ; 
.L16:
	.stabn	68,0,63,.LM33-.LFBB7
.LM33:
	sbi 0x12,7	 ; ,
.L17:
	.stabn	68,0,37,.LM34-.LFBB7
.LM34:
	mov r14,__zero_reg__	 ;  i
	.stabn	68,0,66,.LM35-.LFBB7
.LM35:
	ldi r17,0	 ;  D.1641
	.stabn	68,0,69,.LM36-.LFBB7
.LM36:
	ldi r24,lo8(-124)	 ; ,
	mov r13,r24	 ;  tmp92,
	rjmp .L18	 ; 
.L22:
	.stabn	68,0,74,.LM37-.LFBB7
.LM37:
	out __SREG__,r15	 ; , sreg
	.stabn	68,0,75,.LM38-.LFBB7
.LM38:
	rjmp .L14	 ; 
.L20:
	.stabn	68,0,68,.LM39-.LFBB7
.LM39:
	movw r30,r28	 ;  tmp80, Bytes
	add r30,r24	 ;  tmp80, D.1641
	adc r31,r25	 ; , D.1641
	ld r24,Z	 ;  D.1639, *_35
	out 0x3,r24	 ;  MEM[(volatile uint8_t *)35B], D.1639
	.stabn	68,0,69,.LM40-.LFBB7
.LM40:
	out 0x36,r13	 ;  MEM[(volatile uint8_t *)86B], tmp92
	.stabn	68,0,70,.LM41-.LFBB7
.LM41:
	rcall twi_wait_for_TWINT	 ; 
	.stabn	68,0,73,.LM42-.LFBB7
.LM42:
	ldi r24,lo8(40)	 ; ,
	rcall twi_check	 ; 
	tst r24	 ; 
	breq .L22	 ; ,
	inc r14	 ;  i
.L18:
	.stabn	68,0,66,.LM43-.LFBB7
.LM43:
	mov r24,r14	 ;  D.1641, i
	clr r25	 ;  D.1641
	sbrc r24,7	 ;  D.1641
	com r25	 ;  D.1641
	cp r24,r16	 ;  D.1641, D.1641
	cpc r25,r17	 ;  D.1641, D.1641
	brlt .L20	 ; ,
.LBB12:
.LBB13:
	.stabn	68,0,21,.LM44-.LFBB7
.LM44:
	ldi r24,lo8(-108)	 ;  tmp88,
	out 0x36,r24	 ;  MEM[(volatile uint8_t *)86B], tmp88
.LBE13:
.LBE12:
	.stabn	68,0,82,.LM45-.LFBB7
.LM45:
	out __SREG__,r15	 ;  MEM[(volatile uint8_t *)95B], sreg
	.stabn	68,0,83,.LM46-.LFBB7
.LM46:
	ldi r24,lo8(1)	 ;  D.1638,
.L14:
/* epilogue start */
	.stabn	68,0,84,.LM47-.LFBB7
.LM47:
	pop r29	 ; 
	pop r28	 ; 
	pop r17	 ; 
	pop r16	 ; 
	pop r15	 ; 
	pop r14	 ; 
	pop r13	 ; 
	ret
	.size	twi_master_transmit_byte, .-twi_master_transmit_byte
	.stabs	"sreg:r(6,2)",64,0,38,15
	.stabs	"i:r(6,1)",64,0,65,14
	.stabn	192,0,0,.LFBB7-.LFBB7
	.stabn	224,0,0,.Lscope7-.LFBB7
.Lscope7:
	.stabs	"",36,0,0,.Lscope7-.LFBB7
	.stabd	78,0,0
	.stabs	"twi_slave_receive_byte:F(6,2)",36,0,86,twi_slave_receive_byte
	.stabs	"byte:P(0,48)",64,0,86,28
.global	twi_slave_receive_byte
	.type	twi_slave_receive_byte, @function
twi_slave_receive_byte:
	.stabd	46,0,0
	.stabn	68,0,86,.LM48-.LFBB8
.LM48:
.LFBB8:
	push r15	 ; 
	push r16	 ; 
	push r17	 ; 
	push r28	 ; 
	push r29	 ; 
/* prologue: function */
/* frame size = 0 */
/* stack size = 5 */
.L__stack_usage = 5
	movw r28,r24	 ;  byte, byte
	.stabn	68,0,88,.LM49-.LFBB8
.LM49:
	in r17,__SREG__	 ;  sreg, MEM[(volatile uint8_t *)95B]
	.stabn	68,0,90,.LM50-.LFBB8
.LM50:
/* #APP */
 ;  90 "../twi/twi.c" 1
	cli
 ;  0 "" 2
	.stabn	68,0,93,.LM51-.LFBB8
.LM51:
/* #NOAPP */
	ldi r24,lo8(-60)	 ;  tmp55,
	out 0x36,r24	 ;  MEM[(volatile uint8_t *)86B], tmp55
	.stabn	68,0,94,.LM52-.LFBB8
.LM52:
	rcall twi_wait_for_TWINT	 ; 
	.stabn	68,0,97,.LM53-.LFBB8
.LM53:
	ldi r24,lo8(96)	 ; ,
	rcall twi_check	 ; 
	cpse r24,__zero_reg__	 ; ,
	rjmp .L24	 ; 
	.stabn	68,0,98,.LM54-.LFBB8
.LM54:
	out __SREG__,r17	 ;  MEM[(volatile uint8_t *)95B], sreg
	.stabn	68,0,99,.LM55-.LFBB8
.LM55:
	rjmp .L25	 ; 
.L24:
	.stabn	68,0,97,.LM56-.LFBB8
.LM56:
	ldi r16,0	 ;  i
	.stabn	68,0,105,.LM57-.LFBB8
.LM57:
	ldi r25,lo8(-60)	 ; ,
	mov r15,r25	 ;  tmp66,
.L27:
	out 0x36,r15	 ;  MEM[(volatile uint8_t *)86B], tmp66
	.stabn	68,0,106,.LM58-.LFBB8
.LM58:
	rcall twi_wait_for_TWINT	 ; 
	.stabn	68,0,109,.LM59-.LFBB8
.LM59:
	in r24,0x1	 ;  status, MEM[(volatile uint8_t *)33B]
	andi r24,lo8(-8)	 ;  status,
	.stabn	68,0,110,.LM60-.LFBB8
.LM60:
	cpi r24,lo8(-128)	 ;  status,
	brne .L26	 ; ,
	.stabn	68,0,111,.LM61-.LFBB8
.LM61:
	in r24,0x3	 ;  D.1654, MEM[(volatile uint8_t *)35B]
	movw r30,r28	 ;  tmp62, byte
	add r30,r16	 ;  tmp62, i
	adc r31,__zero_reg__	 ;  tmp62
	st Z,r24	 ;  *_21, D.1654
	.stabn	68,0,112,.LM62-.LFBB8
.LM62:
	subi r16,lo8(-(1))	 ;  i,
	rjmp .L27	 ; 
.L26:
	.stabn	68,0,115,.LM63-.LFBB8
.LM63:
	out __SREG__,r17	 ; , sreg
	.stabn	68,0,114,.LM64-.LFBB8
.LM64:
	cpi r24,lo8(-96)	 ;  status,
	breq .L28	 ; ,
	.stabn	68,0,116,.LM65-.LFBB8
.LM65:
	ldi r24,0	 ;  D.1653
	rjmp .L25	 ; 
.L28:
	.stabn	68,0,121,.LM66-.LFBB8
.LM66:
	ldi r24,lo8(1)	 ;  D.1653,
.L25:
/* epilogue start */
	.stabn	68,0,122,.LM67-.LFBB8
.LM67:
	pop r29	 ; 
	pop r28	 ; 
	pop r17	 ; 
	pop r16	 ; 
	pop r15	 ; 
	ret
	.size	twi_slave_receive_byte, .-twi_slave_receive_byte
	.stabs	"status:r(6,2)",64,0,87,24
	.stabs	"sreg:r(6,2)",64,0,88,17
	.stabs	"i:r(6,2)",64,0,102,16
	.stabn	192,0,0,.LFBB8-.LFBB8
	.stabn	224,0,0,.Lscope8-.LFBB8
.Lscope8:
	.stabs	"",36,0,0,.Lscope8-.LFBB8
	.stabd	78,0,0
	.local	twi_error_code
	.comm	twi_error_code,1,1
	.stabs	"twi_error_code:S(0,49)=B(6,2)",40,0,3,twi_error_code
	.stabs	"",100,0,0,.Letext0
.Letext0:
	.ident	"GCC: (GNU) 4.8.2"
.global __do_clear_bss
