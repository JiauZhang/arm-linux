#define    GPBCON       (*(volatile unsigned long *)0x56000010)
#define    GPBDAT        (*(volatile unsigned long *)0x56000014)
#define    GPB5_out      (1<<(5*2))
#define    GPB6_out      (1<<(6*2))
#define    GPB7_out      (1<<(7*2))
#define    GPB8_out      (1<<(8*2))
/*
void  wait(unsigned long dly)
{
       for(; dly > 0; dly--);
}
int main(void)
{
       unsigned long i = 0;      
       GPBCON = GPB5_out|GPB6_out|GPB7_out|GPB8_out;  // 将LED1-4对应的GPB5/6/7/8四个引脚设为输出
       while(1){
              wait(30000);
              GPBDAT = (~(i<<5));        // 根据i的值，点亮LED1-4
              if(++i == 16){
                     i = 0;
              }
       }
       return 0;
}
*/

//extern void stack_init();

int main(void)
{
	//stack_init();
	unsigned long delay;
	GPBCON = GPB5_out;
	GPBDAT = 0xffffffff;
	while(1)
	{
		for(delay = 50000;delay>0;delay--);
		GPBDAT = 0xffffffdf;
		for(delay = 50000;delay>0;delay--);
		GPBDAT = 0xffffffff;
	}
	return 0;
}

