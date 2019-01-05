#ifndef __BSP_GPT_H
#define __BSP_GPT_H

#include "fsl_iomuxc.h"
#include "fsl_common.h"
#include "fsl_gpt.h"


/* 选择 IPG Clock as PERCLK_CLK clock source */
#define EXAMPLE_GPT_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for PERCLK_CLK clock source */
#define EXAMPLE_GPT_CLOCK_DIVIDER_SELECT (0U)


/* Get source clock for GPT driver (GPT prescaler = 0) */
#define EXAMPLE_GPT_CLK_FREQ (CLOCK_GetFreq(kCLOCK_IpgClk) / (EXAMPLE_GPT_CLOCK_DIVIDER_SELECT + 1U))

#define EXAMPLE_GPT GPT2

#define GPT_IRQ_ID GPT2_IRQn
#define EXAMPLE_GPT_IRQHandler GPT2_IRQHandler



/*********************************************************
 * GPT GPIO端口、引脚号及IOMUXC复用宏定义
 *********************************************************/
 /*与SEMC_D13共用*/
#define GPT1_COMPARE1_GPIO             GPIO3 
#define GPT1_COMPARE1_GPIO_PIN         (21U)
#define GPT1_COMPARE1_IOMUXC           IOMUXC_GPIO_EMC_35_GPT1_COMPARE1
 /*与SEMC_D14共用*/
#define GPT1_COMPARE2_GPIO             GPIO3
#define GPT1_COMPARE2_GPIO_PIN         (22U)
#define GPT1_COMPARE2_IOMUXC           IOMUXC_GPIO_EMC_36_GPT1_COMPARE2
 /*与SEMC_D15共用*/
#define GPT1_COMPARE3_GPIO             GPIO3
#define GPT1_COMPARE3_GPIO_PIN         (23U)
#define GPT1_COMPARE3_IOMUXC           IOMUXC_GPIO_EMC_37_GPT1_COMPARE3

/********************************************************************************/

/*与JTAG_TMS 共用*/
#define GPT2_COMPARE1_GPIO             GPIO1
#define GPT2_COMPARE1_GPIO_PIN         (6U)
#define GPT2_COMPARE1_IOMUXC           IOMUXC_GPIO_AD_B0_06_GPT2_COMPARE1

/*与JTAG_TCK 共用*/
#define GPT2_COMPARE2_GPIO             GPIO1
#define GPT2_COMPARE2_GPIO_PIN         (7U)
#define GPT2_COMPARE2_IOMUXC           IOMUXC_GPIO_AD_B0_07_GPT2_COMPARE2

/*与JTAG_MOD 共用 ，外接有10K下拉电阻*/
#define GPT2_COMPARE3_GPIO             GPIO1
#define GPT2_COMPARE3_GPIO_PIN         (8U)
#define GPT2_COMPARE3_IOMUXC           IOMUXC_GPIO_AD_B0_08_GPT2_COMPARE3
/*********************************************************************************/


/*******************************************************************************
 * 宏
 ******************************************************************************/
/* GPT_COMPARE引脚使用的PAD配置 */
#define GPT_COMPARE_PAD_CONFIG_DATA    (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_0_PULL_KEEPER_DISABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_0_100K_OHM_PULL_DOWN| \
                                        HYS_0_HYSTERESIS_DISABLED)   
    /* 配置说明 : */
    /* 转换速率: 转换速率慢
      驱动强度: R0/6 
      带宽配置 : medium(100MHz)
      开漏配置: 关闭 
      拉/保持器配置: 关闭
      拉/保持器选择: 保持器（上面已关闭，配置无效）
      上拉/下拉选择: 100K欧姆下拉（上面已关闭，配置无效）
      滞回器配置: 关闭 */ 


void GPT_GPIO_Config(void);
void GPT_Config(void);


#endif /* __BSP_GPT_H */


