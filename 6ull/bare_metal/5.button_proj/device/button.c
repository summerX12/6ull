#include "MCIMX6Y2.h"
#include "fsl_iomuxc.h"
#include "pad_config.h"
#include "MCIMX6Y2.h"
#include "fsl_iomuxc.h"
#include "pad_config.h"
#include "button.h"

void delay(uint32_t count)
{
	uint32_t i;
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

