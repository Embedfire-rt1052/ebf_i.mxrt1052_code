#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#include "fsl_common.h"
#include "fsl_adc.h"

/*********************************************************
 * ADC GPIO端口、引脚号及IOMUXC复用宏定义
使用i.MX RT1052-Pro底板：
 *  ADC1_IN0-----GPIO_AD_B1_11-----(CN5,29)
 *  引脚功能       引脚标号        引脚在开发板上对应位置
 (CN5, 68) 含义是引脚连接到开发板CN5排针的第68脚

使用i.MX RT1052-Mini底板    
 *  ADC1_IN0-----GPIO_AD_B1_11-----(CN4,29)
 *引脚功能       引脚标号        引脚在开发板上对应位置
 (CN4,29) 含义是引脚连接到开发板CN4排针的第29脚 
 *********************************************************/
#define CORE_BOARD_ADC_GPIO             GPIO1
#define CORE_BOARD_ADC_GPIO_PIN         (27U)
#define CORE_BOARD_ADC_IOMUXC           IOMUXC_GPIO_AD_B1_11_GPIO1_IO27

/*定义使用的ADC*/ 
#define    ADCx              ADC1
#define DEMO_ADC_CHANNEL_GROUP 0U //ADC 通道组，最多只能使用8个通道组（0到7）
#define DEMO_ADC_USER_CHANNEL 0U  //ADC 通道宏定义

/*中断相关宏定义*/ 
#define    ADC_IRQ                       ADC1_IRQn
#define    ADC_IRQHandler                ADC1_IRQHandler

/*********************************************************
 * ADC引脚PAD配置
 ****************************************************************/
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



















