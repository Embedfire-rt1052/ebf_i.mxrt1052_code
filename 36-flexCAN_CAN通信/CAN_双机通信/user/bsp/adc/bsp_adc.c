
#include "fsl_debug_console.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"  
#include "fsl_adc.h"
  
#include "pad_config.h" 
#include "./bsp/nvic/bsp_nvic.h"
#include "./bsp/adc/bsp_adc.h" 

extern volatile bool ADC_ConversionDoneFlag;
extern volatile uint32_t ADC_ConvertedValue;

/**
* @brief  初始化ADC相关IOMUXC的MUX复用配置
* @param  无
* @retval 无
*/
static void ADC_IOMUXC_MUX_Config(void)
{
  IOMUXC_SetPinMux(CORE_BOARD_ADC_IOMUXC, 0U);  
}
/**
* @brief  初始化ADC相关IOMUXC的MUX复用配置
* @param  无
* @retval 无
*/
static void ADC_IOMUXC_PAD_Config(void)
{
  IOMUXC_SetPinConfig(CORE_BOARD_ADC_IOMUXC, ADC_PAD_CONFIG_DATA);   
}

 /**
  * @brief  初始化ADC相关的GPIO模式
  * @param  无
  * @retval 无
  */
static void ADC_IO_Mode_Config(void)
{
    /* 定义gpio初始化配置结构体 */
  gpio_pin_config_t adc_config; 
   /*ADC，GPIO配置*/   
  adc_config.direction = kGPIO_DigitalInput; //输入模式
//adc_config.outputLogic =  1;                //默认高电平，在输入模式下配置该选项无效
  adc_config.interruptMode = kGPIO_NoIntmode; //不使用中断
  
  GPIO_PinInit(CORE_BOARD_ADC_GPIO, CORE_BOARD_ADC_GPIO_PIN, &adc_config);
}


 /**
  * @brief  初始化ADC工作模式
  * @param  无
  * @retval 无
  */
static void ADC_Mode_Config(void)
{
  adc_config_t adcConfigStrcut; //定义ADC 模式配置结构体
  
  /*初始化ADC工作模式*/
  adc_channel_config_t adcChannelConfigStruct;    //ADC 通道配置结构体
  ADC_GetDefaultConfig(&adcConfigStrcut); //获取ADC 默认工作模式
  adcConfigStrcut.resolution = kADC_Resolution12Bit;
  ADC_Init(ADCx, &adcConfigStrcut); //配置ADC工作模式
  
  /*设置ADC的硬件求平均值*/
  ADC_SetHardwareAverageConfig(ADCx, kADC_HardwareAverageCount32);
  
  /*ADC转换通道设置*/
  adcChannelConfigStruct.channelNumber = DEMO_ADC_USER_CHANNEL;//设置ADC转换通道对应的外部输入通道
  adcChannelConfigStruct.enableInterruptOnConversionCompleted = true; //使能转换完成中断
  ADC_SetChannelConfig(ADCx, DEMO_ADC_CHANNEL_GROUP, &adcChannelConfigStruct);

  /*进行硬件校准*/
  if (kStatus_Success == ADC_DoAutoCalibration(ADCx))
  {
     PRINTF("校准完成 Done.\r\n");
  }
  else
  {
     PRINTF("校准失败\r\n");
  }
  
  
  /*设置中断优先级,*/
  set_IRQn_Priority(ADC_IRQ,Group4_PreemptPriority_6, Group4_SubPriority_0);
  /*开启中断*/
  EnableIRQ(ADC_IRQ); // 开启ADC 中断。
}

/**
  * @brief  初始化控制ADC的IO
  * @param  无
  * @retval 无
  */
void ADC_Config(void)
{
  
  ADC_IOMUXC_MUX_Config();//设置引脚功能
  ADC_IOMUXC_PAD_Config();//设置引脚配置参数，驱动强度、转换速率等
  ADC_IO_Mode_Config(); //设置引脚模式，输入/输出，是否开启中断等
  ADC_Mode_Config(); //初始化ADC工作模式，并且进行硬件校准。
  
  
}


/*ADC中断服务函数*/
void ADC_IRQHandler(void)
{
  /*设置ADC转换完成标志*/
  ADC_ConversionDoneFlag = true;
  /*读取转换结果，读取之后硬件自动清除转换完成中断标志位*/
  ADC_ConvertedValue = ADC_GetChannelConversionValue(ADCx, DEMO_ADC_CHANNEL_GROUP);
}









