
#include "fsl_debug_console.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"  
#include "fsl_adc.h"
  
#include "pad_config.h"  
#include "./bsp/nvic/bsp_nvic.h"
#include "./bsp/adc/bsp_adc.h" 


extern volatile uint32_t g_AdcConversionValue0;
extern volatile uint32_t g_AdcConversionValue1;
extern volatile  bool b_Value0_Conversion_complete_flag;
extern volatile  bool b_Value1_Conversion_complete_flag;   


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
  
  /*ADC，GPIO配置 */   
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
  adc_config_t adcConfigStrcut;                   //定义ADC 模式配置结构体
  adc_channel_config_t adcChannelConfigStruct;    //ADC 通道配置结构体
  
  /*配置工作模式*/
  ADC_GetDefaultConfig(&adcConfigStrcut);         //获取ADC 默认工作模式
  adcConfigStrcut.resolution = kADC_Resolution12Bit;
  
  ADC_Init(ADCx, &adcConfigStrcut);               //配置ADC工作模式
  ADC_EnableHardwareTrigger(ADCx, true);          //使能硬件触发模式
  

  /*配置转换通道组*/
  
  //设置ADC转换通道对应的外部输入通道由ADC_ETC相关的寄存器指定
  adcChannelConfigStruct.channelNumber = DEMO_ADC_ETC_CHANNLE_NUMBER_x; 
  //禁止转换完成中断
  adcChannelConfigStruct.enableInterruptOnConversionCompleted = false;   
  ADC_SetChannelConfig(ADCx, DEMO_ADC_CHANNEL_GROUP0, &adcChannelConfigStruct);
  
  adcChannelConfigStruct.channelNumber = DEMO_ADC_ETC_CHANNLE_NUMBER_x;
  ADC_SetChannelConfig(ADCx, DEMO_ADC_CHANNEL_GROUP1, &adcChannelConfigStruct);


  /*进行硬件校准*/
  while(kStatus_Success != ADC_DoAutoCalibration(ADCx))
  {
    PRINTF("校准失败 \r\n");
  }
  PRINTF("校准完成\r\n");                                                                                               
}

