#Контроллер, установленный на плате. Может быть другим, например atmega328 
DEVICE	=	atmega328p
#Тактовая частота 16 МГц 
CLOCK	=	16000000
PORT	=	COM5
AVRHOME	= 	"C:\Program Files\Arduino\hardware\tools\avr"
AVRDUDE	=	$(AVRHOME)/bin/avrdude -C$(AVRHOME)/etc/avrdude.conf -v -carduino -P$(PORT) -b115200 -D -p$(DEVICE)
OBJECTS	=	main.o
COMPILE	=	avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

all:	main.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

flash:	all
	$(AVRDUDE) -U flash:w:main.hex:i

clean:
	rm -f main.hex main.elf $(OBJECTS)

main.elf: $(OBJECTS)
	$(COMPILE) -o main.elf $(OBJECTS)

main.hex: main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size --format=avr --mcu=$(DEVICE) main.elf