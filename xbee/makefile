CFLAGS = -g -Wall
#CFLAGS = -Wall
LDFLAGS = -lm
IFILES = -I/home/radszuweit/AVR/Programme
CFILES = xbee.c /home/radszuweit/AVR/Programme/math/math.c
OFILES = $(CFILES:%.c=%.o)
ARCH = $(shell arch)
OLD_ARCH = $(shell cat makefile.log) 

look:
ifneq ($(strip $(ARCH)), $(strip $(OLD_ARCH)))
	@echo new architecture: $(OLD_ARCH) to $(ARCH)
	make clean
else 
	@echo $(ARCH) architecture
endif		
	make compile

compile: $(OFILES)

	gcc $(CFLAGS) -o xbee$(ARCH) $(OFILES) $(LDFLAGS) $(IFILES)

%.o : %.c
	gcc $(CFLAGS) -c -o $@ $< $(IFILES)

	@echo $(ARCH) > makefile.log
	

clean:
	rm -f *.o xbee
