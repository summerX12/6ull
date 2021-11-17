#define m_DDR_start	0x80000000
#define m_DDR_size	0x20000000

#define m_text_start	0x80002000

#define SUP_model_stack_start	0x9FE00000
#define SUP_model_stack_size	0x00100000

#define IRQ_model_stack_start	0x9FF00000
#define IRQ_model_stack_size	0x00100000


.text
.align 2
.global _start

_start:
	ldr pc,=Reset_Handler
	ldr pc,=Undefined_Handler
	ldr pc,=SVC_Handler
	ldr pc,=PrefAbort_Handler
	ldr pc,=DataAbort_Handler
	.word	0
	ldr pc,=IRQ_Handler
	ldr pc,=FIQ_Handler

Reset_Handler:
	cpsid i

	mrc p15,0,r0,c1,c0,0
	bic r0,r0,#(0x1 << 12)
	bic r0,r0,#(0x1 << 2)
	bic r0,r0,#0x2
	bic r0,r0,#(0x1 << 11)
	bic r0,r0,#0x1
	mcr p15,0,r0,c1,c0,0

	cps #0x12
	ldr sp,=0x9FF00000

	cps #0x1f
	ldr sp,=0X9FE00000

	cps #0x13
	ldr sp,=0X9FE00000

	ldr r2,=SystemInit
	blx r2

	cpsie i


	b main
	b .


Undefined_Handler:
	b Undefined_Handler
	.size	Undefined_Handler, . - Undefined_Handler

	.align 2
	.arm
	.weak SVC_Handler
	.type SVC_Handler,%function

SVC_Handler:
	ldr r0,=SVC_Handler
	bx  r0
	.size	SVC_Handler, . - SVC_Handler

	.align 2
	.arm
	.weak PrefAbort_Handler
	.type PrefAbort_Handler,%function

PrefAbort_Handler:
	ldr r0,=PrefAbort_Handler
	bx  r0
	.size	PrefAbort_Handler, . - PrefAbort_Handler

	.align 2
	.arm
	.weak DataAbort_Handler
	.type DataAbort_Handler,%function


DataAbort_Handler:
	ldr r0,=DataAbort_Handler
	bx  r0
	.size	DataAbort_Handler, . - DataAbort_Handler

	.align 2
	.arm
	.weak IRQ_Handler
	.type IRQ_Handler,%function


IRQ_Handler:
	push {lr}
	push {r0-r3,r12}

	MRS  r0,spsr
	push {r0}
	
	MRC  p15,4,r1,c15,c0,0
	ADD  r1,r1,#0x2000
	LDR  r0,[r1,#0xc]

	push {r0,r1}

	CPS  #0X13

	push {lr}
	ldr  r2,=SystemIrqHandler
	blx  r2

	POP  {r0,r1}

	CPS  #0X12

	POP  {r0,r1}

	STR  r0,[r1,#0x10]

	POP  {r0}
	MSR  spsr_cxsf,r0

	POP  {r0-r3,r12}
	POP  {lr}
	SUBS pc,lr,#4
	.size IRQ_Handler, . - IRQ_Handler

	.align 2
	.arm
	.weak FIQ_Handler
	.type FIQ_Handler,%function

FIQ_Handler:
	ldr r0,=FIQ_Handler
	bx  r0
	.size FIQ_Handler, . - FIQ_Handler

	.end

