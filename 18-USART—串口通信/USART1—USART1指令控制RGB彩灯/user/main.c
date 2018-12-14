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
  ******************************************************************
  */
  

#include "fsl_debug_console.h"

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "./led/bsp_led.h" 
#include "./uart/bsp_usart.h"


static void Show_Message(void);
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
  char ch;
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

  /*初始化uart1*/
  USART_Config();
    

  while(1)
  {
    /* 获取字符指令 */
    ch = GETCHAR();
    PRINTF("接收到字符：%c\n",ch);

    /* 根据字符指令控制RGB彩灯颜色 */
    switch(ch)
    {
      case '1':
        RGB_LED_COLOR_RED;
      break;
      case '2':
        RGB_LED_COLOR_GREEN;
      break;
      case '3':
        RGB_LED_COLOR_BLUE;
      break;
      case '4':
        RGB_LED_COLOR_YELLOW;
      break;
      case '5':
        RGB_LED_COLOR_PURPLE;
      break;
      case '6':
        RGB_LED_COLOR_CYAN;
      break;
      case '7':
        RGB_LED_COLOR_WHITE;
      break;
      case '8':
        RGB_LED_COLOR_OFF;
      break;
      default:
        /* 如果不是指定指令字符，打印提示信息 */
        Show_Message();
      break;
      } 
    }

}


static void Show_Message(void)
{
  PRINTF("\r\n   这是一个通过串口通信指令控制RGB彩灯实验 \n");
  PRINTF("使用  USART  参数为：%d 8-N-1 \n",DEBUG_USART_BAUDRATE);
  PRINTF("开发板接到指令后控制RGB彩灯颜色，指令对应如下：\n");
  PRINTF("   指令   ------ 彩灯颜色 \n");
  PRINTF("     1    ------    红 \n");
  PRINTF("     2    ------    绿 \n");
  PRINTF("     3    ------    蓝 \n");
  PRINTF("     4    ------    黄 \n");
  PRINTF("     5    ------    紫 \n");
  PRINTF("     6    ------    青 \n");
  PRINTF("     7    ------    白 \n");
  PRINTF("     8    ------    灭 \n");  
}

/****************************END OF FILE**********************/

