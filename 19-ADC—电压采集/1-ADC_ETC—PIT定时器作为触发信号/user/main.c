/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   ADC硬件触发采集测试
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
    
#include "./bsp/nvic/bsp_nvic.h"
#include "./bsp/led/bsp_led.h" 
#include "./bsp/adc/bsp_adc.h"
#include "./bsp/pit/bsp_pit.h"
#include "./bsp/xbara/bsp_xbara.h"




volatile uint32_t g_AdcConversionValue0;
volatile uint32_t g_AdcConversionValue1;
volatile  bool b_Value0_Conversion_complete_flag;
volatile  bool b_Value1_Conversion_complete_flag;

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
  
  /*初始化 ADC */
  ADC_Config();
  
  /*初始化xborta*/
  XBARA_Configuration();
  
  /*初始化并开启PIT定时器*/
  PIT_Configuration();
  PIT_start_timer();
  
  /*初始化转换完成标志*/
  b_Value0_Conversion_complete_flag = false;
  b_Value1_Conversion_complete_flag = false;
  
  while(1)
  {
    /*检测是否转换完成*/
    while(!(b_Value0_Conversion_complete_flag&&b_Value1_Conversion_complete_flag))
    {

    }
   /*打印转换结果*/
   PRINTF("CH0:%d  CH15:%d\r\n", g_AdcConversionValue0, g_AdcConversionValue1);
   b_Value0_Conversion_complete_flag = false;
   b_Value1_Conversion_complete_flag = false;
  }

}



/****************************END OF FILE**********************/

