#ifndef __UART_H
#define __UART_H

#include "fsl_common.h"

/*********************************************************
 * UART5 GPIO端口以及485接收、发送控制端口、引脚号及IOMUXC复用宏定义
 
使用i.MX RT1052-Pro底板：
 *  TX-----GPIO_B1_12,D13-----(CN5, 13)
 *  RX-----GPIO_B1_13,D14-----(CN5, 15)
 *  RE-----GPIO_AD_B1_10,L13--(CN5, 31)
 *引脚功能         引脚标号     引脚在开发板上对应位置
 *TX引脚对应开发板CN5排针的第13脚(CN5, 13)
 *RX引脚对应开发板CN5排针的第15脚(CN5, 15)
 
使用i.MX RT1052-Mini底板：
 *  TX-----GPIO_B1_12,D13-----(CN4, 13)
 *  RX-----GPIO_B1_13,D14-----(CN4, 15)
 *  RE-----GPIO_AD_B1_10,L13--(CN4, 31)
 *引脚功能         引脚标号     引脚在开发板上对应位置
 *TX引脚对应开发板CN4排针的第13脚(CN4, 13)
 *RX引脚对应开发板CN4排针的第15脚(CN4, 15)
 *********************************************************/

/*485 发送、接收控制引脚定义
*为高电平，接收使能
*为低电平，发送使能
*/
#define _485_RE_GPIO             GPIO1
#define _485_RE_GPIO_PIN         (26U)
#define _485_RE_IOMUXC           IOMUXC_GPIO_AD_B1_10_GPIO1_IO26

#define digital_H(p,i)      {p->DR |= (1U << i);}   //输出为高电平    
#define digital_L(p,i)      {p->DR &= ~(1U << i);}  //输出低电平

/*设置485为接收或发送*/
#define _485_Resive           digital_L(_485_RE_GPIO,_485_RE_GPIO_PIN)
#define _485_Send             digital_H(_485_RE_GPIO,_485_RE_GPIO_PIN) 


/*TX引脚*/
#define UART_RX_GPIO                GPIO2
#define UART_RX_GPIO_PIN            (29U)
#define UART_RX_IOMUXC              IOMUXC_GPIO_B1_13_LPUART5_RX

/*RX 引脚*/
#define UART_TX_GPIO                GPIO2
#define UART_TX_GPIO_PIN            (28U)
#define UART_TX_IOMUXC              IOMUXC_GPIO_B1_12_LPUART5_TX





/*******************************************************************************
 * UART5 串口号、中断服务函数、中断号重定义
 ******************************************************************************/

#define DEBUG_UARTx LPUART5
#define DEBUG_UART_IRQ LPUART5_IRQn
#define DEBUG_UART_IRQHandler LPUART5_IRQHandler


/*******************************************************************************
* UART5 串口配置参数定义，默认参数如下:
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
 * uart5引脚配置
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


/* 485芯片接收、发送控制引脚*/
#define RE_485_PAD_CONFIG_DATA          (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_0_PULL_KEEPER_DISABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_0_100K_OHM_PULL_DOWN| \
                                        HYS_0_HYSTERESIS_DISABLED)  

/*******************************************************************************
 * 函数声明
 ******************************************************************************/
 void UART_Config(void);
 void UART_ModeConfig(void);
 void UART_IOMUXC_MUX_Config(void);
 void UART_IOMUXC_PAD_Config(void);
 
 void Uart_SendByte(LPUART_Type *base, uint8_t data);
 void Uart_SendString( LPUART_Type *base, const char *str);
 void Uart_SendHalfWord(LPUART_Type *base, uint16_t ch);

 /*485通信新增函数*/
 static void _485_delay(__IO uint32_t nCount);
 void _485_SendByte(LPUART_Type *base, uint8_t data);
 void _485_Control_GPIO_init(void);


#endif /* __UART_H */


 
 