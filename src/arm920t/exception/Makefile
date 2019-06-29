CFLAGS := -fno-stack-protector -fno-builtin -march=armv4t -c
LDFLAGS := 

exception.bin: ../sdram/sdram.c relocate.c ../uart/uart.c ../delay/delay.c main.c start.S interrupt.c
	arm-linux-gnueabi-gcc $(CFLAGS) -o relocate.o relocate.c
	arm-linux-gnueabi-gcc $(CFLAGS) -o ../sdram/sdram.o ../sdram/sdram.c
	arm-linux-gnueabi-gcc $(CFLAGS) -o ../uart/uart.o ../uart/uart.c
	arm-linux-gnueabi-gcc $(CFLAGS) -o ../delay/delay.o ../delay/delay.c
	arm-linux-gnueabi-gcc $(CFLAGS) -o main.o main.c
	arm-linux-gnueabi-gcc $(CFLAGS) -o interrupt.o interrupt.c
	arm-linux-gnueabi-gcc $(CFLAGS) -o start.o start.S	
	
	arm-linux-gnueabi-ld $(LDFLAGS) -T exception.lds \
		start.o ../sdram/sdram.o ../delay/delay.o relocate.o main.o interrupt.o ../uart/uart.o \
		-o exception.elf
	
	arm-linux-gnueabi-objcopy -O binary -S exception.elf exception.bin
	arm-linux-gnueabi-objdump -D -m arm exception.elf > exception.dis

clean:
	rm *.bin *.o *.elf *.dis