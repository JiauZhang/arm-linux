### 相关配置项
```c
config SMP
	bool "Symmetric Multi-Processing"
	depends on XXX --> 省略
	配置项帮助信息
	  当配置成 N 时，编译后的内核可以在单核和多核处理器上运行，但是如果机器是
	  多核的，那么只会使有其中的一个核心运行。如果配置成 Y，编译后的内核
	  可以在大多数(但并不是所有)的单核处理器上运行，如果处理器是单核的，
	  那么配置成 N 时运行速度会更高。

	  如果不知道该如何配置，那就配置成 N。

config SMP_ON_UP
	bool "Allow booting SMP kernel on uniprocessor systems (EXPERIMENTAL)"
	depends on XXX --> 省略
	default y --> 如果配置了 SMP，则默认配置此项为 Y
	配置项帮助信息
	  SMP 内核中包含有非 SMP 处理器之外的指令，配置该项可以让内核自己来修改
	  相关的指令，达到安全使用的目的，而关闭该选项却可以节省出约 1K 的空间。

	  如果不知道该如何配置，那就配置成 Y。

config NR_CPUS
	int "Maximum number of CPUs (2-32)"
	range 2 32
	depends on SMP
	default "4"
```

### 相关变量
根据内核配置时指定的核心数量，定义对应大小的数组 __cpu_logical_map。
```c
#define CONFIG_NR_CPUS 4

#ifndef CONFIG_NR_CPUS
#define CONFIG_NR_CPUS	1
#endif

#define NR_CPUS		CONFIG_NR_CPUS

int __cpu_logical_map[NR_CPUS];

#define cpu_logical_map(cpu)	__cpu_logical_map[cpu]
```

### 源码分析
```c
void __init smp_setup_processor_id(void)
{
	int i;
	// 首先判断当前 SoC 是否是 SMP，如果是则读取 CPUID，如果不是则置零
	u32 cpu = is_smp() ? read_cpuid_mpidr() & 0xff : 0;
	// 对 CPUID 进行逻辑映射
	cpu_logical_map(0) = cpu;
	for (i = 1; i < NR_CPUS; ++i)
		cpu_logical_map(i) = i == cpu ? 0 : i;
	// 内核启动过程中打印的第一条信息
	printk(KERN_INFO "Booting Linux on physical CPU %d\n", cpu);
}

static inline bool is_smp(void)
{
// 是 SMP 就返回 ture，否则返回 false
#ifndef CONFIG_SMP
	return false;
#elif defined(CONFIG_SMP_ON_UP)
	extern unsigned int smp_on_up;
	return !!smp_on_up;
#else
	return true;
#endif
}
// 从协处理器中读出 CPUID
#define read_cpuid(reg)							\
	({								\
		unsigned int __val;					\
		asm("mrc	p15, 0, %0, c0, c0, " __stringify(reg)	\
		    : "=r" (__val)					\
		    :							\
		    : "cc");						\
		__val;							\
	})

static inline unsigned int __attribute_const__ read_cpuid_mpidr(void)
{
	return read_cpuid(CPUID_MPIDR);
}
```