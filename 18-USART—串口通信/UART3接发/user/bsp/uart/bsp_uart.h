#ifndef __USART_H
#define __USART_H

#include "fsl_common.h"

/*********************************************************
 * UART3 GPIO端口、引脚号及IOMUXC复用宏定义
 
使用i.MX RT1052-Pro底板：
 *  TX-----GPIO_AD_B1_06,J12-----(CN5, 26)
 *  RX-----GPIO_AD_B1_07,K10-----(CN5, 28)
 *引脚功能         引脚标号     引脚在开发板上对应位置
 *TX引脚对应开发板CN5排针的第26脚(CN5, 26)
 *RX引脚对应开发板CN5排针的第28脚(CN5, 28)

使用i.MX RT1052-Mini底板
 *  TX-----GPIO_AD_B1_06,J12-----(CN4, 26)
 *  RX-----GPIO_AD_B1_07,K10-----(CN4, 28)
 *引脚功能         引脚标号     引脚在开发板上对应位置
 *TX引脚对应开发板CN4排针的第26脚(CN4, 26)
 *RX引脚对应开发板CN4排针的第28脚(CN4, 28) 
 *********************************************************/
#define UART_RX_GPIO                GPIO1
#define UART_RX_GPIO_PIN            (23U)
#define UART_RX_IOMUXC              IOMUXC_GPIO_AD_B1_07_LPUART3_RX

#define UART_TX_GPIO                GPIO1
#define UART_TX_GPIO_PIN            (22U)
#define UART_TX_IOMUXC              IOMUXC_GPIO_AD_B1_06_LPUART3_TX

/*******************************************************************************
 * UART3 串口号、中断服务函数、中断号重定义
 ******************************************************************************/

#define DEBUG_USARTx LPUART3
#define DEBUG_USART_IRQ LPUART3_IRQn
#define DEBUG_USART_IRQHandler LPUART3_IRQHandler


/*******************************************************************************
* UART3 串口配置参数定义，默认参数如下:
 * .baudRate_Bps = 115200U;
 * .parityMode = kLPUART_ParityDisabled;
 * .stopBitCount = kLPUART_OneStopBit;
 * .txFifoWatermark = 0;
 * .rxFifoWatermark = 0;
 * .enableTx = false;
 * .enableRx = false;
 ******************************************************************************/
#define  DEBUG_USART_BAUDRATE           115200U
#define  DEBUG_USART_STOP_BIT           kLPUART_OneStopBit
#define  DEBUG_USART_ENABLE_SEND        true
#define  DEBUG_USART_ENABLE_RESIVE      true

/*******************************************************************************
 * uart3引脚配置
 ******************************************************************************/
#define USART_RX_PAD_CONFIG_DATA            (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_1_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_1_PULL_SELECTED| \
                                        PUS_3_22K_OHM_PULL_UP| \
                                        HYS_0_HYSTERESIS_DISABLED) 
    /* 配置说明 : */
    /* 转换速率: 转换速率慢
        驱动强度: R0/6 
        带宽配置 : medium(100MHz)
        开漏配置: 关闭 
        拉/保持器配置: 使能
        拉/保持器选择: 上下拉
        上拉/下拉选择: 22K欧姆上拉(选择了保持器此配置无效)
        滞回器配置: 禁止 */ 

#define USART_TX_PAD_CONFIG_DATA            (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_1_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_3_22K_OHM_PULL_UP| \
                                        HYS_0_HYSTERESIS_DISABLED)
    /* 配置说明 : */
    /* 转换速率: 转换速率慢
        驱动强度: R0/6 
        带宽配置 : medium(100MHz)
        开漏配置: 关闭 
        拉/保持器配置: 使能
        拉/保持器选择: 保持器
        上拉/下拉选择: 22K欧姆上拉(选择了保持器此配置无效)
        滞回器配置: 禁止 */ 

/*******************************************************************************
 * 函数声明
 ******************************************************************************/
 void USART_Config(void);
 void USART_ModeConfig(void);
 void USART_IOMUXC_MUX_Config(void);
 void USART_IOMUXC_PAD_Config(void);
 
 void Usart_SendByte(LPUART_Type *base, uint8_t data);
 void Usart_SendString( LPUART_Type *base, const char *str);
 void Usart_SendHalfWord(LPUART_Type *base, uint16_t ch);




#endif /* __USART_H */


