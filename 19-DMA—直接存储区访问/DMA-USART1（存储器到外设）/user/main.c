/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   使用DMA实现串口的发送与接收
  ******************************************************************
  * @attention
  *
  * 实验平台:野火  i.MXRT1052开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************
  */
 
#include "fsl_debug_console.h"  
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "./led/bsp_led.h"
#include "./bsp/dma_uart/bsp_dma_uart.h"

   
/*设置TX和RX数据存储区*/
AT_NONCACHEABLE_SECTION_INIT(uint8_t g_txBuffer[ECHO_BUFFER_LENGTH]) = {0};
AT_NONCACHEABLE_SECTION_INIT(uint8_t g_rxBuffer[ECHO_BUFFER_LENGTH]) = {0};


/*定义UART传输状态标志*/
volatile bool rxBufferEmpty = true;  //接收缓冲区空
volatile bool txBufferFull = false;  //发送缓冲区满
volatile bool txOnGoing = false;     //正在执行发送
volatile bool rxOnGoing = false;     //正在执行接收


/*句柄定义*/
lpuart_edma_handle_t g_lpuartEdmaHandle;  //串口DMA传输句柄
edma_handle_t g_lpuartTxEdmaHandle;       //串口DMA发送句柄
edma_handle_t g_lpuartRxEdmaHandle;       //串口DMA接收句柄


/*设置系统启动提示信息*/
AT_NONCACHEABLE_SECTION_INIT(uint8_t g_tipString[]) =
    "LPUART EDMA example\r\nSend back received data\r\nEcho every 8 characters\r\n";

/*****************************************************************
 * 主函数
 *****************************************************************/
int main(void)
{
  
  /*定义传输结构体*/
  lpuart_transfer_t xfer;          //定义提示信息传输结构体
  lpuart_transfer_t sendXfer;      //定义发送传输结构体
  lpuart_transfer_t receiveXfer;   //定义接收传输结构体
  
  
  /* 初始化内存保护单元 */
  BOARD_ConfigMPU();
  /* 初始化开发板引脚 */
  BOARD_InitPins();
  /* 初始化开发板时钟 */
  BOARD_BootClockRUN();
  /* 初始化调试串口 */    
  BOARD_InitDebugConsole();
  
  /* 打印系统时钟 */
  PRINTF("\r\n");
  PRINTF("*****欢迎使用 野火i.MX RT1052 开发板*****\r\n");
  PRINTF("CPU:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
  PRINTF("AHB:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AhbClk));
  PRINTF("SEMC:            %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
  PRINTF("SYSPLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
  PRINTF("SYSPLLPFD0:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk));
  PRINTF("SYSPLLPFD1:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd1Clk));
  PRINTF("SYSPLLPFD2:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
  PRINTF("SYSPLLPFD3:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd3Clk));
  
  
  UART_Init();     //初始化串口
  UART_DMA_Init(); //初始化串口DMA传输使用的DMA

  /* 发送提示信息 */
  xfer.data = g_tipString;
  xfer.dataSize = sizeof(g_tipString) - 1;
  txOnGoing = true;
  LPUART_SendEDMA(DEMO_LPUART, &g_lpuartEdmaHandle, &xfer);
  /* 等待发送完成 */
  while (txOnGoing)
  {
    
  }
  
  /* 设置UART发送和接收传输结构体 */
  sendXfer.data = g_txBuffer;
  sendXfer.dataSize = ECHO_BUFFER_LENGTH;
  receiveXfer.data = g_rxBuffer;
  receiveXfer.dataSize = ECHO_BUFFER_LENGTH;
  
  /*轮询检测串口当前状态，接收到数据后立即发送出去*/
  while (1)
  {
    /* 如果接收空闲并且接收缓冲区为空，表示当前串口空闲此时等待接收数据*/
    if ((!rxOnGoing) && rxBufferEmpty)
    {
      rxOnGoing = true;
      LPUART_ReceiveEDMA(DEMO_LPUART, &g_lpuartEdmaHandle, &receiveXfer);
    }
    
    /* 如果发送空闲并且发送缓冲器满，此时应当开始发送数据*/
    if ((!txOnGoing) && txBufferFull)
    {
      txOnGoing = true;
      LPUART_SendEDMA(DEMO_LPUART, &g_lpuartEdmaHandle, &sendXfer);
    }
    
    /* 如果发送缓冲区空并且接收缓冲区满，此时应将接收缓冲区的内容拷贝到发送缓冲区 */
    if ((!rxBufferEmpty) && (!txBufferFull))
    {
      memcpy(g_txBuffer, g_rxBuffer, ECHO_BUFFER_LENGTH);
      rxBufferEmpty = true;
      txBufferFull = true;
    }
  }
}
