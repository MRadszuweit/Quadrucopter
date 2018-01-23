CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS=-g -mmcu=atmega8
#CFLAGS=-g -mmcu=atmega8515
OBJCOPY=avr-objcopy

#Info to AVRDUDE
PROGRAMMER=avrdude
BOARD=stk500
MCU=m8
#MCU=m8515
PORT=/dev/ttyUSB0
BRATE=60 
PFLAGS = -p $(MCU) -c $(BOARD) -P $(PORT) -B $(BRATE)

CFILES = motor.c ../timer/timer.c ../twi/twi.c
OFILES = $(CFILES:%.c=%.o)

rom.hex : motor.out
	$(OBJCOPY) -j .text -O ihex motor.out rom.hex

motor.out : $(OFILES)
	$(CC) $(CFLAGS) -o motor.out -Wl,-Map,motor.map $(OFILES)

%.o : %.c
	$(CC) $(CFLAGS) -Os -c $< -o $@

clean:
	rm -f *.o *.out *.map *.hex

program:
	$(PROGRAMMER) $(PFLAGS) -e
	$(PROGRAMMER) $(PFLAGS) -U flash:w:rom.hex