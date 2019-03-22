#include "../include/s3c2440.h"
#include "../uart/uart.h"
#include "../sdram/sdram.h"
#include "../delay/delay.h"

char global_char1 = 'a';
const char global_char2 = 'b';
int global_int1 = 5;
int global_int2;

extern void data_relocate();
/*
	这些在lds中定义的变量，其实等价于对应地址上的变量值
	所以要想在C文件中获取地址值，就要使用 & 取地址操作
	而在汇编文件中，则代表的就是地址，而不是变量值
*/
extern unsigned int data_start;
extern unsigned int data_end;
extern unsigned int bss_start;
extern unsigned int bss_end;

int main(void)
{
	char local_char1 = 'c';
	const char local_char2 = 'd';
	
	 unsigned int *data_start_ = (unsigned int *)&data_start; 
	 unsigned int *data_end_ = (unsigned int *)&data_end; 
	 unsigned int *bss_start_ = (unsigned int *)bss_start;
	 unsigned int *bss_end_ = (unsigned int *)bss_end;
	
	uart0_init();
	sdram_init();
	
	data_relocate();
	
	puts("\r\ndata start:\n\r");
	putuint((unsigned int)data_start_);
	puts("\r\ndata end:\n\r");
	putuint((unsigned int)data_end_);
	puts("\r\nbss start:\n\r");
	putuint((unsigned int)bss_start_);
	puts("\r\nbss end:\n\r");
	putuint((unsigned int)bss_end_);
	
	
	while (1) {
		puts("\n\rglobal char:\n\r");
		putchar(global_char1);
		putchar(global_char2);
		puts("\n\rglobal int:\n\r");
		putint(global_int1);
		putint(global_int2);
		puts("\n\rlocal char:\n\r");
		putchar(local_char1);
		putchar(local_char2);
		delay(6);
		puts("\n\rglobal add\n\r");
		global_char1++;
		/* global_char2++; 只读变量不可进行修改 */
		global_int1++;
		global_int2++;
		puts("\n\rlocal add\n\r");
		local_char1++;
		/* local_char2++; 只读变量不可进行修改 */
		puts("\n\r=================\n\r");
	}
	
	return 0;
}



