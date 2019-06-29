clock_405MHz.bin:clock.S twinkle.c watchdog.S stack.S
	arm-linux-gnueabi-gcc -march=armv4t -g -c -o clock.o clock.S
	arm-linux-gnueabi-gcc -march=armv4t -g -c -o twinkle.o twinkle.c
	arm-linux-gnueabi-gcc -march=armv4t -c -o stack.o stack.S
	arm-linux-gnueabi-gcc -march=armv4t -g -c -o watchdog.o watchdog.S
	arm-linux-gnueabi-ld -Ttext 0x00000000 -g stack.o twinkle.o clock.o watchdog.o -o clock.elf
	arm-linux-gnueabi-objcopy -O binary -S clock.elf clock_405MHz.bin
	arm-linux-gnueabi-objdump -D -m arm clock.elf > clock_405MHz.dis

clean:
	rm -f *.bin *.elf *.o
clock_12MHz.bin:clock_12MHz.c
	arm-linux-gnueabi-gcc -march=armv4t -c -o clock_12MHz.o clock_12MHz.c
	arm-linux-gnueabi-gcc -march=armv4t -c -o stack.o stack.S
	arm-linux-gnueabi-ld -Ttext 0x00000000 -g stack.o clock_12MHz.o -o clock_12MHz.elf
	arm-linux-gnueabi-objcopy -O binary -S clock_12MHz.elf clock_12MHz.bin
	arm-linux-gnueabi-objdump -D -m arm clock_12MHz.elf > clock_12MHz.dis

