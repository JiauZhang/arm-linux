 /*
* 功能：实现LED1灯循环亮灭
* LED1--GPB5
* LED2--GPB6
* LED3--GPB7
* LED4--GPB8
*/
#define rGPBCON  (*(volatile unsigned long*)0x56000010)
#define rGPBDAT  (*(volatile unsigned long*)0x56000014)

#define Led1_On      ~(1<<5)
#define Led1_Off      (1<<5)

void Led_Port_Init(void);
void delay(int i);

int main(void)
{
 Led_Port_Init();
 while(1)
 {
  //Led1亮
  rGPBDAT&=Led1_On;
  delay(10000);
  //Led1灭
  rGPBDAT|=Led1_Off;
  delay(10000); 
 }

}

/*
* 函数名称：Led_Port_Init()
* 全局变量：无
* 参    数：无
* 返 回 值：无
* 说    明：实现Led1灯的寄存器初始化
*/
void Led_Port_Init(void)
{
   //设置GPB5为输出端口
    rGPBCON &= ~(3<<10);
    rGPBCON |= (1<<10); 
}
/*
* 函数名称：delay(int t)
* 全局变量：无
* 参    数：t
* 返 回 值：无
* 说    明：延时函数
*/
void delay(int t)
{
    int a,b;
    for(a=0;a<=t;a++)
  for(b=0;b<=100;b++);
}
