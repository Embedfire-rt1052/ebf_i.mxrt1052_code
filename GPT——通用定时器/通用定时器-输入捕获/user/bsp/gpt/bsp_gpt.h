#ifndef __BSP_GPT_H
#define __BSP_GPT_H

#include "fsl_iomuxc.h"
#include "fsl_common.h"
#include "fsl_gpt.h"




/* 选择 IPG Clock as PERCLK_CLK clock source */
#define EXAMPLE_GPT_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for PERCLK_CLK clock source */
#define EXAMPLE_GPT_CLOCK_DIVIDER_SELECT (0U)

/*GPT 时钟分频(1-4096)*/
#define GPT_DIVIDER 1

/* 得到GPT定时器的计数频率*/
#define EXAMPLE_GPT_CLK_FREQ ( (CLOCK_GetFreq(kCLOCK_IpgClk) / (EXAMPLE_GPT_CLOCK_DIVIDER_SELECT + 1U))/GPT_DIVIDER )

/*定义使用的GPT*/
#define EXAMPLE_GPT GPT2

/*中断号和中断服务函数宏定义*/
#define GPT_IRQ_ID GPT2_IRQn
#define EXAMPLE_GPT_IRQHandler GPT2_IRQHandler





/*********************************************************
 * GPT GPIO端口、引脚号及IOMUXC复用宏定义
 *********************************************************/
  /*与W9825G6KH-6的CAS脚冲突*/
#define GPT1_CAPTURE1_GPIO             GPIO4 
#define GPT1_CAPTURE1_GPIO_PIN         (24U)
#define GPT1_CAPTURE1_IOMUXC           IOMUXC_GPIO_EMC_24_GPT1_CAPTURE1
 /*与W9825G6KH-6的A10冲突*/
#define GPT1_CAPTURE2_GPIO             GPIO4
#define GPT1_CAPTURE2_GPIO_PIN         (23U)
#define GPT1_CAPTURE2_IOMUXC           IOMUXC_GPIO_EMC_23_GPT1_CAPTURE2
 
 
 
 
 /*NAND Flash 的CE脚冲突（CN4,27）*/
#define GPT2_CAPTURE1_GPIO             GPIO3 
#define GPT2_CAPTURE1_GPIO_PIN         (27U)
#define GPT2_CAPTURE1_IOMUXC           IOMUXC_GPIO_EMC_41_GPT2_CAPTURE1



/* sw3, (CN4,48)*/
 /*NAND Flash 的R/B脚共用，外接有10K上拉电阻（CN4,25）*/
#define GPT2_CAPTURE2_GPIO             GPIO3
#define GPT2_CAPTURE2_GPIO_PIN         (26U)
#define GPT2_CAPTURE2_IOMUXC           IOMUXC_GPIO_EMC_40_GPT2_CAPTURE2




// 定时器输入捕获用户自定义变量结构体声明
typedef struct
{   
	uint8_t   Capture_FinishFlag;   // 捕获结束标志位
	uint8_t   Capture_StartFlag;    // 捕获开始标志位
	uint32_t  Capture_CcrValue_1;     // 捕获寄存器的值
  uint32_t   Capture_CcrValue_2;     // 捕获寄存器的值
	uint16_t  Capture_Period;       // 定时器溢出次数 
}GPT_ICUserValueTypeDef;




/********************************************************************************/


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


