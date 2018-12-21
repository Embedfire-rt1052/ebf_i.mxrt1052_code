#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#include "fsl_common.h"
#include "fsl_adc.h"
#include "fsl_adc_etc.h"

/*********************************************************
 * ADC GPIO端口、引脚号及IOMUXC复用宏定义
 *********************************************************/
 //GPIO_AD_B1_11
#define CORE_BOARD_ADC_GPIO_CH0             GPIO1
#define CORE_BOARD_ADC_GPIO_PIN_CH0         (27U)
#define CORE_BOARD_ADC_IOMUXC_CH0          IOMUXC_GPIO_AD_B1_11_GPIO1_IO27

 //GPIO_AD_B1_10
#define CORE_BOARD_ADC_GPIO_CH15             GPIO1
#define CORE_BOARD_ADC_GPIO_PIN_CH15             (26U)
#define CORE_BOARD_ADC_IOMUXC_CH15           IOMUXC_GPIO_AD_B1_10_GPIO1_IO26
 

// ADC 编号选择
// 可以是 ADC1/2，中断要与ADC对应
#define    ADCx                          ADC1



// ADC 中断相关宏定义
#define    ADC_IRQ                       ADC1_IRQn
#define    ADC_IRQHandler                ADC1_IRQHandler
#define EXAMPLE_ADC_ETC_DONE0_Handler ADC_ETC_IRQ0_IRQHandler
#define EXAMPLE_ADC_ETC_DONE1_Handler ADC_ETC_IRQ1_IRQHandler

/*ADC 通道组，最多只能使用8个通道组（0到7）*/
#define DEMO_ADC_CHANNEL_GROUP 0U

/*ADC 通道宏定义*/
#define DEMO_ADC_USER_CHANNEL0 15U
#define DEMO_ADC_USER_CHANNEL1 0U


/*外部转换通道宏定义，取值范围 0~31 为什么这里取16 还不清楚*/
#define DEMO_ADC_USER_CHANNEL 16U

/*设置转换通道组，取值范围为0~8，一个转换通道组可以包含多个转换通道，但是这里只包含一个*/
#define DEMO_ADC_CHANNEL_GROUP0 0U
#define DEMO_ADC_CHANNEL_GROUP1 1U


#define DEMO_ADC_ETC_BASE ADC_ETC
#define DEMO_ADC_ETC_CHAIN_LENGTH 1U /* Chain length is 2. */

/*定义转换通道。*/
#define DEMO_ADC_ETC_CHANNEL0 15U
#define DEMO_ADC_ETC_CHANNEL1 0U
/*******************************************************************************
 * ADC引脚PAD配置
 ******************************************************************************/
#define ADC_PAD_CONFIG_DATA            (SRE_0_SLOW_SLEW_RATE| \
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

static void ADC_IOMUXC_MUX_Config(void);
static void ADC_IOMUXC_PAD_Config(void);
static void ADC_IO_Mode_Config(void);
static void ADC_Mode_Config(void);


void ADC_Config(void);

#endif /* __BSP_ADC_H */
