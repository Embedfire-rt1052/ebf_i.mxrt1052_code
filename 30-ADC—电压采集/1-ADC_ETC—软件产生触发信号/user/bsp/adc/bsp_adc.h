#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#include "fsl_common.h"
#include "fsl_adc.h"
#include "fsl_adc_etc.h"

/*********************************************************
 * ADC GPIO端口、引脚号及IOMUXC复用宏定义
 
 使用i.MX RT1052-Pro底板：
 *  ADC1 CH0-----GPIO_AD_B1_11,J13-----(CN5, 29)
 *  CH15 CH15----GPIO_AD_B1_10,l13-----(CN5, 31)
 *引脚功能         引脚标号        引脚在开发板上对应位置
 * CH0 引脚对应开发板CN5排针的第29脚(CN5, 29)
 * CH15引脚对应开发板CN5排针的第31脚(CN5, 31)

使用i.MX RT1052-Mini底板
 *  ADC1 CH0-----GPIO_AD_B1_11,J13-----(CN4, 29)
 *  CH15 CH15----GPIO_AD_B1_10,l13-----(CN4, 31)
 *引脚功能         引脚标号        引脚在开发板上对应位置
 * CH0 引脚对应开发板CN4排针的第29脚(CN4, 29)
 * CH15引脚对应开发板CN4排针的第31脚(CN4, 31) 

 *********************************************************/
 //GPIO_AD_B1_11
#define CORE_BOARD_ADC_GPIO_CH0             GPIO1
#define CORE_BOARD_ADC_GPIO_PIN_CH0         (27U)
#define CORE_BOARD_ADC_IOMUXC_CH0          IOMUXC_GPIO_AD_B1_11_GPIO1_IO27

// //GPIO_AD_B1_10
//#define CORE_BOARD_ADC_GPIO_CH15             GPIO1
//#define CORE_BOARD_ADC_GPIO_PIN_CH15         (26U)
//#define CORE_BOARD_ADC_IOMUXC_CH15           IOMUXC_GPIO_AD_B1_10_GPIO1_IO26
 


// 可以是 ADC1/2，
#define    ADCx                          ADC1




/**
*设置ADC_ETC转换组
*每个ADC_ETC有7个转换通道组(0到7)，每个ADC_ETC转换通道组最多可添加7个ADC转换通道。
*当一个ADC_ETC转换通道组收到触发信号时开始转换该通道组指定的ADC转换通道.
*/
#define ADC_ETC_CHANNEL_GROUPx 3


/**
*定义ADC转换通道组
*每个ADC有8个这样的转换通道（0到7），每个转换通道只能指定一个输入通道（一个ADC有16个输入通道）
*/
#define DEMO_ADC_CHANNEL_GROUP0 4U
#define DEMO_ADC_CHANNEL_GROUP1 5U


/**
*定义ADC输入通道，每个ADC有16个输入通道（0到15）对应到芯片的不同引脚。
*/
#define DEMO_ADC_ETC_CHANNEL0 15U
#define DEMO_ADC_ETC_CHANNEL1 0U


#define DEMO_ADC_ETC_BASE ADC_ETC
#define DEMO_ADC_ETC_CHAIN_LENGTH 0U /* Chain length is 1. */



#define EXAMPLE_ADC_ETC_DONE0_Handler ADC_ETC_IRQ0_IRQHandler
#define EXAMPLE_ADC_ETC_DONE1_Handler ADC_ETC_IRQ1_IRQHandler
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
