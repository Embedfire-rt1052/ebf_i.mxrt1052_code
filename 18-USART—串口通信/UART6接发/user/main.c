/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   串口中断接收测试
  ******************************************************************
  * @attention
  *
  * 实验平台:野火  i.MXRT1052开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************
  */
  
  
/*************注意：串口6不能与蜂鸣器同时使用，使用串口6时要拔下J37跳线帽*******************************/

#include "fsl_debug_console.h"

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "./led/bsp_led.h" 
#include "./uart/bsp_usart.h"



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

  /*初始化uart6*/
  USART_Config();
  
  /*输出提示信息*/
  Usart_SendString( DEBUG_USARTx,"     这是一个串口中断接收回显实验 \r\n");
  Usart_SendString( DEBUG_USARTx, "在接收中断服务函数中接收并发送收到的数据\r\n");
  Usart_SendString( DEBUG_USARTx, "RGB灯交替显示红色、绿色表示主循环正在运行\r\n");
  while(1)
  {
    /* 亮红灯 */
    RGB_RED_LED_ON;
    RGB_GREEN_LED_OFF
    delay(LED_DELAY_COUNT);

    /*亮绿灯*/
    RGB_RED_LED_OFF
    RGB_GREEN_LED_ON;
    delay(LED_DELAY_COUNT);
  }
}



/****************************END OF FILE**********************/

