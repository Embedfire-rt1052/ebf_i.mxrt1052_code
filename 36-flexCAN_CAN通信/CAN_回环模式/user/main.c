/**
******************************************************************
* @file    main.c
* @author  fire
* @version V1.0
* @date    2019-xx-xx
* @brief   CAN_回环模式
******************************************************************
* @attention
*
* 实验平台:野火  i.MXRT1052开发板 
* 论坛    :http://www.firebbs.cn
* 淘宝    :http://firestm32.taobao.com
******************************************************************
*/


#include "fsl_debug_console.h"

#include "board.h"
#include "pin_mux.h"
#include "fsl_flexcan.h"


#include "clock_config.h"
#include "./bsp/nvic/bsp_nvic.h"
#include "./bsp/led/bsp_led.h" 
#include "./can/bsp_can.h"



/*全局变量，保存CAN接收状态，false：没有接收到消息。true:接收到消息并且还未被处理*/
volatile bool rxComplete = false;

/*全局变量，定义接收和发送消息结构体*/
flexcan_frame_t txFrame, rxFrame;


/*******************************************************************
* Prototypes
*******************************************************************/
/**
* @brief 延时一段时间
*/
void delay(uint32_t count);

/*******************************************************************
* Code
*******************************************************************/
/**
* @note 本函数在不同的优化模式下延时时间不同，
*       如flexspi_nor_debug和flexspi_nor_release版本的程序中，
*       flexspi_nor_release版本的延时要短得多  
*/ 
void delay(uint32_t count)
{
  volatile uint32_t i = 0;
  for (i = 0; i < count; ++i)
  {
    __asm("NOP"); /* 调用nop空指令 */
  }
}

/**
* @brief  主函数
* @param  无
* @retval 无
*/
int main(void)
{

  /* 初始化内存保护单元 */
  BOARD_ConfigMPU();
  /* 初始化开发板引脚 */
  BOARD_InitPins();
  /* 初始化开发板时钟 */
  BOARD_BootClockRUN();
  /* 初始化调试串口 */
  BOARD_InitDebugConsole();
  
  /*设置中断优先级分组*/
  Set_NVIC_PriorityGroup(Group_4); 
  
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
  
  /* 初始化LED引脚 */
  LED_GPIO_Config(); 
  
  PRINTF("\r\n==FlexCAN 回环测试例程 -- Start.==\r\n\r\n");
  CAN_Config();
  
  while(1)
  {
    CAN_RX_Buffer_Config(0x123,RX_MESSAGE_BUFFER_NUM);//初始化接收缓冲区
    
    FLEXCAN_SetTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, true);//设置发送消息缓冲区
    CAN_TX_Buffer_Config(0x123);//初始化发送数据格式以及数据
    
    PRINTF("从发送缓冲区 MB%d 向接收缓冲区 MB%d 发送消息\r\n", TX_MESSAGE_BUFFER_NUM, RX_MESSAGE_BUFFER_NUM);

    /*输出发送缓冲区的内容*/
    PRINTF("tx word0 = 0x%x\r\n", txFrame.dataWord0);
    PRINTF("tx word1 = 0x%x\r\n", txFrame.dataWord1);

    /*执行发送*/
    FLEXCAN_TransferSendBlocking(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, &txFrame);

    /* 等待接收完成 */
    while (!rxComplete)
    {
    }

    PRINTF("\r\n接收缓冲区MB%d 的数据\r\n", RX_MESSAGE_BUFFER_NUM);
    
    /*输出接收到的内容*/
    PRINTF("rx word0 = 0x%x\r\n", rxFrame.dataWord0);
    PRINTF("rx word1 = 0x%x\r\n", rxFrame.dataWord1);

    /* 停止 FlexCAN 接收和发送. */
    FLEXCAN_DisableMbInterrupts(EXAMPLE_CAN, 1 << RX_MESSAGE_BUFFER_NUM);

    PRINTF("\r\n==FlexCAN 回环模式测试结束==\r\n");
    while (1)
    {
      ;
    }
  }
  
}



/****************************END OF FILE**********************/

