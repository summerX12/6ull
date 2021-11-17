#include "button.h"
#include "pad_config.h"
#include "system_MCIMX6Y2.h"

uint8_t button_status=0;

void delay(uint32_t count)
{
	volatile uint32_t i=0;
	for(i=0; i<count; ++i)
	{
		__asm("NOP");
	}
}

void button_init(void)
{
//button gpio5 1
	CCM_CCGR1_CG15(0X3);

	IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01,0);
	IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01,button_PAD_CONFIG_DATA);

	GPIO1->GDIR &= ~(1<<1);
}

int get_button_status(void)
{
	if( (GPIO5->DR)&(1<<1) )//BUTTON PRESS
	{
		delay(0xff);
		if( (GPIO5->DR)&(1<<1) )
		{
			return 1;
		}
	}
	return 0;

}

void button_interrupt_init(void)
{
	volatile uint32_t *icr;
	uint32_t icrShift;

	SystemInstallIrqHandler(GPIO5_Combined_0_15_IRQn,(system_irq_handler_t)EXAMPLE_GPIO_IRQHandler,NULL);
	GIC_EnableIRQ(GPIO5_Combined_0_15_IRQn);

	CCM_CCGR1_CG15(0X3);
	IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01,0);
	IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01,button_PAD_CONFIG_DATA);

	GPIO5->IMR &= ~(1<<1U);
	GPIO5->GDIR &= ~(1<<1);

	//TRIGGER
	GPIO5->EDGE_SEL &= ~(1U << 1);

//	if(1 <16)
//	{
		icr = &(GPIO5->ICR1);
/*	}
	else
	{
		icr = &(GPIO5->ICR2);
		icrShift -= 16;
	}
*/
	*icr = (*icr & (~(3U << (2*icrShift)))) | (2U << (2*icrShift));

	GPIO5->IMR |= (1<<1U);//ENABLE
}
void EXAMPLE_GPIO_IRQHandler(void)
{
	GPIO5->ISR = 1U << 1;
	if(button_status >0)
	{
		button_status = 0;
	}
	else
	{
		button_status = 1;
	}

}



