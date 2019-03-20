/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   使用内核时间戳计数器延时
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
#include "./delay/core_delay.h"   



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

//使用不精确的延时
//#define LED_DELAY   delay(LED_DELAY_COUNT)

//使用精确的延时，延时不受代码优化影响
#define LED_DELAY   CPU_TS_Tmr_Delay_S(1)


/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
    #if (!CPU_TS_INIT_IN_DELAY_FUNCTION)      
      //使用时间戳延时函数前必须先使能计数器
      CPU_TS_TmrInit();
    #endif
  
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
  
    PRINTF("使用内核时间戳精确延时\r\n");
  
    /* 初始化LED引脚 */
    LED_GPIO_Config();  
    
    while(1)
    {         
      /* LED亮 */
      CORE_BOARD_LED_ON;
      /* 延时 */
      LED_DELAY;
      
      /* 独立操作红灯 */
      RGB_RED_LED_ON;
      LED_DELAY;
      
      RGB_RED_LED_OFF;
      LED_DELAY;
      
      /* 独立操作绿灯 */
      RGB_GREEN_LED_ON;
      LED_DELAY;
      
      RGB_GREEN_LED_OFF;
      LED_DELAY;
      
      /* 独立操作蓝灯 */
      RGB_BLUE_LED_ON;
      LED_DELAY;
      
      RGB_BLUE_LED_OFF;
      LED_DELAY;    

      /* 整体操作红色 */
      RGB_LED_COLOR_RED;
      LED_DELAY;    
      
      /* 整体操作绿色 */
      RGB_LED_COLOR_GREEN;
      LED_DELAY;    
      
      /* 整体操作蓝色 */
      RGB_LED_COLOR_BLUE;
      LED_DELAY;    
      
      /* 整体操作黄色 */
      RGB_LED_COLOR_YELLOW;
      LED_DELAY;    
      
      /* 整体操作紫色 */
      RGB_LED_COLOR_PURPLE;
      LED_DELAY;    
      
      /* 整体操作青色 */
      RGB_LED_COLOR_CYAN;
      LED_DELAY;    
      
      /* 整体操作白色 */
      RGB_LED_COLOR_WHITE;
      LED_DELAY;    
      
      /* 整体操作黑色（全关闭） */
      RGB_LED_COLOR_OFF;
      LED_DELAY;    
    }     

}
/****************************END OF FILE**********************/
