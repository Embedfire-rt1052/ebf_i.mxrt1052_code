/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   485收发实验
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
#include "./bsp/nvic/bsp_nvic.h"
#include "./bsp/led/bsp_led.h" 
#include "./bsp/485/bsp_485.h"



extern volatile uint8_t ucTemp;//用于保存收到的字符
extern volatile bool resived;//用于保存接收状态

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
   
  /*初始化uart5*/
  UART_Config();
  
  /*初始化485芯片接收与发送控制引脚*/
  _485_Control_GPIO_init();  
  
  PRINTF("这是一个485通信实验\r\n");
  PRINTF("程序大约每1秒发送一次字符‘a’。接收到消息后则输出接收到的字符\r\n");
  while(1)
  {
    /* 亮红灯 */
    RGB_RED_LED_ON;
    RGB_GREEN_LED_OFF
    /*执行485发送*/
    _485_SendByte(DEBUG_UARTx,'a');  
    PRINTF("send finish \r\n");
     
    delay(LED_DELAY_COUNT);
    
    if(resived)
    {
      PRINTF("resive is:%c\r\n",ucTemp);
      resived = false;
    }
    /*亮绿灯*/
    RGB_RED_LED_OFF
    RGB_GREEN_LED_ON;
    delay(LED_DELAY_COUNT);    
  }
}



/****************************END OF FILE**********************/
      
