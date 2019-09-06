/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   GPIO输入—按键查询检测
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
#include "./key/bsp_key.h"



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
  
    PRINTF("GPIO输入—按键查询检测\r\n");
  
    /* 初始化LED引脚 */
    LED_GPIO_Config();  
  
    /* 初始化KEY引脚 */
    Key_GPIO_Config();

    /* 初学时了解阻塞检测的原理即可 */
#if 1 
    //阻塞检测
    PRINTF("阻塞检测示例，按下按键可控制LED灯反转\r\n");
    while(1)
    {
      /* 检测WAUP按键 */
      if(Key_Scan(CORE_BOARD_WAUP_KEY_GPIO, CORE_BOARD_WAUP_KEY_GPIO_PIN) == KEY_ON )
      {
        CORE_BOARD_LED_TOGGLE;
        PRINTF("检测到 %s 按键操作\r\n", CORE_BOARD_WAUP_KEY_NAME);
      }
      
      /* 检测MODE按键 */
      if(Key_Scan(CORE_BOARD_MODE_KEY_GPIO, CORE_BOARD_MODE_KEY_GPIO_PIN) == KEY_ON )
      {
        CORE_BOARD_LED_TOGGLE;
        PRINTF("检测到 %s 按键操作\r\n", CORE_BOARD_MODE_KEY_NAME);
      }
    } 
    
#else 
    //使用状态机检测 ,不要求掌握
    PRINTF("状态机检测示例，短按按键可点亮LED灯，长按关闭\r\n");
    
    while(1)
    { 
      /* 检测WAUP按键 */
      Key_State state = KEY_NOT_PRESS;

      state = Key_Scan_Machine(CORE_BOARD_WAUP_KEY_GPIO,CORE_BOARD_WAUP_KEY_GPIO_PIN);    
      
      if(state == KEY_PRESS_RELEASE )
      {
        CORE_BOARD_LED_ON;
        PRINTF("检测到 %s 按键短按释放操作\r\n", CORE_BOARD_WAUP_KEY_NAME);
      }
      else if(state == KEY_LONG_PRESS_RELEASE )
      {
        CORE_BOARD_LED_OFF;
        PRINTF("检测到 %s 按键长按释放操作\r\n", CORE_BOARD_WAUP_KEY_NAME);
      }
      /* 检测MODE按键 */
      state = KEY_NOT_PRESS;

      state = Key_Scan_Machine(CORE_BOARD_MODE_KEY_GPIO,CORE_BOARD_MODE_KEY_GPIO_PIN);    
      
      if(state == KEY_PRESS_RELEASE )
      {
        CORE_BOARD_LED_ON;
        PRINTF("检测到 %s 按键短按释放操作\r\n", CORE_BOARD_MODE_KEY_NAME);
      }
      else if(state == KEY_LONG_PRESS_RELEASE )
      {
        CORE_BOARD_LED_OFF;
        PRINTF("检测到 %s 按键长按释放操作\r\n", CORE_BOARD_MODE_KEY_NAME);
      }

    } 
#endif

}
/****************************END OF FILE**********************/
