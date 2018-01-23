OPT=
CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS= -mmcu=atmega64 $(OPT)
# -g -O0
#CFLAGS=-g -mmcu=atmega8515
OBJCOPY=avr-objcopy

#Info to AVRDUDE
ASFDIR=/home/radszuweit/AVR/xdk-asf-3.20.1
PROGRAMMER=avrdude
BOARD=stk500
MCU=m64
#MCU=m8515
PORT=/dev/ttyUSB0
BRATE=10 
# BRATE=115200 works as well
PFLAGS = -p $(MCU) -c $(BOARD) -P $(PORT) -B $(BRATE)

IFLAGS = \
	-I../uart \
	-I./ 
#	-I$(ASFDIR)/mega/utils \
#	-I$(ASFDIR)/mega/utils/preprocessor \
#	-I$(ASFDIR)/mega/drivers/spi \
#	-I$(ASFDIR)/common/utils/membag \
#	-I$(ASFDIR)/common/utils \
#	-I$(ASFDIR)/common/services/ioport \
#	-I$(ASFDIR)/common/services/spi \
#	-I$(ASFDIR)/common/services/sensors \
#	-I$(ASFDIR)/common/services/sensors/drivers/bosch

CFILES = qcopter.c ../timer/timer64.c ../usart/uart64.c ../BMA220/BMA220.c  ../twi/twi.c ../math/math.c ../ADC/adc.c
#../twi/twi.c ../usart/usart.c 
#bma220.c bma_axis.inc.c
OFILES = $(CFILES:%.c=%.o)
	

rom.hex : qcopter.out
	$(OBJCOPY) -j .text -O ihex qcopter.out rom.hex
	$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 -O ihex qcopter.out rom.eep

qcopter.out : $(OFILES)
	$(CC) $(CFLAGS) -o qcopter.out -Wl,-Map,qcopter.map $(OFILES) -lm
	@echo $(OFILES)

%.o : %.c
	$(CC) $(CFLAGS) -Os -c $< -o $@ $(IFLAGS) -lm

clean:
	rm -f *.o *.out *.map *.hex

program:
	$(PROGRAMMER) $(PFLAGS) -e	
	$(PROGRAMMER) $(PFLAGS) -U flash:w:rom.hex

eeprom: 
	$(PROGRAMMER) $(PFLAGS) -U eeprom:w:rom.eep
