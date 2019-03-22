CFLAGS := -fno-stack-protector -fno-builtin -march=armv4t -c
LDFLAGS := 

relocate_01.bin:../init/init.S ../sdram/sdram.c relocate_01.c ../uart/uart.c ../delay/delay.c data_relocate.S
	arm-linux-gnueabi-gcc $(CFLAGS) -o ../init/init.o ../init/init.S
	arm-linux-gnueabi-gcc $(CFLAGS) -o relocate_01.o relocate_01.c
	arm-linux-gnueabi-gcc $(CFLAGS) -o ../sdram/sdram.o ../sdram/sdram.c
	arm-linux-gnueabi-gcc $(CFLAGS) -o uart.o ../uart/uart.c
	arm-linux-gnueabi-gcc $(CFLAGS) -o delay.o ../delay/delay.c
	arm-linux-gnueabi-gcc $(CFLAGS) -o data_relocate.o data_relocate.S
#	arm-linux-gnueabi-ld -Ttext 0x00000000 \
#						 -Tdata 0x900 \
#						 $(LDFLAGS) ../init/init.o ../sdram/sdram.o delay.o relocate_01.o uart.o -o relocate_01.elf
	
	
	arm-linux-gnueabi-ld $(LDFLAGS) -T relocate_bss.lds \
		../init/init.o ../sdram/sdram.o delay.o relocate_01.o uart.o data_relocate.o -o relocate_01.elf
	
	arm-linux-gnueabi-objcopy -O binary -S relocate_01.elf relocate_01.bin
	arm-linux-gnueabi-objdump -D -m arm relocate_01.elf > relocate_01.dis

clean:
	rm -f *.bin *.elf *.o
