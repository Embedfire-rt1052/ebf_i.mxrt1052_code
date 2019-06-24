#ifndef __CAN_H
#define	__CAN_H

#include "fsl_iomuxc.h"
#include "pad_config.h"
#include "fsl_gpio.h" 
#include "fsl_common.h"
#include "fsl_flexcan.h"


/*********************************************************
 * FLEXCAN2 GPIO端口、引脚号及IOMUXC复用宏定义
 
使用i.MX RT1052-Pro底板：
 *  TX-----GPIO_AD_B0_14-----(CN5,17)
 *  RX-----GPIO_AD_B0_15-----(CN5, 19)
 *引脚功能         引脚标号     引脚在开发板上对应位置
 *TX引脚对应开发板CN5排针的第17脚(CN5, 17)
 *RX引脚对应开发板CN5排针的第19脚(CN5, 19)
 
使用i.MX RT1052-Mini底板：
 *  TX-----GPIO_AD_B0_14-----(CN4, 17)
 *  RX-----GPIO_AD_B0_15-----(CN4, 19)
 *引脚功能         引脚标号     引脚在开发板上对应位置
 *TX引脚对应开发板CN4排针的第17脚(CN4, 17)
 *RX引脚对应开发板CN4排针的第19脚(CN4, 19)
 *********************************************************/
#define FLEXCAN2_TX_GPIO                GPIO1
#define FLEXCAN2_TX_GPIO_PIN            (14U)
#define FLEXCAN2_TX_IOMUXC              IOMUXC_GPIO_AD_B0_14_FLEXCAN2_TX

#define FLEXCAN2_RX_GPIO                GPIO1
#define FLEXCAN2_RX_GPIO_PIN            (15U)
#define FLEXCAN2_RX_IOMUXC              IOMUXC_GPIO_AD_B0_15_FLEXCAN2_RX




/*CAN相关宏定义*/
#define EXAMPLE_CAN CAN2                 //定义使用的CAN
#define EXAMPLE_FLEXCAN_IRQn CAN2_IRQn   //定义中断号
#define EXAMPLE_FLEXCAN_IRQHandler CAN2_IRQHandler//定义中断服务函数


/*发送邮箱相关定义*/
#define RX_MESSAGE_BUFFER_NUM (9)    //定义使用的接收邮箱
#define TX_MESSAGE_BUFFER_NUM (8)    //定义使用的发送邮箱

#define DLC (8)                      //定义数据长度


/*时钟相关宏定义*/
#define FLEXCAN_CLOCK_SOURCE_SELECT (2U) //选择时钟源，PLL3(480 MHz)经过6分频后(80MHz)作为CAN根时钟。
#define FLEXCAN_CLOCK_SOURCE_DIVIDER (3U)//设置时钟分频，80MHz的CAN根时钟经过分频后作为CAN时钟源。
/* 读取CAN是工作频率 */
#define EXAMPLE_CAN_CLK_FREQ ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 6) / (FLEXCAN_CLOCK_SOURCE_DIVIDER + 1U))





/*******************************************************************************
 * FLEXCAN2引脚配置
 ******************************************************************************/
#define FLEXCAN2_PAD_CONFIG_DATA        (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_1_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_1_PULL_SELECTED| \
                                        PUS_3_22K_OHM_PULL_UP| \
                                        HYS_0_HYSTERESIS_DISABLED) 



void CAN_Mode_Config(uint32_t baudRate, bool LoopBack);
void CAN_Config(void);
void CAN_RX_Buffer_Config(uint32_t ID_STD,uint8_t RX_MB);


#endif







