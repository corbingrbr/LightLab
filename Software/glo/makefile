CC=/usr/bin/avr-gcc
MEGA=168
CFLAGS= -g -Os /*-std=gnu99*/ -Wall -mcall-prologues -mmcu=atmega$(MEGA) main.cpp Adafruit_NeoPixel.cpp glow_manager.cpp mc25aa.c

OBJ2HEX=/usr/bin/avr-objcopy 
PROG=/usr/bin/avrdude
TARGET= main

program : $(TARGET).hex
	$(PROG) -c avrispv2 -p m$(MEGA)  -P /dev/ttyUSB0 -e
	$(PROG) -c avrispv2 -p m$(MEGA)  -P /dev/ttyUSB0 -U flash:w:$(TARGET).hex

setfuse:
	avrdude $(AVRDUDE_FLAGS) -F -u -U lfuse:w:0xA4:m
	avrdude $(AVRDUDE_FLAGS) -F -u -U hfuse:w:0xD1:m

%.obj : %.o
	$(CC) $(CFLAGS) $< -o $@

%.hex : %.obj
	$(OBJ2HEX) -R .eeprom -O ihex $< $@

clean :
	rm -f *.hex *.obj *.o
