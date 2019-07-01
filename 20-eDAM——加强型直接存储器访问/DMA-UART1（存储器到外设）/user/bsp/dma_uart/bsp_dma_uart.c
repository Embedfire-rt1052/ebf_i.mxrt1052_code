#include "./bsp/dma_uart/bsp_dma_uart.h"
#include "fsl_lpuart_edma.h"

/*变量定义在main.c文件*/
extern volatile bool rxBufferEmpty;
extern volatile bool txBufferFull;
extern volatile bool txOnGoing;
extern volatile bool rxOnGoing;


/*句柄定义*/
extern lpuart_edma_handle_t g_lpuartEdmaHandle;
extern edma_handle_t g_lpuartTxEdmaHandle;
extern edma_handle_t g_lpuartRxEdmaHandle;


/*
*函数功能：初始化串口引脚
注意:本实验使用的是UART1,UART1是系统串口，在系统启动后已经完成了引脚的初始化
所以在这里可以屏蔽了UART1引脚初始化，如果使用其他串口请添加上引脚初始化代码是必须添加的。
*/
void UART_GPIO_Init(void)
{
  /* 定义gpio初始化配置结构体 */
  gpio_pin_config_t adc_config;
  /*ADC，GPIO配置*/   
  adc_config.direction = kGPIO_DigitalInput; //输入模式
//adc_config.outputLogic =  1;                //默认高电平，在输入模式下配置该选项无效
  adc_config.interruptMode = kGPIO_NoIntmode; //不使用中断
  
  
  IOMUXC_SetPinMux(UART_RX_IOMUXC, 0U);                                      
  IOMUXC_SetPinMux(UART_TX_IOMUXC, 0U);
  
  IOMUXC_SetPinConfig(UART_RX_IOMUXC, UART_RX_PAD_CONFIG_DATA);
  IOMUXC_SetPinConfig(UART_TX_IOMUXC, UART_TX_PAD_CONFIG_DATA);
  
  GPIO_PinInit(UART_RX_GPIO, UART_RX_GPIO_PIN, &adc_config);
  
  adc_config.direction = kGPIO_DigitalOutput; //输入模式
  adc_config.outputLogic =  1;                //默认高电平
  GPIO_PinInit(UART_TX_GPIO, UART_TX_GPIO_PIN, &adc_config);
  
}


/*
*函数功能：初始化串口
*/
void UART_Init(void)
{
  lpuart_config_t lpuartConfig;//定义LUART初始化结构体
  
  /*初始化UART引脚*/
  UART_GPIO_Init();
  /* LPUART.默认配置 */
  /*
  * lpuartConfig.baudRate_Bps = 115200U;
  * lpuartConfig.parityMode = kLPUART_ParityDisabled;
  * lpuartConfig.stopBitCount = kLPUART_OneStopBit;
  * lpuartConfig.txFifoWatermark = 0;
  * lpuartConfig.rxFifoWatermark = 0;
  * lpuartConfig.enableTx = false;
  * lpuartConfig.enableRx = false;
  */
  LPUART_GetDefaultConfig(&lpuartConfig);
  lpuartConfig.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
  lpuartConfig.enableTx = true;
  lpuartConfig.enableRx = true;
  
  LPUART_Init(DEMO_LPUART, &lpuartConfig, DEMO_LPUART_CLK_FREQ);
}


/*
*函数功能：初始化串口所使用的DMA
*/
void UART_DMA_Init(void)
{
  edma_config_t config;//定义eDMA初始化结构体
  
  /*初始化DMAMUX */
  DMAMUX_Init(EXAMPLE_LPUART_DMAMUX_BASEADDR);
  /* 为LPUART设置DMA传输通道*/
  DMAMUX_SetSource(EXAMPLE_LPUART_DMAMUX_BASEADDR, LPUART_TX_DMA_CHANNEL, LPUART_TX_DMA_REQUEST);
  DMAMUX_SetSource(EXAMPLE_LPUART_DMAMUX_BASEADDR, LPUART_RX_DMA_CHANNEL, LPUART_RX_DMA_REQUEST);
  DMAMUX_EnableChannel(EXAMPLE_LPUART_DMAMUX_BASEADDR, LPUART_TX_DMA_CHANNEL);
  DMAMUX_EnableChannel(EXAMPLE_LPUART_DMAMUX_BASEADDR, LPUART_RX_DMA_CHANNEL);
  
  /* 初始化DMA */
  EDMA_GetDefaultConfig(&config);
  EDMA_Init(EXAMPLE_LPUART_DMA_BASEADDR, &config);
  /*创建eDMA传句柄*/
  EDMA_CreateHandle(&g_lpuartTxEdmaHandle, EXAMPLE_LPUART_DMA_BASEADDR, LPUART_TX_DMA_CHANNEL);
  EDMA_CreateHandle(&g_lpuartRxEdmaHandle, EXAMPLE_LPUART_DMA_BASEADDR, LPUART_RX_DMA_CHANNEL);
  
  /* 创建 LPUART DMA 句柄 */
  LPUART_TransferCreateHandleEDMA(DEMO_LPUART, &g_lpuartEdmaHandle, LPUART_UserCallback, NULL, &g_lpuartTxEdmaHandle,
                                  &g_lpuartRxEdmaHandle);
}




/* LPUART 回调函数 */
void LPUART_UserCallback(LPUART_Type *base, lpuart_edma_handle_t *handle, status_t status, void *userData)
{
    userData = userData;

    if (kStatus_LPUART_TxIdle == status)
    {
        txBufferFull = false;
        txOnGoing = false;
    }

    if (kStatus_LPUART_RxIdle == status)
    {
        rxBufferEmpty = false;
        rxOnGoing = false;
    }
}
