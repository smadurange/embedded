compile:\
avr-gcc -c -std=gnu99 -Os -Wall -ffunction-sections -fdata-sections -mmcu=atmega328p -DF_CPU=16000000 blink.c

link:\
avr-gcc -Os -mmcu=atmega328p -ffunction-sections -fdata-sections -Wl,--gc-sections blink.o -o blink.elf

hex file:\
avr-objcopy -O ihex -R .eeprom blink.elf blink.ihex

upload:\
avrdude -p atmega328p -c arduino -P /dev/ttyACM0 -D -U flash:w:blink.ihex:i

disassemble:\
avr-objdump -d blink.elf
