#ifndef _EPIT_H
#define _EPIT_H

#include "MCIMX6Y2.h"
#include "fsl_iomuxc.h"

extern uint8_t epit_flag;

void epit_init(uint16_t frac,uint32_t value);
void EPIT_IRQHandler(void);
void epit_start(void);
void epti_stop(void);

#endif
