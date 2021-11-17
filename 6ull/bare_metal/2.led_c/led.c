#define CCM_CCGR1 (volatile unsigned long*)0x20c406c
#define IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO4 (volatile unsigned long*)0x20e006c
#define IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO4 (volatile unsigned long*)0x20e02f8
#define GPIO1_GDIR (volatile unsigned long*)0x0209c004
#define GPIO1_DR (volatile unsigned long*)0x0209c000

#define uint32_t unsigned int
void delay(uint32_t count)
{
	volatile uint32_t i=0;
	for(i=0; i<count; ++i)
	{
		__asm("NOP");
	}
}

int main()
{
	*(CCM_CCGR1) = 0xffffffff;
	*(IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO4) = 0x5;
	*(IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO4) = 0x1f838;
	*(GPIO1_GDIR) = 0x10;
	*(GPIO1_DR) = 0x0;

	
	while(1)
	{
		*(GPIO1_DR) = 0x0;
		delay(0xfffff);
		*(GPIO1_DR) = 1<<4;
		delay(0xfffff);
	}
}
