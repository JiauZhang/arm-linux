#include "../led/led.h"

extern void clock_init();
extern void watchdog_init();

void delay(unsigned int times);

int main()
{
	watchdog_init();
	clock_init();

	(*(volatile int *)GPBCON) = LED_OUT;
	(*(volatile int *)GPBDAT) |= ~LEDS_ON;

	//unsigned int count = 0;
	unsigned int led1_count = 0;
	unsigned int led2_count = 0;
	unsigned int led3_count = 0;
	unsigned int led4_count = 0;

	while(1) {
		if (led1_count == 0) {
			(*(volatile int *)GPBDAT) &= nLED_1_ON;
		}
		if (led1_count == 2000) {
			(*(volatile int *)GPBDAT) |= ~nLED_1_ON;
		}
		if (led1_count == 4000) {
			led1_count = 0;
		}

		if (led2_count == 0) {
			(*(volatile int *)GPBDAT) |= ~nLED_2_ON;
		}
		if (led2_count == 20000) {
			(*(volatile int *)GPBDAT) &= nLED_2_ON;
		}
		if (led2_count == 40000) {
			led2_count = 0;
		}

		if (led3_count == 0) {
			(*(volatile int *)GPBDAT) |= ~nLED_3_ON;
		}
		if (led3_count == 200000) {
			(*(volatile int *)GPBDAT) &= nLED_3_ON;
		}
		if (led3_count == 400000) {
			led3_count = 0;
		}

		if (led4_count == 0) {
			(*(volatile int *)GPBDAT) |= ~nLED_4_ON;
			//delay(50);
		}
		if (led4_count == 20000000) {
			(*(volatile int *)GPBDAT) &= nLED_4_ON;
			//delay(100);
		}
		 if (led4_count == 40000000) {
                        led4_count = 0;
                }


		led1_count++;
		led2_count++;
		led3_count++;
		led4_count++;
		/*
		if (count == 2000) {
			(*(volatile int *)GPBDAT) &= nLED_2_ON;
			(*(volatile int *)GPBDAT) |= ~nLED_1_ON;
		}

		if (count == 2000) {
			(*(volatile int *)GPBDAT) &= nLED_3_ON;
			(*(volatile int *)GPBDAT) |= ~nLED_2_ON;
		}

		if (count == 2000) {
			(*(volatile int *)GPBDAT) &= nLED_4_ON;
			(*(volatile int *)GPBDAT) |= ~nLED_3_ON;
		}

		count++;

		if (count > 2000) {
			count = 0;
		}
		*/
		/*
		count++;

		if (count == 1000000000)
			count = 1;

		(*(volatile int *)GPBDAT) &= LEDS_ON;
		delay(count);
		(*(volatile int *)GPBDAT) |= ~LEDS_ON;
		delay(count);
		*/

	}
}

void delay(unsigned int times)
{
	unsigned int multi = 4000;
	int i = 0;
	for (; i<times; i++) {
		while (multi) 
			multi--;
	}
}
