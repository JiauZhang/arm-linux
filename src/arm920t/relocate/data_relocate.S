.text
.global data_relocate
data_relocate:

    /* 重定位data段 */
    ldr r1, =data_load_addr  /* data段在bin文件中的地址, 加载地址 */
    ldr r2, =data_start      /* data段在重定位地址, 运行时的地址 */
    ldr r3, =data_end        /* data段结束地址 */

copy_data:
    ldrb r4, [r1] //从r1读到r4
    strb r4, [r2] //r4存放到r2
    add r1, r1, #1 //r1+1
    add r2, r2, #1 //r2+1
    cmp r2, r3 //r2 r3比较
    bne copy_data //如果不等则继续拷贝
	
	/* 清除BSS段 */
	ldr r1, =bss_start
	ldr r2, =bss_end
	mov r3, #0
clean_bss:
    strb r3, [r1]
    add r1, r1, #1
    cmp r1, r2
    bne clean_bss

    mov pc, lr