### 步骤 1
关闭中断、进入 SVC 模式
```c
ENTRY(stext)
	 THUMB(	adr	r9, BSYM(1f)	)	@ Kernel is always entered in ARM.
	 THUMB(	bx	r9		)	@ If this is a Thumb-2 kernel,
	 THUMB(	.thumb			)	@ switch to Thumb now.
	 THUMB(1:			)
		setmode	PSR_F_BIT | PSR_I_BIT | SVC_MODE, r9 @ 关中断、进入 SVC 模式
```

### 步骤 2
查找指定处理器类型的 proc_info
```c
	mrc	p15, 0, r9, c0, c0		@ 取出处理器 ID 放入寄存器 r9 中
	bl	__lookup_processor_type		@ 查找处理器类型 r5=procinfo r9=cpuid
	|
	|-->/* 找到匹配 proc_info 则返回，否则将 r5 清零 */
	|		__CPUINIT
	|	__lookup_processor_type:
	|		adr	r3, __lookup_processor_type_data
	|			|
	|			|-->.align	2
	|			|		.type	__lookup_processor_type_data, %object
	|			|	__lookup_processor_type_data:
	|			|		.long	.
	|			|		.long	__proc_info_begin
	|			|		.long	__proc_info_end
	|			|		.size	__lookup_processor_type_data, . - __lookup_processor_type_data
	|		ldmia	r3, {r4 - r6}	@ r4=当前数据地址、r5=处理器数据起始地址、r6=结束地址
	|		sub	r3, r3, r4			@ 计算出运行地址和链接地址间的偏移
	|		add	r5, r5, r3			@ 修正 r5
	|		add	r6, r6, r3			@ 修正 r6
	|	1:	ldmia	r5, {r3, r4}
	|		and	r4, r4, r9
	|		teq	r3, r4
	|		beq	2f					@ 如果相等则匹配成功
	|		add	r5, r5, #PROC_INFO_SZ	@ 开始指向下一个处理器数据
	|		cmp	r5, r6
	|		blo	1b					@ 如果还有数据则循环查找
	|		mov	r5, #0				@ 未找到时将 r5 清零
	|	2:	mov	pc, lr				@ 返回
	|	ENDPROC(__lookup_processor_type)
	
	movs	r10, r5			@ 使用 r5 改变标志位
	THUMB( it	eq )
	beq	__error_p			@ 如果相等则没找到
	
#ifndef CONFIG_XIP_KERNEL
	adr	r3, 2f				@ r3=运行地址
	ldmia	r3, {r4, r8}	@ r4=链接地址(虚拟地址)、r8=页偏移
	sub	r4, r3, r4			@ 运行地址与链接地址间的差值
	/*
	 * 内核被解压到 物理地址+text_offset 处，即 0x40008000，也是当前的运行地址
	 * 而内核在编译时被链接到 page_offset+text_offset 处，即 0xc0008000
	 * 因此 r4=r3-r4 记录的是内核实际存放的物理地址和运行时的虚拟地址间的偏移
	 * 即 r4=phys-page_offset
	 * 所以 r8 = r4+r8 = phys-page_offset+page_offset = phys，即物理地址的起始地址
	 */
	add	r8, r8, r4			@ 物理地址的起始地址
#else
	ldr	r8, =PHYS_OFFSET	@ always constant in this case
#endif

#ifndef CONFIG_XIP_KERNEL
2:	.long	.
	.long	PAGE_OFFSET
#endif
```

### 步骤 3
检查 bootloader 传递的启动参数是否有效
```c
/*
 * r1 = machine no, r2 = atags or dtb,
 * r8 = phys_offset, r9 = cpuid, r10 = procinfo
 */
bl	__vet_atags
	|
	+-->/* Returns:
	|	 *  r2 either valid atags pointer, valid dtb pointer, or zero
	|	 *  r5, r6 corrupted
	|	 */
	|	__vet_atags:
	|		tst	r2, #0x3			@ 判断 atags 是否 4 字节对齐
	|		bne	1f
	|
	|		ldr	r5, [r2, #0]
	|	#ifdef CONFIG_OF_FLATTREE	@ 配置此项时支持设备树
	|		ldr	r6, =OF_DT_MAGIC	@ 判断是否是 DTB 数据
	|		cmp	r5, r6
	|		beq	2f
	|	#endif
	|		cmp	r5, #ATAG_CORE_SIZE		@ 判断第一个 atags 参数的大小是否是与 ATAG_CORE 相同
	|		cmpne	r5, #ATAG_CORE_SIZE_EMPTY
	|		bne	1f
	|		ldr	r5, [r2, #4]
	|		ldr	r6, =ATAG_CORE			@ 再判断该参数是不是 ATAG_CORE 节点
	|		cmp	r5, r6
	|		bne	1f
	|
	|	2:	mov	pc, lr				@ 所传递参数合法，正常返回
	|
	|	1:	mov	r2, #0
	|		mov	pc, lr
	|	ENDPROC(__vet_atags)
```

