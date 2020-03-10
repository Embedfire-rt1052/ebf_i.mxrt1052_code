/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   GPIO输入—按键中断检测
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  i.MXRT1052开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "fsl_gpt.h"

#include "pin_mux.h"
#include "clock_config.h"

#include "./bsp/nvic/bsp_nvic.h"
#include "./bsp/led/bsp_led.h"
#include "./bsp/key/bsp_key_it.h"


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
    /* 初始化内存管理单元 */
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
  
    PRINTF("GPIO输入—按键中断实验\r\n");
  
    /* 初始化LED引脚 */
    LED_GPIO_Config();
    
    /* 初始化KEY引脚 */
    Key_IT_GPIO_Config();
    
    /* 等待按键中断来临，当按键按下就执行按键中断服务函数，
       按键中断服务函数在bsp_key_it.c实现。
       中断服务函数会对全局变量g_KeyDown设置为true。
    */ 

    while(1)
    {   
      /* WAUP按键的标志 */
      /* 若g_KeyDown为true表明按键被按下 */
      if(g_KeyDown[CORE_BOARD_WAUP_KEY_ID])
      {
          /* 稍微延时 */
          delay(100);
          /* 等待至按键被释放 （高电平）*/
          if(1 == GPIO_PinRead(CORE_BOARD_WAUP_KEY_GPIO, CORE_BOARD_WAUP_KEY_GPIO_PIN))
          {
              /* 翻转LED灯，串口输出信息 */
              CORE_BOARD_LED_TOGGLE;
              PRINTF("检测到 %s 按键操作\r\n", CORE_BOARD_WAUP_KEY_NAME);
          }
          /* 重新设置标志位 */
          g_KeyDown[CORE_BOARD_WAUP_KEY_ID] = false; 
      }
      
      /* MODE按键的标志 */
      /* 若g_KeyDown为true表明按键被按下 */
      if(g_KeyDown[CORE_BOARD_MODE_KEY_ID])
      {
          /* 稍微延时 */
          delay(100);
          /* 等待至按键被释放 （高电平）*/
          if(1 == GPIO_PinRead(CORE_BOARD_MODE_KEY_GPIO, CORE_BOARD_MODE_KEY_GPIO_PIN))
          {
              /* 翻转LED灯，串口输出信息 */
              CORE_BOARD_LED_TOGGLE;
              PRINTF("检测到 %s 按键操作\r\n", CORE_BOARD_MODE_KEY_NAME);
          }
          /* 重新设置标志位 */
          g_KeyDown[CORE_BOARD_MODE_KEY_ID] = false; 
      }
    }     

}
/*********************************************END OF FILE**********************/




