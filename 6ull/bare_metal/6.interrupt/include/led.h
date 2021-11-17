#ifndef led_h
#define led_h

#include "MCIMX6Y2.h"
#include "fsl_iomuxc.h"
#include "pad_config.h"

#define led_on	GPIO1->DR &= ~(1<<4)
#define led_off	GPIO1->DR |= (1<<4)

void led_init(void);


#endif
