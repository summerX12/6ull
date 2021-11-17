.text
.align 2
.global _start

_start:
	ldr sp,=0x84000000
	b main
loop:
	b loop
