### 1. 概述
之前已经写了几篇Linux内核启动相关的文章，比如：《[解压内核镜像](http://mp.weixin.qq.com/s?__biz=MzUzNjU2OTkyOA==&mid=2247484463&idx=1&sn=1dc7706fccd141ecbdb2704d785de0d2&chksm=faf57508cd82fc1e333787dcece7f524165035422cc512313ecddb1ceb1d22b9e9605c4d8332&scene=21#wechat_redirect)》《[调用 start_kernel](http://mp.weixin.qq.com/s?__biz=MzUzNjU2OTkyOA==&mid=2247484478&idx=1&sn=8f4c367406b47c84fdd8a6674ce47100&chksm=faf57519cd82fc0f6efbeb8c7778d2f4929e73adec5c46bf7da5085aa0cb3d5717dda832d7ec&scene=21#wechat_redirect)》都是用汇编语言写的，这些代码的作用仅仅是把内核镜像放置到特定的位置，同时配置好C语言的运行环境，再有就是简单的把内核镜像所在区域的页表设置一下，在开启MMU之后就正式开始了C语言代码的执行，C语言代码的入口是start_kernel这个函数，本文要介绍其中的set_arch这个函数，该函数的作用是查找给定机器ID的数据结构信息、配置内存条信息、解析bootloader传递命令行参数，然后根据machine_desc结构体所记录的信息对机器进行一些必要的设置，最后开始正式建立完整的页表，大致流程如下图所示。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201122103313892.PNG)
### 2. set_processor
该函数首先调用汇编代码来查找给定机器ID的proc_info数据，找到之后取出其中的processor结构体，该结构体中包含了很多任务切换相关的底层函数。

```c
/* arch/arm/kernel/setup.c */
list = lookup_processor_type(read_cpuid_id());
/* arch/arm/kernel/head-common.S */
ENTRY(lookup_processor_type)
 stmfd sp!, {r4 - r6, r9, lr}
 mov r9, r0
 bl __lookup_processor_type
 mov r0, r5
 ldmfd sp!, {r4 - r6, r9, pc}
ENDPROC(lookup_processor_type)
```
cacheid_init函数根据CPU ID设置缓冲相关的标志位；cpu_init调用刚刚找到的processor中的processor._proc_init函数，不过该函数没有做什么实际操作。

```c
/* arch/arm/mm/proc-v7.S */
ENTRY(cpu_v7_proc_init)
 mov pc, lr
ENDPROC(cpu_v7_proc_init)
```
设置内核启动时所在CPU不同异常模式下的栈指针。

```c
/* arch/arm/kernel/setup.c::cpu_init */
 __asm__ (
 "msr cpsr_c, %1\n\t"
 "add r14, %0, %2\n\t"
 "mov sp, r14\n\t"
 "msr cpsr_c, %3\n\t"
 "add r14, %0, %4\n\t"
 "mov sp, r14\n\t"
 "msr cpsr_c, %5\n\t"
 "add r14, %0, %6\n\t"
 "mov sp, r14\n\t"
 "msr cpsr_c, %7"
     :
     : "r" (stk),
       PLC (PSR_F_BIT | PSR_I_BIT | IRQ_MODE),
       "I" (offsetof(struct stack, irq[0])),
       PLC (PSR_F_BIT | PSR_I_BIT | ABT_MODE),
       "I" (offsetof(struct stack, abt[0])),
       PLC (PSR_F_BIT | PSR_I_BIT | UND_MODE),
       "I" (offsetof(struct stack, und[0])),
       PLC (PSR_F_BIT | PSR_I_BIT | SVC_MODE)
     : "r14");
```
### 3. setup_machine_tags
根据机器ID查找machine_desc结构体，如果没有找到就打印一条提示信息，然后直接宕机。而此时使用的打印函数是early_print的话，再根据《[printk流程分析](http://mp.weixin.qq.com/s?__biz=MzUzNjU2OTkyOA==&mid=2247484610&idx=1&sn=9d9d2ce3e05ae93b7d16eb362ad79c38&chksm=faf575e5cd82fcf37f7d971403c119db0b517f99a609c2cce7ef3c68230c19977c320a2301c4&scene=21#wechat_redirect)》，此时其实还没有注册console驱动，因此如果没有打开early_printk功能，则系统就悄无声息的死机了。

```c
/* arch/arm/kernel/setup.c */
for_each_machine_desc(p)
  if (nr == p->nr) {
   printk("Machine: %s\n", p->name);
   mdesc = p;
   break;
  }
```
找到mdesc之后，执行mdesc->fixup()，该调用实际执行的函数是定义在cpu.c中的cpu_fixup函数，该函数的作用是设置内存条个数以及对应物理起始地址和大小。

```c
/* arch/arm/kernel/setup.c */
if (mdesc->fixup)
 mdesc->fixup(tags, &from, &meminfo);
/* arch/arm/mach-s5p4418/cpu.c */
MACHINE_START(S5P4418, NXP_MACH_NAME)
 .atag_offset =  0x00000100,
 .fixup   =  cpu_fixup,
 .map_io   =  cpu_map_io,
 .init_irq  =  nxp_cpu_init_irq,
#ifdef CONFIG_ARM_GIC
 .handle_irq  =  gic_handle_irq,
#else
 .handle_irq  =  vic_handle_irq,
#endif
 .timer   = &nxp_cpu_sys_timer,
 .init_machine =  cpu_init_machine,
#if defined CONFIG_CMA && defined CONFIG_ION
 .reserve  =  cpu_mem_reserve,
#endif
MACHINE_END
static void __init cpu_fixup(...)
{
 mi->nr_banks      = 1;
 mi->bank[0].start = CFG_MEM_PHY_SYSTEM_BASE;
#if !defined(CFG_MEM_PHY_DMAZONE_SIZE)
 mi->bank[0].size  = CFG_MEM_PHY_SYSTEM_SIZE;
#else
 mi->bank[0].size  = CFG_MEM_PHY_SYSTEM_SIZE + CFG_MEM_PHY_DMAZONE_SIZE;
#endif
}
```
接下来就是解析bootloader传递的命令行参数，通过tag->hdr.tag查找内核中预置的对应类型tag的解析函数，然后调用对应类型tag的parse函数即可实现对参数的解析。

```c
/* arch/arm/kernel/setup.c */
static int __init parse_tag(const struct tag *tag)
{
 extern struct tagtable __tagtable_begin, __tagtable_end;
 struct tagtable *t;

 for (t = &__tagtable_begin; t < &__tagtable_end; t++)
  if (tag->hdr.tag == t->tag) {
   t->parse(tag);
   break;
  }

 return t < &__tagtable_end;
}
/* arch/arm/kernel/setup.h */
struct tagtable {
 __u32 tag;
 int (*parse)(const struct tag *);
};
```
### 4. 总结
为了避免文章篇幅太长，所以会拆分成三四篇来写，下面是本文的总结：

- setup_processor：根据给定机器ID查找机器描述信息，然后再根据CPU ID设置cache相关的标志位，再执行processor._proc_init对处理器进行初始化，最后设置CPU不同异常模式下的栈指针；
- setup_machine_tags：根据机器ID查找machine_desc结构体，然后执行cpu_fixup函数配置内存条信息，最后解析bootloader传递的命令行参数。
