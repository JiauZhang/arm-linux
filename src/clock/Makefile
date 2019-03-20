clock.bin:clock.S twinkle.c watchdog.S
	arm-linux-gnueabi-gcc -march=armv4t -g -c -o clock.o clock.S
	arm-linux-gnueabi-gcc -march=armv4t -g -c -o twinkle.o twinkle.c
	arm-linux-gnueabi-gcc -march=armv4t -g -c -o watchdog.o watchdog.S
	arm-linux-gnueabi-ld -Ttext 0x00000000 -g twinkle.o clock.o watchdog.o -o clock.elf
	arm-linux-gnueabi-objcopy -O binary -S clock.elf clock.bin

clean:
	rm -f *.bin *.elf *.o

