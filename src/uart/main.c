#include "uart.h"

int main(int argc, char **argv)
{
	uart0_init();
	unsigned int count = 0;
	while (1) {
		if (count == 0) {
			print1();
			print2();
			puts("uart0 output test\n");
			count++;
		} else if (count == 50000) {
			count = 0;
		} else {
			count++;
		}		
	}
	
	return 0;
}