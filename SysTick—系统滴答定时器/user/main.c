/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   用V2.3.1版本库建的工程模板
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
#include "fsl_gpio.h"
#include "fsl_gpt.h"

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "./led/bsp_led.h"   
#include "./SysTick/bsp_SysTick.h"



/*******************************************************************
 * Prototypes
 *******************************************************************/
/*!
 * @brief delay a while.
 */
void delay(uint32_t count);

/*******************************************************************
 * Code
 *******************************************************************/
void delay(uint32_t count)
{
    volatile uint32_t i = 0;
    for (i = 0; i < count; ++i)
    {
        __asm("NOP"); /* delay */
    }
}

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
    /* 初始化内存管理单元 */
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
  
    PRINTF("SysTick—系统滴答定时器\r\n");
  
    /* 初始化LED引脚 */
    LED_GPIO_Config();  
    
    /* 启动SysTick */
    SysTick_Init();
    
    while(1)
    {
      /* LED亮 */
      CORE_BOARD_LED_ON;
      RGB_LED_COLOR_GREEN;
      /* 延时 */

      //SysTick_Delay_Ms(500);
      Delay_ms(500);
      
      /* LED灭 */
      CORE_BOARD_LED_OFF;
      RGB_LED_COLOR_OFF;
      /* 延时 */

      //SysTick_Delay_Ms(500);
      Delay_ms(500);
      
    }     

}
/****************************END OF FILE**********************/
