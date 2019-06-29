### 二进制文件问题
当程序中出现全局变量和静态变量时，二进制文件会变得非常大，反汇编如下：
```
Disassembly of section .rodata:

000007ec <global_char2>:
 7ec:	00000062 	andeq	r0, r0, r2, rrx
 7f0:	626f6c67 	rsbvs	r6, pc, #26368	; 0x6700
 7f4:	63206c61 			; <UNDEFINED> instruction: 0x63206c61
 7f8:	3a726168 	bcc	1c98da0 <__bss_end__+0x1c88544>
 7fc:	00000000 	andeq	r0, r0, r0
 800:	626f6c67 	rsbvs	r6, pc, #26368	; 0x6700
 804:	69206c61 	stmdbvs	r0!, {r0, r5, r6, sl, fp, sp, lr}
 808:	003a746e 	eorseq	r7, sl, lr, ror #8
 80c:	61636f6c 	cmnvs	r3, ip, ror #30
 810:	6863206c 	stmdavs	r3!, {r2, r3, r5, r6, sp}^
 814:	003a7261 	eorseq	r7, sl, r1, ror #4
 818:	626f6c67 	rsbvs	r6, pc, #26368	; 0x6700
 81c:	61206c61 			; <UNDEFINED> instruction: 0x61206c61
 820:	00006464 	andeq	r6, r0, r4, ror #8
 824:	61636f6c 	cmnvs	r3, ip, ror #30
 828:	6461206c 	strbtvs	r2, [r1], #-108	; 0xffffff94
 82c:	00000064 	andeq	r0, r0, r4, rrx
 830:	00007830 	andeq	r7, r0, r0, lsr r8
 834:	0a636261 	beq	18d91c0 <__bss_end__+0x18c8964>
 838:	0000000d 	andeq	r0, r0, sp
 83c:	0a333231 	beq	ccd108 <__bss_end__+0xcbc8ac>
 840:	0000000d 	andeq	r0, r0, sp
 844:	20495753 	subcs	r5, r9, r3, asr r7
 848:	206c6176 	rsbcs	r6, ip, r6, ror r1
 84c:	0000203d 	andeq	r2, r0, sp, lsr r0
 850:	Address 0x0000000000000850 is out of bounds.
 
Disassembly of section .data:

00010853 <global_char1>:
   10853:	Address 0x0000000000010853 is out of bounds.


Disassembly of section .bss:

00010854 <global_int1>:
   10854:	00000000 	andeq	r0, r0, r0

00010858 <global_int2>:
   10858:	00000000 	andeq	r0, r0, r0
```
注意，这里之所以出现问题是因为这些全局变量和静态变量造成的，全局常量和局部常量是直接和代码
放在一起的，可以看到，.rodata和代码段还是紧挨着的，而.data段就出现了巨大地址空间的跳跃
解决办法就是手动指定数据段地址*-Tdata address*即可，这里指定为*-Tdata 0x900*。
程序分布：
```
	.text 代码段
	.data 数据段
	rodata 只读数据段(const全局变量)
	bss段 (初始值为0，无初始值的全局变量)
	commen 注释
```
### Makefile编译错误
目标的依赖文件一定要写源文件，如果写的是源文件生成的*.o文件，编译器就会根据隐规则自动寻找对应的源文件
自动编译生成，而交叉编译需要在Makefile中指定各种参数，当自动编译时编译器并不会使用我们的参数，所以会
导致编译报错
### relocate程序运行输出
```
global char:
ab
global int:
-1-1
local char:
cd
global add
local add
global char:
ab
global int:
-1-1
local char:
dd
global add
local add
global char:
ab
global int:
-1-1
local char:
ed
global add
local add
global char:
ab
....
```
从输出可以看出，只有局部非常量变量是可以改变的，这是因为其他的变量(全局、静态、常量)都固化在了Nor FLASH上，
而Nor FLASH在运行期间是不可写的，只可读，而局部变量则是通过栈的方式分配的内存地址(这里需要说明的是，
栈的初始化在init.S中完成的)，所以是可读可写的。
### 解决方法一
- 将布局在Nor FLASH上的数据想办法放到RAM中，这样就可以进行正常的读写操作了，如果使用*-Tdata=address*，
可用的RAM有系统自带的4KB SRAM和外设SDRAM可用，地址分别为0x40000000-0x40001000和0x30000000-0x34000000，
但是这样做有一个问题，就是在生成二进制文件时会造成文件特别大，主要是因为从代码段和数据段之间什么也没有，
全用0来填充了，所以这个方法肯定是不可取的，所以需要使用更高级的方法来控制编译器如何链接生成二进制文件。
- 这里使用*.lds文件来告诉链接器如何生成二进制文件：
```
SECTIONS {
   .text   0  : { *(.text) }
   .rodata  : { *(.rodata) }
   .data 0x30000000 : AT(0x900) 
   { 
      data_load_addr = LOADADDR(.data);
      data_start = . ;
      *(.data)
      data_end = . ;
   }
   .bss  : { *(.bss) *(.COMMON) }
}
```
链接脚本用法为：
```
段名 程序运行地址：AT(程序在二进制文件中的地址，即布局在哪里){每个程序中的段名}
```
这个链接脚本中，代码段和只读数据段还是和原来一样，不同的是把数据段的运行地址指定在了SDRAM中，为了防止生成的
二进制文件很大，把数据段的布局位置放在了代码和只读数据段之后，这样既可以保证对变量的读写，也可以保证
二进制文件的大小不会变的特别大。此外由于程序的运行地址是被安排到了SDRAM中，所以二进制代码中对数据段的访问
也就都是相对SDRAM来进行的，而下载到FLASH中的数据段还是只存在于FLASH中，所以需要程序自己把这个数据段自动的
搬运到SDRAM的对应位置，这样才可以进行正常的读写操作，最后程序输出如下，可以看出，输出正常。
```
global char:
ab
global int:
5-869020620
local char:
cd
global add
local add
=================
global char:
bb
global int:
6-869020619
local char:
dd
global add
local add
=================
global char:
cb
global int:
7-869020618
local char:
ed
global add
local add
=================
global char:
db
global int:
8-869020617
local char:
fd
global add
local add
....
```
### 解决方法二
将整个整个二进制文件全部重定位到SDRAM中，这样做就要求在NOR FLASH中执行的程序必须位置无关，即：
```
使用相对跳转命令 b或bl;
重定位之前，不可使用绝对地址，不可访问全局变量/静态变量，也不可访问有初始值的数组(因为初始值放在rodata里，使用绝对地址来访问)；
重定位之后，使用ldr pc = xxx，跳转到/runtime地址；
写位置无关码，其实就是不使用绝对地址，判断有没有使用绝对地址，除了前面的几个规则，最根本的办法看反汇编。
```
### 总结
- 输出换行不只是回车，正确的用法为："\n\r"
- 链接脚本*段名*之后一定要加空格与*:*冒号隔开，否则make报错

