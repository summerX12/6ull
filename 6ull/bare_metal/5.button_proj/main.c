#include "MCIMX6Y2.h"
#include "fsl_iomuxc.h"
#include "pad_config.h"

#include "button.h"
#include "led.h"

void main()
{
	uint8_t i=0;
	
	led_init();
	button_init();

	while(1)
	{
		if( get_button_status() )//BUTTON PRESS
		{
			i++;
			while(get_button_status() );
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

