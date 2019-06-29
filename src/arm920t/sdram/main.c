#include "../include/s3c2440.h"
#include "sdram.h"
#include "led.h"

int main(void)
{
	sdram_init();
	bool status = sdram_test();
	
	while (1) {
		if (status == 0)
			//该函数是死循环，不会再跳转回来了
			led_test();
	}
	
	return 0;
}



