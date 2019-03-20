led.bin:led.S
	arm-linux-gnueabi-gcc -march=armv4t -g -c -o led.o led.S
	arm-linux-gnueabi-ld -Ttext 0x00000000 -g led.o -o led.elf
	arm-linux-gnueabi-objcopy -O binary -S led.elf led.bin

clean:
	rm -f *.bin *.elf *.o

