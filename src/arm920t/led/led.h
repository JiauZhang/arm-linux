
#define GPBCON (*(volatile int *)0x56000010)
#define GPBDAT (*(volatile int *)0x56000014)

#define GPB5 (1<<(5*2))
#define GPB6 (1<<(6*2))
#define GPB7 (1<<(7*2))
#define GPB8 (1<<(8*2))

#define nLED_1 GPB5
#define nLED_2 GPB6
#define nLED_3 GPB7
#define nLED_4 GPB8

#define LED_OUT (nLED_1 | nLED_2 | nLED_3 | nLED_4)

#define GPB5_ON ~(1<<5)
#define GPB6_ON ~(1<<6)
#define GPB7_ON ~(1<<7)
#define GPB8_ON ~(1<<8)

#define nLED_1_ON GPB5_ON
#define nLED_2_ON GPB6_ON
#define nLED_3_ON GPB7_ON
#define nLED_4_ON GPB8_ON

#define LEDS_ON (nLED_1_ON & nLED_2_ON & nLED_3_ON & nLED_4_ON)


