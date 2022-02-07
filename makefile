projName = lab3
mcu = atmega16
isp = avrispv2
port = /dev/ttyACM0



build:
	clear
	avr-gcc -Os -mmcu=$(mcu) -I include -o $(projName).elf \
		-Wall -ffunction-sections -fdata-sections \
		-Wl,--gc-sections \
		lib/*.c main.c
	avr-objcopy -j .text -j .data \
		-O ihex $(projName).elf \
		$(projName).hex
	avr-size --format=avr --mcu=$(mcu) $(projName).elf

upload:
	avrdude -c $(isp) -P $(port) -p $(mcu) \
		-U flash:w:$(projName).hex

clean:
	rm *.o *.elf *.hex

build_mega32:
	clear
	avr-gcc -Os -mmcu=atmega32 -I include -o $(projName)_mega32.elf \
		-Wall -ffunction-sections -fdata-sections \
		-Wl,--gc-sections \
		lib/*.c main.c
	avr-objcopy -j .text -j .data \
		-O ihex $(projName)_mega32.elf \
		$(projName)_mega32.hex
	avr-size --format=avr --mcu=atmega32 $(projName)_mega32.elf
