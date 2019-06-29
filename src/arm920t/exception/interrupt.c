#include "../include/s3c2440.h"
#include "../uart/uart.h"

#define CONFIG_GPG0_EINT8 2 
#define CONFIG_EINT8_EN ~(1<<8)
#define CONFIG_EINT8 (~(1<<5))
#define CONFIG_EINT8_MODE 7
//不知为何，无法触发中断！！
void interrupt_init()
{
	puts("\r\nentry interrupt init\r\n");

	//打开EINT8中断使能
	INTMSK &= CONFIG_EINT8;
	
	//GPG0设置为中断模式
	GPGCON &= ~(3 << 0); //先将后两位置零
	GPGCON |= CONFIG_GPG0_EINT8;
	EINTMASK &= CONFIG_EINT8_EN;
	
	//设置为双边沿触发
	EXTINT1 &= ~(7 << 0); //先将后三位置零
	EXTINT1 |= CONFIG_EINT8_MODE;
	
}

void interrupt_service()
{
	puts("\r\ninterrupt service\n\r");
	/* 指示那个IRQ发生了 */
	int bit = INTOFFSET;
	SRCPND = (1<<bit);
	INTPND = (1<<bit);
}