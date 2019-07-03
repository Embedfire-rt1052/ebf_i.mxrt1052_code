#ifndef __BSP_DMA_UART_H
#define __BSP_DMA_UART_H

#include "fsl_dmamux.h"
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "pad_config.h"
#include "fsl_lpuart.h"
#include "board.h"
#include "fsl_lpuart_edma.h"



/*********************************************************
 * UART1 GPIO端口、引脚号及IOMUXC复用宏定义
 
使用i.MX RT1052-Pro底板：
 *  TX-----GPIO_AD_B0_12,K14-----(CN5, 9)
 *  RX-----GPIO_AD_B0_13,L14-----(CN5, 11)
 *引脚功能         引脚标号     引脚在开发板上对应位置
 *TX引脚对应开发板CN5排针的第9脚(CN5, 9)
 *RX引脚对应开发板CN5排针的第11脚(CN5, 11)
 
使用i.MX RT1052-Mini底板：
 *  TX-----GPIO_AD_B0_12,K14-----(CN4, 9)
 *  RX-----GPIO_AD_B0_13,L14-----(CN4, 11)
 *引脚功能         引脚标号     引脚在开发板上对应位置
 *TX引脚对应开发板CN4排针的第9脚(CN4, 9)
 *RX引脚对应开发板CN4排针的第11脚(CN4, 11)
 *********************************************************/
#define UART_RX_GPIO                GPIO1
#define UART_RX_GPIO_PIN            (13U)
#define UART_RX_IOMUXC              IOMUXC_GPIO_AD_B0_13_LPUART1_RX

#define UART_TX_GPIO                GPIO1
#define UART_TX_GPIO_PIN            (12U)
#define UART_TX_IOMUXC              IOMUXC_GPIO_AD_B0_12_LPUART1_TX


/*******************************************************************************
 * uart引脚配置
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
   
   
   

/*宏定义*/
/*定义本程序使用的串口，
*注意:LPUART1是系统调试串口，在系统初始化时已经完成了相关GPIO的初始化，如果使用其他
*     串口，使用之前一定要初始化对应引脚。
*/
#define DEMO_LPUART LPUART1               
#define DEMO_LPUART_CLK_FREQ BOARD_DebugConsoleSrcFreq() //UART时钟频率
#define LPUART_TX_DMA_CHANNEL 0U                         //UART发送使用的DMA通道号
#define LPUART_RX_DMA_CHANNEL 1U                         //UART接收使用的DMA通道号
#define LPUART_TX_DMA_REQUEST kDmaRequestMuxLPUART1Tx    //定义串口DMA发送请求源
#define LPUART_RX_DMA_REQUEST kDmaRequestMuxLPUART1Rx    //定义串口DMA接收请求源
#define EXAMPLE_LPUART_DMAMUX_BASEADDR DMAMUX            //定义所使用的DMA多路复用模块(DMAMUX)
#define EXAMPLE_LPUART_DMA_BASEADDR DMA0                 //定义使用的DAM
#define ECHO_BUFFER_LENGTH 8                             //UART接收和发送数据缓冲区长度


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void UART_GPIO_Init(void);
void UART_Init(void);
void UART_DMA_Init(void);
/* LPUART 回调函数 */
void LPUART_UserCallback(LPUART_Type *base, lpuart_edma_handle_t *handle, status_t status, void *userData);

#endif /* __BSP_DMA_UART_H */
