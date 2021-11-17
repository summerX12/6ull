#include "MCIMX6Y2.h"
#include "fsl_iomuxc.h"
#include "pad_config.h"
#include "system_MCIMX6Y2.h"

#include "epit.h"
#include "led.h"


void main()
{
	uint8_t i=0;
	
	led_init();
	epit_init(0,66000000/2);
//	button_interrupt_init();

	while(1)
	{
		if( epit_flag)//BUTTON PRESS
		{
			epit_flag = 0;
			i++;
		}

		switch(i)
		{
			case 0:led_off;break;
			case 1:led_on;break;
			case 2:led_off;i=0;break;
			default:break;
		}
	}
}

