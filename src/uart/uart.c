#include "../include/s3c2440.h"

/* 115200,8n1 */
void uart0_init()
{
	/* �����������ڴ��� */
	/* GPH2,3����TxD0, RxD0 */
	GPHCON &= ~((3<<4) | (3<<6));
	GPHCON |= ((2<<4) | (2<<6));

	GPHUP &= ~((1<<2) | (1<<3));  /* ʹ���ڲ����� */
	

	/* ���ò����� */
	/* UBRDIVn = (int)( UART clock / ( buad rate x 16) ) �C1
	 *  UART clock = 50M
	 *  UBRDIVn = (int)( 50000000 / ( 115200 x 16) ) �C1 = 26
	 */
	UCON0 = 0x00000005; /* PCLK,�ж�/��ѯģʽ */
	UBRDIV0 = 26;

	/* �������ݸ�ʽ */
	ULCON0 = 0x00000003; /* 8n1: 8������λ, �޽���λ, 1��ֹͣλ */

	/*  */

}

int putchar(unsigned char c)
{
	/* UTRSTAT0 */
	/* UTXH0 */

	while (!(UTRSTAT0 & (1<<2)));
	UTXH0 = c;
	return 0;
}

void putint(int num)
{
	if (num<0) {
		putchar('-');
		num = -num;
	}
	
	if (num>=0 && num<=9) {
		putchar('0'+num);
	} else {
		char nums[10];
		int i = 0;
		
		for (; i<10; i++) {
			nums[i] = 'f';
		}
		
		i = 0;
		while (num) {
			nums[i++] = num % 10;
			num = num /10;
		}
		
		i--;
		for (; i>=0; i--) {
			putchar('0'+nums[i]);
		}
	}	
}

void putuint(unsigned int num)
{
	if (num>=0 && num<=9) {
		putchar('0'+num);
	} else {
		char nums[10];
		int i = 0;
		
		for (; i<10; i++) {
			nums[i] = 'f';
		}
		
		i = 0;
		while (num) {
			nums[i++] = num % 10;
			num = num /10;
		}
		
		i--;
		for (; i>=0; i--) {
			putchar('0'+nums[i]);
		}
	}	
}

int getchar(void)
{
	while (!(UTRSTAT0 & (1<<0)));
	return URXH0;
}

int puts(const char *s)
{
	while (*s)
	{
		putchar(*s);
		s++;
	}
	return 0;
}

/* 0xABCDEF12 */
void printHex(unsigned int val)
{
	int i;
	unsigned int arr[8];

	/* ��ȡ��ÿһλ��ֵ */
	for (i = 0; i < 8; i++)
	{
		arr[i] = val & 0xf;
		val >>= 4;   /* arr[0] = 2, arr[1] = 1, arr[2] = 0xF */
	}

	/* ��ӡ */
	puts("0x");
	for (i = 7; i >=0; i--)
	{
		if (arr[i] >= 0 && arr[i] <= 9)
			putchar(arr[i] + '0');
		else if(arr[i] >= 0xA && arr[i] <= 0xF)
			putchar(arr[i] - 0xA + 'A');
	}
}
