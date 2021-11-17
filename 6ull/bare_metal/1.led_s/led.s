.text
.align 2
.global _start

//#define gpio1_clock_ccm_ccgr1   0x20c406c
//#define gpio1_io4_mux_ctl_reg   0x20e006c
//#define gpio1_io4_pad_ctl_reg   0x20e02f8
//#define gpio1_io4_gdir_reg      0x0209c004
//#define gpio1_io4_dr            0x0209c000

_start:
    b reset

reset:
    ldr r0,=0x20c406c
    ldr r1,=0xffffffff
    str r1,[r0]

    ldr r0,=0x20e006c
    ldr r1,=0x5
    str r1,[r0]

    ldr r0,=0x20e02f8
    ldr r1,=0x1f838
    str r1,[r0]

    ldr r0,=0x0209c004
    ldr r1,=0x10
    str r1,[r0]

    ldr r0,=0x0209c000
    ldr r1,=0x0
    str r1,[r0]

loop:
    b loop

    
