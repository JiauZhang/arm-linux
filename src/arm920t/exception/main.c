#include "../include/s3c2440.h"
#include "../uart/uart.h"
#include "../delay/delay.h"
#include "interrupt.h"

char g_Char = 'A';
char g_Char3 = 'a';
const char g_Char2 = 'B';
int g_A = 0;
int g_B;

int main(void)
{
	puts("\r\ninit interrupt\n\r");
	interrupt_init();
	
	puts("\n\rg_A = ");
	printHex(g_A);
	puts("\n\r");

	while (1)
	{
#if 0		
		puts("\n\rg_Char = ");
		printHex(g_Char);
		puts("\n\r");


		puts("\n\rg_Char3 = ");
		printHex(g_Char3);
		puts("\n\r");
#endif
		putchar(g_Char);
		g_Char++;

		putchar(g_Char3);
		g_Char3++;
		delay(10);
	}

	
	return 0;
}



