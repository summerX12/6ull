#ifndef button_h
#define button_h

#include "MCIMX6Y2.h"
#include "fsl_iomuxc.h"
#include "pad_config.h"

extern uint8_t button_status;

void button_init(void);
int get_button_status(void);

void button_interrupt_init(void);
void EXAMPLE_GPIO_IRQHandler(void);

#endif
