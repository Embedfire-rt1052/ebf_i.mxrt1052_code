#ifndef __UART_H
#define	__UART_H

#include "fsl_common.h"


/*********************************************************
 * UART2 GPIO端口、引脚号及IOMUXC复用宏定义
 
使用i.MX RT1052-Pro底板：
 *  TX-----GPIO_AD_B1_02,L11-----(CN5, 10)
 *  RX-----GPIO_AD_B1_03,M12-----(CN5, 8)
 *引脚功能         引脚标号     引脚在开发板上对应位置
 *TX引脚对应开发板CN5排针的第10脚(CN5, 10)
 *RX引脚对应开发板CN5排针的第8脚(CN5, 8)
 
使用i.MX RT1052-Mini底板：
 *  TX-----GPIO_AD_B1_02,L11-----(CN4, 10)
 *  RX-----GPIO_AD_B1_03,M12-----(CN4, 8)
 *引脚功能         引脚标号     引脚在开发板上对应位置
 *TX引脚对应开发板CN4排针的第10脚(CN4, 10)
 *RX引脚对应开发板CN4排针的第8脚(CN4, 8)
 *********************************************************/
#define UART_RX_GPIO                GPIO1
#define UART_RX_GPIO_PIN            (19U)
#define UART_RX_IOMUXC              IOMUXC_GPIO_AD_B1_03_LPUART2_RX

#define UART_TX_GPIO                GPIO1
#define UART_TX_GPIO_PIN            (18U)
#define UART_TX_IOMUXC              IOMUXC_GPIO_AD_B1_02_LPUART2_TX

/*******************************************************************************
 * UART2 串口号、中断服务函数、中断号重定义
 ******************************************************************************/

#define DEBUG_UARTx LPUART2
#define DEBUG_UART_IRQ LPUART2_IRQn
#define DEBUG_UART_IRQHandler LPUART2_IRQHandler


/*******************************************************************************
* UART2 串口配置参数定义，默认参数如下:
 * .baudRate_Bps = 115200U;
 * .parityMode = kLPUART_ParityDisabled;
 * .stopBitCount = kLPUART_OneStopBit;
 * .txFifoWatermark = 0;
 * .rxFifoWatermark = 0;
 * .enableTx = false;
 * .enableRx = false;
 ******************************************************************************/
#define  DEBUG_UART_BAUDRATE           115200U
#define  DEBUG_UART_STOP_BIT           kLPUART_OneStopBit
#define  DEBUG_UART_ENABLE_SEND        true
#define  DEBUG_UART_ENABLE_RESIVE      true

/*******************************************************************************
 * uart2引脚配置
 ******************************************************************************/
#define UART_RX_PAD_CONFIG_DATA            (SRE_0_SLOW_SLEW_RATE| \
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

#define UART_TX_PAD_CONFIG_DATA            (SRE_0_SLOW_SLEW_RATE| \
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
 void UART_Config(void);
 void UART_ModeConfig(void);
 void UART_IOMUXC_MUX_Config(void);
 void UART_IOMUXC_PAD_Config(void);
 
 void Uart_SendByte(LPUART_Type *base, uint8_t data);
 void Uart_SendString( LPUART_Type *base,  const char *str);
 void Uart_SendHalfWord(LPUART_Type *base, uint16_t ch);



#endif /* __UART_H */


