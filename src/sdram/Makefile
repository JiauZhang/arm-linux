CFLAGS := -fno-stack-protector -march=armv4t -c

sdram.bin:../init/init.S sdram.c main.c led.c
	arm-linux-gnueabi-gcc $(CFLAGS) -o ../init/init.o ../init/init.S
	arm-linux-gnueabi-gcc $(CFLAGS) -o sdram.o sdram.c
	arm-linux-gnueabi-gcc $(CFLAGS) -o main.o main.c
	arm-linux-gnueabi-gcc $(CFLAGS) -o led.o led.c
	arm-linux-gnueabi-ld -Ttext 0x00000000 -g ../init/init.o sdram.o main.o led.o -o sdram.elf
	arm-linux-gnueabi-objcopy -O binary -S sdram.elf sdram.bin
	arm-linux-gnueabi-objdump -D -m arm sdram.elf > sdram.dis

clean:
	rm -f *.bin *.elf *.o