/*配置为允许外部触发*/
void ADC_ETC_Config(void)
{
  adc_etc_config_t adcEtcConfig;//配置外部触发全局控制器。
  adc_etc_trigger_config_t adcEtcTriggerConfig; //配置外部触发转换通道组。主要包括优先级、触发方式、触发通道数量
  adc_etc_trigger_chain_config_t adcEtcTriggerChainConfig; // 配置外部触发转换通道组具体的ADC转换通道
  
  
  /*配置外部触发控制器*/
  ADC_ETC_GetDefaultConfig(&adcEtcConfig);
  
  /*如果要使用ADC2则必须要设置该属性*/
  adcEtcConfig.enableTSCBypass = false;//设置该位允许使用ADC2
  
  adcEtcConfig.XBARtriggerMask = 1<<ADC_ETC_XBARA_TRIGGER_CHANNELx; /* 并且选择了触发通道为触发通道0*/
  ADC_ETC_Init(DEMO_ADC_ETC_BASE, &adcEtcConfig);
  
  
  /*  配置外部触发转换通道组。*/
  adcEtcTriggerConfig.enableSyncMode = false;                      //异步模式
  adcEtcTriggerConfig.enableSWTriggerMode = false;                 //禁止软件触发
  adcEtcTriggerConfig.triggerChainLength = DEMO_ADC_ETC_CHAIN_LENGTH; //设置转换通道数量，实际转换通道数量为DEMO_ADC_ETC_CHAIN_LENGTH+1
  adcEtcTriggerConfig.triggerPriority = 0U;                         //外部触发优先级
  adcEtcTriggerConfig.sampleIntervalDelay = 0U;                                  
  adcEtcTriggerConfig.initialDelay = 0U;                            
  
  ADC_ETC_SetTriggerConfig(DEMO_ADC_ETC_BASE, ADC_ETC_XBARA_TRIGGER_CHANNELx, &adcEtcTriggerConfig);//设置外部XBAR触发器配置
  
  
  
  /*配置外部触发转换通道组具体的ADC转换通道*/
  /*****************************************************************************************************************************/
  adcEtcTriggerChainConfig.enableB2BMode = true;                               //使用B2B模式
  adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U<< DEMO_ADC_CHANNEL_GROUP0; //选择要参与转换的ADC转换通道
  adcEtcTriggerChainConfig.ADCChannelSelect = DEMO_ADC_ETC_CHANNLE_NUMBER_0;                               // 选择外部输入通道
  adcEtcTriggerChainConfig.InterruptEnable = kADC_ETC_Done0InterruptEnable;    // 使能该通道的转换完成中断0 
  ADC_ETC_SetTriggerChainConfig(DEMO_ADC_ETC_BASE, ADC_ETC_XBARA_TRIGGER_CHANNELx, 0U, &adcEtcTriggerChainConfig); //
 /*****************************************************************************************************************************/


  /****************************************************************************************************************************/                             
  adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U<< DEMO_ADC_CHANNEL_GROUP1;         //选择要参与转换的ADC转换通道
  adcEtcTriggerChainConfig.ADCChannelSelect = DEMO_ADC_ETC_CHANNLE_NUMBER_15;                                      //选择外部输入通道
  adcEtcTriggerChainConfig.InterruptEnable = kADC_ETC_Done1InterruptEnable;            //使能当前通道转换完成中断1.
  ADC_ETC_SetTriggerChainConfig(DEMO_ADC_ETC_BASE, ADC_ETC_XBARA_TRIGGER_CHANNELx, 1U,&adcEtcTriggerChainConfig);  
  /********************************************************************************************************************************/

  
  /*设置中断优先级,*/
  set_IRQn_Priority(ADC_ETC_IRQ1_IRQn,Group4_PreemptPriority_6, Group4_SubPriority_0);
  set_IRQn_Priority(ADC_ETC_IRQ0_IRQn,Group4_PreemptPriority_6, Group4_SubPriority_1);
  
  /*使能中断 NVIC. */
  EnableIRQ(ADC_ETC_IRQ1_IRQn);
  EnableIRQ(ADC_ETC_IRQ0_IRQn);

}



/**
  * @brief  初始化控制ADC和ADC_ETC
  * @param  无
  * @retval 无
  */
void ADC_Config(void)
{
  
  ADC_IOMUXC_MUX_Config();//设置引脚功能
  ADC_IOMUXC_PAD_Config();//设置引脚配置参数，驱动强度、转换速率等
  ADC_IO_Mode_Config();   //设置引脚模式，输入/输出，是否开启中断等
  ADC_Mode_Config();      //初始化ADC工作模式，并且进行硬件校准。
  
  ADC_ETC_Config();       //配置外部触发控制器

}

/*中断服务函数*/
void EXAMPLE_ADC_ETC_DONE0_Handler(void)
{
  /*清除转换完成中断标志位*/
  ADC_ETC_ClearInterruptStatusFlags(DEMO_ADC_ETC_BASE, (adc_etc_external_trigger_source_t)ADC_ETC_XBARA_TRIGGER_CHANNELx, kADC_ETC_Done0StatusFlagMask);
  /*读取转换结果*/
  g_AdcConversionValue0 = ADC_ETC_GetADCConversionValue(DEMO_ADC_ETC_BASE, ADC_ETC_XBARA_TRIGGER_CHANNELx, 0U); /* Get trigger0 chain0 result. */
  b_Value0_Conversion_complete_flag = true;
}

void EXAMPLE_ADC_ETC_DONE1_Handler(void)
{
  /*清除转换完成中断标志位*/
  ADC_ETC_ClearInterruptStatusFlags(DEMO_ADC_ETC_BASE, (adc_etc_external_trigger_source_t)ADC_ETC_XBARA_TRIGGER_CHANNELx, kADC_ETC_Done1StatusFlagMask);
  /*读取转换结果*/
  g_AdcConversionValue1 = ADC_ETC_GetADCConversionValue(DEMO_ADC_ETC_BASE, ADC_ETC_XBARA_TRIGGER_CHANNELx, 1U); /* Get trigger0 chain1 result. */ 
  b_Value1_Conversion_complete_flag = true;
}










