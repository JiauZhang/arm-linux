#define CLKDIVN 0x4c000014
#define MPLLCON 0x4c000004
#define MPLL_405MHz ((127<<12)|(2<<4)|(1<<0))
#define MPLL_400MHz ((92<<12)|(1<<4)|(1<<0))

@export clock_init

.text
.global clock_init
clock_init:
	@set clock divide factor
	ldr r0,=CLKDIVN
	mov r1,#0x5           @set HDIVN=2 PDIVN=1
	str r1,[r0]

	@set asynchronous work mode
	mrc p15,0,r0,c1,c0,0  @read control register
	orr r0,r0,#0xc0000000
	mcr p15,0,r0,c1,c0,0  @write control register	
	
	@set FCLK
	ldr r0,=MPLLCON
	ldr r1,=MPLL_405MHz
	@ldr r1,=MPLL_400MHz
	str r1,[r0]
	
	mov pc,lr
