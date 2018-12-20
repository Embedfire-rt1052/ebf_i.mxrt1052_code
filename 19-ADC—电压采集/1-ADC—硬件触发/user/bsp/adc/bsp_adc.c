
#include "fsl_debug_console.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"  
#include "fsl_adc.h"
  
#include "pad_config.h"  
#include "./adc/bsp_adc.h" 

extern volatile bool ADC_ConversionDoneFlag;
extern volatile uint32_t ADC_ConvertedValue;

/**
* @brief  初始化ADC相关IOMUXC的MUX复用配置
* @param  无
* @retval 无
*/
static void ADC_IOMUXC_MUX_Config(void)
{
  IOMUXC_SetPinMux(CORE_BOARD_ADC_IOMUXC_CH0, 0U);  
  IOMUXC_SetPinMux(CORE_BOARD_ADC_IOMUXC_CH15, 0U);
}

/**
* @brief  初始化ADC相关IOMUXC的MUX复用配置
* @param  无
* @retval 无
*/
static void ADC_IOMUXC_PAD_Config(void)
{
  IOMUXC_SetPinConfig(CORE_BOARD_ADC_IOMUXC_CH0, ADC_PAD_CONFIG_DATA);  
  IOMUXC_SetPinConfig(CORE_BOARD_ADC_IOMUXC_CH15, ADC_PAD_CONFIG_DATA);   
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

  
   /** ADC，GPIO配置 **/   
  adc_config.direction = kGPIO_DigitalInput; //输入模式
//adc_config.outputLogic =  1;                //默认高电平，在输出模式下配置该选项无效
  adc_config.interruptMode = kGPIO_NoIntmode; //不使用中断
  
  GPIO_PinInit(CORE_BOARD_ADC_GPIO_CH0, CORE_BOARD_ADC_GPIO_PIN_CH0, &adc_config);
  GPIO_PinInit(CORE_BOARD_ADC_GPIO_CH15, CORE_BOARD_ADC_GPIO_PIN_CH15, &adc_config);
}


 /**
  * @brief  初始化ADC工作模式
  * @param  无
  * @retval 无
  */
static void ADC_Mode_Config(void)
{
  adc_config_t adcConfigStrcut; //定义ADC 模式配置结构体
  adc_channel_config_t adcChannelConfigStruct; //ADC 通道配置结构体
  
  ADC_GetDefaultConfig(&adcConfigStrcut); //获取ADC 默认工作模式
  adcConfigStrcut.resolution = kADC_Resolution12Bit;
  ADC_Init(ADCx, &adcConfigStrcut); //配置ADC工作模式
  
  adcChannelConfigStruct.channelNumber = DEMO_ADC_USER_CHANNEL;
  adcChannelConfigStruct.enableInterruptOnConversionCompleted = false; //禁止转换完成中断
  
  /*配置转换通道与转换通道组之间的关联，这里第一次涉及转换通道组*/
  ADC_SetChannelConfig(ADCx, DEMO_ADC_CHANNEL_GROUP0, &adcChannelConfigStruct);
  ADC_SetChannelConfig(ADCx, DEMO_ADC_CHANNEL_GROUP1, &adcChannelConfigStruct);
  
  ///*设置ADC的硬件求平均值*/
  //ADC_SetHardwareAverageConfig(ADCx, kADC_HardwareAverageCount32);
  
  /*进行硬件校准*/
  if (kStatus_Success == ADC_DoAutoCalibration(ADCx))
  {
     PRINTF("校准完成 Done.\r\n");
  }
  else
  {
     PRINTF("校准失败\r\n");
  }
  

}

/*配置为允许外部触发*/
void ADC_ETC_Config(void)
{
  adc_etc_config_t adcEtcConfig;//外部配置
  adc_etc_trigger_config_t adcEtcTriggerConfig; //adc 外部触发配置
  adc_etc_trigger_chain_config_t adcEtcTriggerChainConfig; // ADC 触发连配置
  
  ADC_ETC_GetDefaultConfig(&adcEtcConfig);
  adcEtcConfig.XBARtriggerMask = 1U; /* Enable the external XBAR trigger0.允许外部触发*/
  ADC_ETC_Init(DEMO_ADC_ETC_BASE, &adcEtcConfig);
  
  
  /* 
   *Set the external XBAR trigger0 configuration. 
   *配置ADC外部触发源属性。
  */
  adcEtcTriggerConfig.enableSyncMode = false;
  adcEtcTriggerConfig.enableSWTriggerMode = false;
  adcEtcTriggerConfig.triggerChainLength = DEMO_ADC_ETC_CHAIN_LENGTH; /* Chain length is 2. */
  adcEtcTriggerConfig.triggerPriority = 0U;
  adcEtcTriggerConfig.sampleIntervalDelay = 0U;
  adcEtcTriggerConfig.initialDelay = 0U;
  //DEMO_ADC_ETC_BASE 触发器基地址，触发器组 触发器配置
  ADC_ETC_SetTriggerConfig(DEMO_ADC_ETC_BASE, 0U, &adcEtcTriggerConfig);
  
  
      /*************************************************************************************************************************/
    adcEtcTriggerChainConfig.enableB2BMode = true;
    adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U
                                                   << DEMO_ADC_CHANNEL_GROUP0; /* Select ADC_HC0 register to trigger. 在这里设置了触发源*/
    adcEtcTriggerChainConfig.ADCChannelSelect =
        DEMO_ADC_ETC_CHANNEL0; /* ADC_HC0 will be triggered to sample Corresponding channel. */
    adcEtcTriggerChainConfig.InterruptEnable = kADC_ETC_Done0InterruptEnable; /* Enable the Done0 interrupt. */
    ADC_ETC_SetTriggerChainConfig(DEMO_ADC_ETC_BASE, 0U, 0U,
                                  &adcEtcTriggerChainConfig); /* Configure the trigger0 chain0. */
   /*****************************************************************************************************************************/


    /****************************************************************************************************************************/                             
    adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U
                                                   << DEMO_ADC_CHANNEL_GROUP1; /* Select ADC_HC1 register to trigger. */
    adcEtcTriggerChainConfig.ADCChannelSelect =
        DEMO_ADC_ETC_CHANNEL1; /* ADC_HC1 will be triggered to sample Corresponding channel. */
    adcEtcTriggerChainConfig.InterruptEnable = kADC_ETC_Done1InterruptEnable; /* Enable the Done1 interrupt. */
    ADC_ETC_SetTriggerChainConfig(DEMO_ADC_ETC_BASE, 0U, 1U,
                                  &adcEtcTriggerChainConfig); /* Configure the trigger0 chain1. */
    /********************************************************************************************************************************/




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
  
  /*开启中断*/
  EnableIRQ(ADC_IRQ); // 开启ADC 中断。
}


/*ADC中断服务函数*/
void ADC_IRQHandler(void)
{
  /*设置ADC转换完成标志*/
  ADC_ConversionDoneFlag = true;
  /*读取转换结果，读取之后硬件自动清除转换完成中断标志位*/
  ADC_ConvertedValue = ADC_GetChannelConversionValue(ADCx, DEMO_ADC_CHANNEL_GROUP);
}

