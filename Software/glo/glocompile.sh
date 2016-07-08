#!/bin/bash

avr-g++ -mmcu=atmega168 -O1 main.cpp Adafruit_NeoPixel.cpp glow_manager.cpp mc25aa.c button.cpp
avr-objcopy -R .eeprom -O ihex a.out main.hex
sudo avrdude -c usbtiny -p m168 -e
sudo avrdude -c usbtiny -p m168 -U flash:w:main.hex


#avrdude -c usbtiny -p m168 -P /dev/ttyUSB0 -e
#avrdude -c usbtiny -p m168  -P /dev/ttyUSB0 -U flash:w:main.hex



