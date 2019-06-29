### 串口测试
1.init.S初始化系统时钟，其中串口时钟为50MHz，根据波特率115200设置对应的寄存器
2.出现如下编译错误提示：
        arm-linux/src/uart/uart.c:77: undefined reference to `__stack_chk_fail'
        arm-linux/src/uart/uart.c:77: undefined reference to `__stack_chk_guard'
错误解决方法：
        添加编译选项：-fno-stack-protector

