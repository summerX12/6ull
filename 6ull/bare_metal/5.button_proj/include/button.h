#ifndef button_h
#define button_h

#include "MCIMX6Y2.h"
#include "fsl_iomuxc.h"
#include "pad_config.h"

void button_init(void);
int get_button_status(void);

#endif
