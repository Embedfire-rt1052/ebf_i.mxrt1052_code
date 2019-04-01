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
  *CPU_MIMXRT1052DVL6B,PRINTF_FLOAT_ENABLE=1, SCANF_FLOAT_ENABLE=1, PRINTF_ADVANCED_ENABLE=1, SCANF_ADVANCED_ENABLE=1
  ******************************************************************
  */


#include "fsl_debug_console.h"

#include "board.h"
#include "pin_mux.h"

#include "clock_config.h"
#include "./bsp/nvic/bsp_nvic.h"
#include "./bsp/led/bsp_led.h" 
#include "./bsp/adc/bsp_adc.h"



volatile bool ADC_ConversionDoneFlag; //标志位
volatile uint32_t ADC_ConvertedValue;


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
  adc_channel_config_t adcChannelConfigStruct;//定义ADC 通道配置结构体
  float ADC_ConvertedValueLocal = 0;  //保存转换得到的电压值
  
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
  
  adcChannelConfigStruct.channelNumber = DEMO_ADC_USER_CHANNEL;
  adcChannelConfigStruct.enableInterruptOnConversionCompleted = true;
  while(1)
  {
    ADC_ConversionDoneFlag = false;
    /*未开启连续转换时和硬件触发转换时，每调用一次该函数触发一次转换*/
    ADC_SetChannelConfig(ADCx, DEMO_ADC_CHANNEL_GROUP, &adcChannelConfigStruct);
    while (ADC_ConversionDoneFlag == false)
    {
      
    }
    
    /*输出原始转换结果*/
    PRINTF("The Conversion Value: %d\r\n", ADC_ConvertedValue);
    
    /*将结果转换为电压值并通过串口输出*/
    ADC_ConvertedValueLocal =((float)ADC_ConvertedValue)/4095.0f*3.3f; 
    PRINTF("The current AD value = %f V \r\n",ADC_ConvertedValueLocal);
    PRINTF("\r\n");
    
    /*防止转换速度过快导致串口软件接收异常*/
    delay(LED_DELAY_COUNT/3);
    
  }

}



/****************************END OF FILE**********************/

