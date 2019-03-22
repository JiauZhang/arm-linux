#ifndef _UART_H_
#define _UART_H_

void uart0_init();
int putchar(unsigned char c);
void putint(int num);
void putuint(unsigned int num);
int getchar(void);
int puts(const char *s);
void printHex(unsigned int val);
void print1(void);
void print2(void);

#endif

