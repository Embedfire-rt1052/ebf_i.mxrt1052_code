/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   GPIO输出—使用固件库点亮LED灯
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
  
//  uint32_t i = 0;//用于for循环
  
  
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
  //DMA_Config();
  
  
  DMA_UART_Config();
  while(1)
  {         
//    while (g_Transfer_Done != true)
//    {
//      //等待传输完成
//    }
//    
//    /* 打印目的缓存区内容 */
//    PRINTF("\r\n\r\eDAM 存储器到存储器传输完成\r\n\r\n");
//    PRINTF("Destination Buffer:\r\n");
//    for (i = 0; i < BUFF_LENGTH; i++)
//    {
//      PRINTF("%d\t", destAddr[i]);
//    }
//    while (1)
//    {
//          
//    }
  }     

}
/****************************END OF FILE**********************/