### 步骤 4
当前内核镜像在内存中的布局
```c
// 物理内存中的布局
       _____________________________________________
      |      |          |                          |
      |      |          |                          |
      |      | 段描述符 |       kernel image       |
      |      |          |                          |
      |______|__________|__________________________|
0x4000_0000      0x4000_8000

// 虚拟内存中的布局
       _____________________________________________
      |      |          |                          |
      |      |          |                          |
      |      | 段描述符 |       kernel image       |
      |      |          |                          |
      |______|__________|__________________________|
0xc000_0000      0xc000_8000
```
内核建立内核空间临时的线性映射，采用一级映射，也就是 section 模式，每个section 为 1MB.
```c
#ifdef CONFIG_SMP_ON_UP
	bl	__fixup_smp			@ 自旋锁在 SMP 和 UP 上的相关修正
							@ arch/arm/include/asm::ALT_SMP
#endif
#ifdef CONFIG_ARM_PATCH_PHYS_VIRT
	bl	__fixup_pv_table	@ 物理地址和虚拟地址间的偏移修正等
							@ arch/arm/include/asm::pv_stub
#endif
	bl	__create_page_tables
	|	
	+-->/* r8 = phys_offset, r9 = cpuid, r10 = procinfo
	|	 *
	|	 * Returns:
	|	 *  r0, r3, r5-r7 corrupted
	|	 *  r4 = physical page table address
	|	 */
	|	__create_page_tables:
	|		pgtbl	r4, r8				@ 将页表起始物理地址放入 r4 中
	|			|
	|			+-->.macro	pgtbl, rd, phys
	|			|	add	\rd, \phys, #TEXT_OFFSET - PG_DIR_SIZE
	|			|	.endm
	|
	|		@ 对页表区域进行清零
	|		mov	r0, r4
	|		mov	r3, #0
	|		add	r6, r0, #PG_DIR_SIZE
	|	1:	str	r3, [r0], #4
	|		str	r3, [r0], #4
	|		str	r3, [r0], #4
	|		str	r3, [r0], #4
	|		teq	r0, r6
	|		bne	1b
	|
	|		ldr	r7, [r10, #PROCINFO_MM_MMUFLAGS] @ mm_mmuflags
	|
	|		@ 创建临时的线性映射
	|		@ 页表项格式：一级页表入口值[31:20]  MMUFLAGS[19:0]
	|		adr	r0, __turn_mmu_on_loc
	|		ldmia	r0, {r3, r5, r6}@ 得到函数的物理地址
	|		sub	r0, r0, r3			@ virt->phys offset
	|		add	r5, r5, r0			@ phys __turn_mmu_on
	|		add	r6, r6, r0			@ phys __turn_mmu_on_end
	|		mov	r5, r5, lsr #SECTION_SHIFT @ 得到一级页表入口值
	|		mov	r6, r6, lsr #SECTION_SHIFT
	|
	|	1:	orr	r3, r7, r5, lsl #SECTION_SHIFT	@ 一级段描述符
	|		str	r3, [r4, r5, lsl #PMD_ORDER]	@ 将 r3 中存放的段描述符放入对应的物理地址中
	|		cmp	r5, r6
	|		addlo	r5, r5, #1			@ 下一个段描述符
	|		blo	1b
	|
	|		@ 设置映射页表
	|		mov	r3, pc
	|		mov	r3, r3, lsr #SECTION_SHIFT @ 得到当前执行程序的段描述符编号
	|		orr	r3, r7, r3, lsl #SECTION_SHIFT @ 合成段描述符
	|		@ kernel_start=0xc000_8000, section_shift=20, pmd_order=2
	|		@ 以下两行其实是在计算段描述符的入口地址
	|		@ 因为要回写到 r0 中，因此拆分来写的
	|		add	r0, r4,  #(KERNEL_START & 0xff000000) >> (SECTION_SHIFT - PMD_ORDER)
	|		str	r3, [r0, #((KERNEL_START & 0x00f00000) >> SECTION_SHIFT) << PMD_ORDER]!
	|		ldr	r6, =(KERNEL_END - 1) @ 内核(包括数据段)的最后一个字节位置
	|		add	r0, r0, #1 << PMD_ORDER @ 下一个段描述符存放的物理地址
	|		add	r6, r4, r6, lsr #(SECTION_SHIFT - PMD_ORDER) @ 内核需要的最后一个段描述符存放的物理地址
	|	1:	cmp	r0, r6
	|		@ 内核对自身进行了线性映射，将自身物理内存所在段直接放入页表中
	|		add	r3, r3, #1 << SECTION_SHIFT @ 下一个段描述符，只需要增加段基址即可
	|		strls	r3, [r0], #1 << PMD_ORDER @ 写入到物理内存对应的页表中
	|		bls	1b
	|
	|		@ 将 atags 所在段写到页表中
	|		mov	r0, r2, lsr #SECTION_SHIFT @ atags 段编号
	|		movs	r0, r0, lsl #SECTION_SHIFT @ 如果 r0 为零则赋值为 r8，即没有指定 atags 的情况
	|		moveq	r0, r8
	|		sub	r3, r0, r8 @ 段内偏移量
	|		add	r3, r3, #PAGE_OFFSET @ 转化成虚拟地址
	|		add	r3, r4, r3, lsr #(SECTION_SHIFT - PMD_ORDER) @ 得到该段描述符存放的物理地址
	|		orr	r6, r7, r0 @ 合成段描述
	|		str	r6, [r3]   @ 写入物理内存中
	|
	|		mov	pc, lr
	|		ENDPROC(__create_page_tables)
	
	/*
	 * r10 = base of xxx_proc_info structure selected by __lookup_processor_type
	 * On return, the CPU will be ready for the MMU to be turned on, 
	 * r0 = CPU control register value.
	 */
	 /*
	  * 以下代码流程 
	  * 1. 设置v7核心，主要涉及SMP，准备MMU硬件配置，I/D cache，TLB，涉及协处理的配置
	  * --> arch/arm/mm/proc-v7.S::__v7_setup
	  * 2. 配置MMU，设置内存访问权限，并激活MMU
	  * --> arch/arm/kernel/head.S::__enable_mmu
	  * 3. 将数据段复制到内存中，清理bss段，将processor ID，machine ID，atags 指针保存到指定变量中
	  * --> arch/arm/kernel/head-common.S::__mmap_switched
	  * 4. __mmap_switched 最后进入C语言函数start_kernel，至此终于走出了汇编代码，进入C语言的天堂
	  * --> init/main.c::start_kernel
	  */
	@ 因为跳转到该函数时，MMU已激活，故这里使用的是虚拟地址，而不是物理地址
	ldr	r13, =__mmap_switched	@ address to jump to after
								@ mmu has been enabled
	adr	lr, BSYM(1f)			@ return (PIC) address
	mov	r8, r4					@ set TTBR1 to swapper_pg_dir
	ARM(	add	pc, r10, #PROCINFO_INITFUNC	)
	THUMB(	add	r12, r10, #PROCINFO_INITFUNC	)
	THUMB(	mov	pc, r12				)
1:	b	__enable_mmu
```

