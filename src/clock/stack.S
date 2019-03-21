#define STACK_NOR_FLASH_BOOT (0x40000000+4096)
//ldr sp, =0x40000000+4096  
.text
.global stack_init
stack_init:
	LDR SP, =STACK_NOR_FLASH_BOOT
	
	@MOV PC, LR