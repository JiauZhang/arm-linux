.global watchdog_init

#define WTCON 0x53000000
#define DISABLE 0x00

watchdog_init:
	LDR R0, =WTCON
	MOV R1, #DISABLE
	STR R1, [R0]

	MOV PC, LR
