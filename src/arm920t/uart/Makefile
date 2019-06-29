CFLAGS := -fno-stack-protector -march=armv4t

uart.bin:../init/init.S uart.c main.c
	arm-linux-gnueabi-gcc $(CFLAGS) -g -c -o init.o ../init/init.S
	arm-linux-gnueabi-gcc $(CFLAGS) -g -c -o uart.o uart.c
	arm-linux-gnueabi-gcc $(CFLAGS) -g -c -o main.o main.c
	arm-linux-gnueabi-ld -Ttext 0x00000000 -g init.o uart.o main.o -o uart.elf
	arm-linux-gnueabi-objcopy -O binary -S uart.elf uart.bin
	arm-linux-gnueabi-objdump -D -m arm uart.elf > uart.dis

clean:
	rm -f *.bin *.elf *.o
