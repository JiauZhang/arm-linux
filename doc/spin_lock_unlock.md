### 自旋锁
#### arch_spin_lock
```c
{
	unsigned long tmp;
	u32 newval;
	arch_spinlock_t lockval;
	// 处理器通知内存系统将对某个地址的内存进行读写
	prefetchw(&lock->slock);
	// 以独占的方式对 lock->tickets.next 执行 +1 操作
	__asm__ __volatile__(
"1:	ldrex	%0, [%3]\n"
"	add	%1, %0, %4\n"
"	strex	%2, %1, [%3]\n"
"	teq	%2, #0\n"
"	bne	1b"
	: "=&r" (lockval), "=&r" (newval), "=&r" (tmp)
	: "r" (&lock->slock), "I" (1 << TICKET_SHIFT)
	: "cc");
	// 判断是否轮到自己拥有自旋锁
	while (lockval.tickets.next != lockval.tickets.owner) {
		// 将当前核心挂起等待其他核心发送信号，也就是等 SEV 指令
		wfe();
		// 读取自旋锁中的 lock->tickets.owner
		lockval.tickets.owner = ACCESS_ONCE(lock->tickets.owner);
	}
	// 数据同步
	smp_mb();
}
```

#### arch_spin_unlock
```c
{
	// 数据同步
	smp_mb();
	// 将自旋锁交给下一个使用者
	lock->tickets.owner++;
	// 先进行数据同步，然后给其他核心发信号
	dsb_sev();
}
```