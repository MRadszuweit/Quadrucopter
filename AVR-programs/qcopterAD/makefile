CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS= -mmcu=atmega8 -O0
# -g -O0
#CFLAGS=-g -mmcu=atmega8515
OBJCOPY=avr-objcopy

#Info to AVRDUDE
PROGRAMMER=avrdude
BOARD=stk500
MCU=m8
#MCU=m8515
PORT=/dev/ttyUSB1
BRATE=10 
# BRATE=115200 works as well
PFLAGS = -p $(MCU) -c $(BOARD) -P $(PORT) -B $(BRATE)

CFILES = ADC.c ../twi/twi.c ../ADC/adc.c ../timer/timer8.c ../math/math.c
#../twi/twi.c ../usart/usart.c 
#bma220.c bma_axis.inc.c
OFILES = $(CFILES:%.c=%.o)


rom.hex : ADC.out
	$(OBJCOPY) -j .text -O ihex ADC.out rom.hex

ADC.out : $(OFILES)
	$(CC) $(CFLAGS) -o ADC.out -Wl,-Map,ADC.map $(OFILES) -lm
	@echo $(OFILES)

%.o : %.c
	$(CC) $(CFLAGS) -Os -c $< -o $@ $(IFLAGS) -lm

clean:
	rm -f *.o *.out *.map *.hex

program:
	$(PROGRAMMER) $(PFLAGS) -e
	$(PROGRAMMER) $(PFLAGS) -U flash:w:rom.hex