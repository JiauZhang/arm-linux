#include "../include/s3c2440.h"
#include "../led/led.h"

void delay(volatile int d)
{
	while (d--);
}

int led_test(void)
{
	GPBCON = LED_OUT;
	GPBDAT |= ~LEDS_ON;

	while (1)
	{
		GPBDAT &= LEDS_ON;
		delay(100000);
		GPBDAT != ~LEDS_ON;
		delay(100000);
	}

	return 0;
}



