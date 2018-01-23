CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS=-g -mmcu=atmega8
#CFLAGS=-g -mmcu=atmega8515
OBJCOPY=avr-objcopy

#Info to AVRDUDE
ASFDIR=/home/radszuweit/AVR/xdk-asf-3.20.1
PROGRAMMER=avrdude
BOARD=stk500
MCU=m8
#MCU=m8515
PORT=/dev/ttyUSB0
BRATE=60 
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

CFILES = BMA220.c ../usart/usart.c ../timer/timer.c ../twi/twi.c
#bma220.c bma_axis.inc.c
OFILES = $(CFILES:%.c=%.o)


rom.hex : BMA220.out
	$(OBJCOPY) -j .text -O ihex BMA220.out rom.hex

BMA220.out : $(OFILES)
	$(CC) $(CFLAGS) -o BMA220.out -Wl,-Map,BMA220.map $(OFILES) -lm
	@echo $(OFILES)

%.o : %.c
	$(CC) $(CFLAGS) -Os -c $< -o $@ $(IFLAGS) -lm

clean:
	rm -f *.o *.out *.map *.hex

program:
	$(PROGRAMMER) $(PFLAGS) -e
	$(PROGRAMMER) $(PFLAGS) -U flash:w:rom.hex