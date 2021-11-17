#include "MCIMX6Y2.h"
#include "fsl_iomuxc.h"
#include "pad_config.h"

#include "button.h"
#include "led.h"
#include "clock.h"

void delay(uint32_t count)
{
	volatile uint32_t i;

	for(i=0; i<count; ++i)
	{
		__asm("NOP");
	}
}

void main()
{
	
	system_clock_init();
	led_init();
//	button_interrupt_init();

	led_on;

	while(1)
	{
		led_on;
		delay(0xfffff);
		led_off;
		delay(0xfffff);
	}
}

