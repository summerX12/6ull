
#include "MCIMX6Y2.h"
#include "fsl_iomuxc.h"

/* SRE 压摆率选择 */
#define SRE_0_SLOW_SLEW_RATE                IOMUXC_SW_PAD_CTL_PAD_SRE(0)
#define SRE_1_FAST_SLEW_RATE                IOMUXC_SW_PAD_CTL_PAD_SRE(1)

/* 驱动能力配置，配置阻值的大小 */
#define DSE_0_OUTPUT_DRIVER_DISABLED        IOMUXC_SW_PAD_CTL_PAD_DSE(0)
/* R0 260 Ohm @ 3.3V, 150Ohm@1.8V, 240 Ohm for DDR */
#define DSE_1_R0_1                          IOMUXC_SW_PAD_CTL_PAD_DSE(1) 
/* R0/2 */
#define DSE_2_R0_2                          IOMUXC_SW_PAD_CTL_PAD_DSE(2)
/* R0/3 */
#define DSE_3_R0_3                          IOMUXC_SW_PAD_CTL_PAD_DSE(3)
/* R0/4 */
#define DSE_4_R0_4                          IOMUXC_SW_PAD_CTL_PAD_DSE(4)
/* R0/5 */
#define DSE_5_R0_5                          IOMUXC_SW_PAD_CTL_PAD_DSE(5)
/* R0/6 */
#define DSE_6_R0_6                          IOMUXC_SW_PAD_CTL_PAD_DSE(6)
/* R0/7 */
#define DSE_7_R0_7                          IOMUXC_SW_PAD_CTL_PAD_DSE(7)

/* SPEED 带宽配置 */
#define SPEED_0_LOW_50MHz                   IOMUXC_SW_PAD_CTL_PAD_SPEED(0)
#define SPEED_1_MEDIUM_100MHz               IOMUXC_SW_PAD_CTL_PAD_SPEED(1)
#define SPEED_2_MEDIUM_100MHz               IOMUXC_SW_PAD_CTL_PAD_SPEED(2)
#define SPEED_3_MAX_200MHz                  IOMUXC_SW_PAD_CTL_PAD_SPEED(3)

/* ODE 是否使用开漏模式 */
#define ODE_0_OPEN_DRAIN_DISABLED           IOMUXC_SW_PAD_CTL_PAD_ODE(0)     
#define ODE_1_OPEN_DRAIN_ENABLED            IOMUXC_SW_PAD_CTL_PAD_ODE(1)     

/* PKE 是否使能保持器或上下拉功能 */
#define PKE_0_PULL_KEEPER_DISABLED          IOMUXC_SW_PAD_CTL_PAD_PKE(0)      
#define PKE_1_PULL_KEEPER_ENABLED           IOMUXC_SW_PAD_CTL_PAD_PKE(1)      

/* PUE 选择使用保持器还是上下拉 */
#define PUE_0_KEEPER_SELECTED               IOMUXC_SW_PAD_CTL_PAD_PUE(0)   
#define PUE_1_PULL_SELECTED                 IOMUXC_SW_PAD_CTL_PAD_PUE(1)   

/* PUS 上下拉配置 */
#define PUS_0_100K_OHM_PULL_DOWN            IOMUXC_SW_PAD_CTL_PAD_PUS(0)     
#define PUS_1_47K_OHM_PULL_UP               IOMUXC_SW_PAD_CTL_PAD_PUS(1)   
#define PUS_2_100K_OHM_PULL_UP              IOMUXC_SW_PAD_CTL_PAD_PUS(2)   
#define PUS_3_22K_OHM_PULL_UP               IOMUXC_SW_PAD_CTL_PAD_PUS(3)   

/* HYS 滞后功能 */
#define HYS_0_HYSTERESIS_DISABLED           IOMUXC_SW_PAD_CTL_PAD_HYS(0)  
#define HYS_1_HYSTERESIS_ENABLED            IOMUXC_SW_PAD_CTL_PAD_HYS(1) 


#define LED_PAD_CONFIG_DATA            (SRE_0_SLOW_SLEW_RATE| \
                                          DSE_6_R0_6| \
                                          SPEED_2_MEDIUM_100MHz| \
                                          ODE_0_OPEN_DRAIN_DISABLED| \
                                          PKE_0_PULL_KEEPER_DISABLED| \
                                          PUE_0_KEEPER_SELECTED| \
                                          PUS_0_100K_OHM_PULL_DOWN| \
                                          HYS_0_HYSTERESIS_DISABLED)
void delay(uint32_t count)
{
    volatile uint32_t i = 0;
    for (i = 0; i < count; ++i)
    {
        __asm("NOP"); /* 调用nop空指令 */
    }
}

int main()
{
	CCM_CCGR1_CG13(0x3);
	CCM_CCGR3_CG6(0x3);

	IOMUXC_SetPinMux(IOMUXC_GPIO1_IO04_GPIO1_IO04,0);
	IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO04_GPIO1_IO04,0x1f838);//LED_PAD_CONFIG_DATA);

	IOMUXC_SetPinMux(IOMUXC_CSI_HSYNC_GPIO4_IO20,0);
	IOMUXC_SetPinConfig(IOMUXC_CSI_HSYNC_GPIO4_IO20,LED_PAD_CONFIG_DATA);

	IOMUXC_SetPinMux(IOMUXC_CSI_VSYNC_GPIO4_IO19,0);
	IOMUXC_SetPinConfig(IOMUXC_CSI_VSYNC_GPIO4_IO19,LED_PAD_CONFIG_DATA);

	GPIO1->GDIR |= (1<<4);  //设置GPIO1_04为输出模式
      GPIO1->DR |= (1<<4);    //设置GPIO1_04输出电平为高电平

      GPIO4->GDIR |= (1<<20);  //设置GPIO4_20为输出模式
      GPIO4->DR |= (1<<20);    //设置GPIO4_20输出电平为高电平

      GPIO4->GDIR |= (1<<19);  //设置GPIO4_19为输出模式
      GPIO4->DR |= (1<<19);    //设置GPIO4_19输出电平为高电平

	while(1)
	{
	       GPIO1->DR &= ~(1<<4); 
           delay(0xFFFFF);
           GPIO1->DR |= (1<<4); 

           GPIO4->DR &= ~(1<<20); 
           delay(0xFFFFF);
           GPIO4->DR |= (1<<20);

           GPIO4->DR &= ~(1<<19);
           delay(0xFFFFF);
           GPIO4->DR |= (1<<19);
	}
}


