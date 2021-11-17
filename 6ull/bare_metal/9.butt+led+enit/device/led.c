#include "MCIMX6Y2.h"
#include "fsl_iomuxc.h"
#include "pad_config.h"
#include "led.h"

void led_init(void)
{
//led
	CCM_CCGR1_CG13(0X3);

	IOMUXC_SetPinMux(IOMUXC_GPIO1_IO04_GPIO1_IO04,0);
	IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO04_GPIO1_IO04,0x1f838);

	GPIO1->GDIR |= (1<<4);
	GPIO1->DR |= (1<<4);
}

