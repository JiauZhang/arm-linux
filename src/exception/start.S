
.text
.global _start

_start:
	b reset  /* vector 0 : reset */
	b do_und /* vector 4 : und */
	b do_swi
	b halt
	b halt
	b halt
	b do_irq
	b halt

.align 4

do_und:
	/* 执行到这里之前:
	 * 1. lr_und保存有被中断模式中的下一条即将执行的指令的地址
	 * 2. SPSR_und保存有被中断模式的CPSR
	 * 3. CPSR中的M4-M0被设置为11011, 进入到und模式
	 * 4. 跳到0x4的地方执行程序 
	 */

	/* sp_und未设置, 先设置它 */
	ldr sp, =0x34000000

	/* 在und异常处理函数中有可能会修改r0-r12, 所以先保存 */
	/* lr是异常处理完后的返回地址, 也要保存 */
	stmdb sp!, {r0-r12, lr}  
	
	/* 保存现场 */
	/* 处理und异常 */
	mrs r0, cpsr
	bl printHex
	//ldr r1, =und_string
	//bl printException
	ldr r0, =und_string
	bl puts
	
	/* 恢复现场 */
	ldmia sp!, {r0-r12, pc}^  /* ^会把spsr的值恢复到cpsr里 */
	
und_string:
	.string "\r\nundefined instruction exception\r\n"

.align 4

reset:
	/* 关闭看门狗 */
	ldr r0, =0x53000000
	ldr r1, =0
	str r1, [r0]

	/* 设置MPLL, FCLK : HCLK : PCLK = 400m : 100m : 50m */
	/* LOCKTIME(0x4C000000) = 0xFFFFFFFF */
	ldr r0, =0x4C000000
	ldr r1, =0xFFFFFFFF
	str r1, [r0]

	/* CLKDIVN(0x4C000014) = 0X5, tFCLK:tHCLK:tPCLK = 1:4:8  */
	ldr r0, =0x4C000014
	ldr r1, =0x5
	str r1, [r0]

	/* 设置CPU工作于异步模式 */
	mrc p15,0,r0,c1,c0,0
	orr r0,r0,#0xc0000000   //R1_nF:OR:R1_iA
	mcr p15,0,r0,c1,c0,0

	/* 设置MPLLCON(0x4C000004) = (92<<12)|(1<<4)|(1<<0) 
	 *  m = MDIV+8 = 92+8=100
	 *  p = PDIV+2 = 1+2 = 3
	 *  s = SDIV = 1
	 *  FCLK = 2*m*Fin/(p*2^s) = 2*100*12/(3*2^1)=400M
	 */
	ldr r0, =0x4C000004
	ldr r1, =(92<<12)|(1<<4)|(1<<0)
	str r1, [r0]

	/* 一旦设置PLL, 就会锁定lock time直到PLL输出稳定
	 * 然后CPU工作于新的频率FCLK
	 */
	

	/* 设置内存: sp 栈 */
	/* 分辨是nor/nand启动
	 * 写0到0地址, 再读出来
	 * 如果得到0, 表示0地址上的内容被修改了, 它对应ram, 这就是nand启动
	 * 否则就是nor启动
	 */
	mov r1, #0
	ldr r0, [r1] /* 读出原来的值备份 */
	str r1, [r1] /* 0->[0] */ 
	ldr r2, [r1] /* r2=[0] */
	cmp r1, r2   /* r1==r2? 如果相等表示是NAND启动 */
	ldr sp, =0x40000000+4096 /* 先假设是nor启动 */
	moveq sp, #4096  /* nand启动 */
	streq r0, [r1]   /* 恢复原来的值 */

	bl sdram_init

	bl relocate

	/* 复位之后, cpu处于svc模式
	 * 现在, 切换到usr模式
	 */
	mrs r0, cpsr      /* 读出cpsr */
	bic r0, r0, #0xf  /* 修改M4-M0为0b10000, 进入usr模式 */
	msr cpsr, r0

	bl uart0_init

	ldr r0, =exception_tip_1
	bl puts
	/* 故意加入一条未定义指令 */
und_code:
	.word 0xdeadc0de  /* 未定义指令 */
	ldr r0, =exception_tip_2
	bl puts

	swi 0x123

	//bl main  /* 使用BL命令相对跳转, 程序仍然在NOR/sram执行 */
	ldr pc, =main  /* 绝对跳转, 跳到SDRAM */

exception_tip_1:
	.string "\r\nundefined instruction start\r\n"

exception_tip_2:
	.string "\r\nundefined instruction end\r\n"

.align 4

do_swi:
	/* 执行到这里之前:
	 * 1. lr_svc保存有被中断模式中的下一条即将执行的指令的地址
	 * 2. SPSR_svc保存有被中断模式的CPSR
	 * 3. CPSR中的M4-M0被设置为10011, 进入到svc模式
	 * 4. 跳到0x08的地方执行程序 
	 */

	/* sp_svc未设置, 先设置它 */
	ldr sp, =0x33e00000

	/* 保存现场 */
	/* 在swi异常处理函数中有可能会修改r0-r12, 所以先保存 */
	/* lr是异常处理完后的返回地址, 也要保存 */
	stmdb sp!, {r0-r12, lr}  
	/* 
		swi中断码和swi指令形成一个4字节的指令
		其中高一个字节为指令，后三个字节为中断码
	*/
	/* swi中断发生的下一条指令地址 */
	mov r4, lr
	
	/* 处理swi异常 */
	mrs r0, cpsr
	bl printHex
	ldr r0, =swi_string
	bl puts
	/* 减去4个字节得到swi指令地址，中断码就在其中 */
	sub r0, r4, #4
	ldr r1, [r0]
	mov r0, r1
	bl printHex
	
	/* 恢复现场 */
	ldmia sp!, {r0-r12, pc}^  /* ^会把spsr的值恢复到cpsr里 */
	
swi_string:
	.string "\r\nswi exception\r\n"


.align 4

do_irq:
	/* 执行到这里之前:
	 * 1. lr_irq保存有被中断模式中的下一条即将执行的指令的地址
	 * 2. SPSR_irq保存有被中断模式的CPSR
	 * 3. CPSR中的M4-M0被设置为10010, 进入到irq模式
	 * 4. 跳到0x18的地方执行程序 
	 */

	/* sp_irq未设置, 先设置它 */
	ldr sp, =0x33d00000

	/* 保存现场 */
	/* 在irq异常处理函数中有可能会修改r0-r12, 所以先保存 */
	/* lr-4是异常处理完后的返回地址, 也要保存 */
	sub lr, lr, #4
	stmdb sp!, {r0-r12, lr}  
	
	/* 处理irq异常 */
	bl interrupt_service
	
	/* 恢复现场 */
	ldmia sp!, {r0-r12, pc}^  /* ^会把spsr_irq的值恢复到cpsr里 */

.align 4
	
halt:
	b halt
	
