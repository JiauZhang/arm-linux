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
		if (led1_count == 10000) {
			(*(volatile int *)GPBDAT) |= ~nLED_1_ON;
		}
		if (led1_count == 20000) {
			led1_count = 0;
		} else {
			led1_count++;
		}

		if (led2_count == 0) {
			(*(volatile int *)GPBDAT) |= ~nLED_2_ON;
		}
		if (led2_count == 50000) {
			(*(volatile int *)GPBDAT) &= nLED_2_ON;
		}
		if (led2_count == 100000) {
			led2_count = 0;
		} else {
			led2_count++;
		}

		if (led3_count == 0) {
			(*(volatile int *)GPBDAT) |= ~nLED_3_ON;
		}
		if (led3_count == 100000) {
			(*(volatile int *)GPBDAT) &= nLED_3_ON;
		}
		if (led3_count == 200000) {
			led3_count = 0;
		} else {
			led3_count++;
		}

		if (led4_count == 0) {
			(*(volatile int *)GPBDAT) |= ~nLED_4_ON;
			//delay(50);
		}
		if (led4_count == 200000) {
			(*(volatile int *)GPBDAT) &= nLED_4_ON;
			//delay(100);
		}
		if (led4_count == 400000) {
			led4_count = 0;
		} else {
			led4_count++;
		}
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
