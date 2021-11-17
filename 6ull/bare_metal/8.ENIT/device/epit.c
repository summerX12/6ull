#include "MCIMX6Y2.h"
#include "fsl_iomuxc.h"
#include "system_MCIMX6Y2.h"

#include "epit.h"

uint8_t epit_flag = 0;

void epit_init(uint16_t frac,uint32_t value)
{
	if(frac > 0xfff)
	{
		frac = 0xfff;
	}
	EPIT1->CR = 0;

	EPIT1->CR = (1<<24 | frac<<4 | 1<<3 | 1<<2 |1<<1);
	EPIT1->LR = value;
	EPIT1->CMPR = 0;

	SystemInstallIrqHandler(EPIT1_IRQn,(system_irq_handler_t)EPIT_IRQHandler,NULL);
	GIC_EnableIRQ(EPIT1_IRQn);

	EPIT1->CR |= 0x01;
}

void EPIT_IRQHandler(void)
{
	if(EPIT1->SR & (0X01))
	{
		epit_flag = 1;
	}

	EPIT1->SR |= 0X01;
}

