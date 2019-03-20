led_on.bin:led.S
	arm-linux-gnueabi-gcc -march=armv4t -g -c -o led_on.o led.S
	arm-linux-gnueabi-ld -Ttext 0x00000000 -g led_on.o -o led_on_elf
	arm-linux-gnueabi-objcopy -O binary -S led_on_elf led_on.bin

clean:
	rm -f led_on.bin led_on_elf *.o

led_c:led.c
	arm-linux-gnueabi-gcc -march=armv4t -g -c -o led_on.o led.c
	arm-linux-gnueabi-ld -Ttext 0x00000000 -g led_on.o -o led_on_elf
	arm-linux-gnueabi-objcopy -O binary -S led_on_elf led_on_c.bin

