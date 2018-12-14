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
  
    PRINTF("GPIO输出-使用固件库点亮LED\r\n");
  
    /* 初始化LED引脚 */
    LED_GPIO_Config();  
    
    while(1)
    {         
      /* LED亮 */
      CORE_BOARD_LED_ON;
      /* 延时 */
      delay(LED_DELAY_COUNT);
      
      /* 独立操作红灯 */
      RGB_RED_LED_ON;
      delay(LED_DELAY_COUNT);
      
      RGB_RED_LED_OFF;
      delay(LED_DELAY_COUNT);
      
      /* 独立操作绿灯 */
      RGB_GREEN_LED_ON;
      delay(LED_DELAY_COUNT);
      
      RGB_GREEN_LED_OFF;
      delay(LED_DELAY_COUNT);
      
      /* 独立操作蓝灯 */
      RGB_BLUE_LED_ON;
      delay(LED_DELAY_COUNT);
      
      RGB_BLUE_LED_OFF;
      delay(LED_DELAY_COUNT);   

      /* 整体操作红色 */
      RGB_LED_COLOR_RED;
      delay(LED_DELAY_COUNT);   
      
      /* 整体操作绿色 */
      RGB_LED_COLOR_GREEN;
      delay(LED_DELAY_COUNT);   
      
      /* 整体操作蓝色 */
      RGB_LED_COLOR_BLUE;
      delay(LED_DELAY_COUNT);   
      
      /* 整体操作黄色 */
      RGB_LED_COLOR_YELLOW;
      delay(LED_DELAY_COUNT);   
      
      /* 整体操作紫色 */
      RGB_LED_COLOR_PURPLE;
      delay(LED_DELAY_COUNT);   
      
      /* 整体操作青色 */
      RGB_LED_COLOR_CYAN;
      delay(LED_DELAY_COUNT);   
      
      /* 整体操作白色 */
      RGB_LED_COLOR_WHITE;
      delay(LED_DELAY_COUNT);   
      
      /* 整体操作黑色（全关闭） */
      RGB_LED_COLOR_OFF;
      delay(LED_DELAY_COUNT);   
    }     

}
/****************************END OF FILE**********************/