### 关键宏定义
```c
::arch/arm/kernel/vmlinux.ld.S
. = PAGE_OFFSET + TEXT_OFFSET
::arcm/arm/kernel/head.S
/*
 * swapper_pg_dir is the virtual address of the initial page table.
 * We place the page tables 16K below KERNEL_RAM_VADDR.  Therefore, we must
 * make sure that KERNEL_RAM_VADDR is correctly set.  Currently, we expect
 * the least significant 16 bits to be 0x8000, but we could probably
 * relax this restriction to KERNEL_RAM_VADDR >= PAGE_OFFSET + 0x4000.
 */
#define KERNEL_RAM_VADDR	(PAGE_OFFSET + TEXT_OFFSET)
#if (KERNEL_RAM_VADDR & 0xffff) != 0x8000
#error KERNEL_RAM_VADDR must start at 0xXXXX8000
#endif

#ifdef CONFIG_ARM_LPAE
	/* LPAE requires an additional page for the PGD */
#define PG_DIR_SIZE	0x5000
#define PMD_ORDER	3
#else
#define PG_DIR_SIZE	0x4000
#define PMD_ORDER	2
#endif

	.globl	swapper_pg_dir
	.equ	swapper_pg_dir, KERNEL_RAM_VADDR - PG_DIR_SIZE

	.macro	pgtbl, rd, phys
	add	\rd, \phys, #TEXT_OFFSET - PG_DIR_SIZE
	.endm

#ifdef CONFIG_XIP_KERNEL
#define KERNEL_START	XIP_VIRT_ADDR(CONFIG_XIP_PHYS_ADDR)
#define KERNEL_END	_edata_loc
#else
#define KERNEL_START	KERNEL_RAM_VADDR
#define KERNEL_END	_end
#endif
```