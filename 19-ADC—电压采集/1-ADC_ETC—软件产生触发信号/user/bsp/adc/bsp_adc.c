
#include "fsl_debug_console.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"  
#include "fsl_adc.h"
  
#include "pad_config.h"  
#include "./adc/bsp_adc.h" 


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
//  IOMUXC_SetPinMux(CORE_BOARD_ADC_IOMUXC_CH15, 0U);
}

/**
* @brief  初始化ADC相关IOMUXC的MUX复用配置
* @param  无
* @retval 无
*/
static void ADC_IOMUXC_PAD_Config(void)
{
  IOMUXC_SetPinConfig(CORE_BOARD_ADC_IOMUXC_CH0, ADC_PAD_CONFIG_DATA);  
//  IOMUXC_SetPinConfig(CORE_BOARD_ADC_IOMUXC_CH15, ADC_PAD_CONFIG_DATA);   
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
}


 /**
  * @brief  初始化ADC工作模式
  * @param  无
  * @retval 无
  */
static void ADC_Mode_Config(void)
{
  adc_config_t adcConfigStrcut;                //定义ADC 模式配置结构体

  /*配置工作模式*/
  ADC_GetDefaultConfig(&adcConfigStrcut);         //获取ADC 默认工作模式
  adcConfigStrcut.resolution = kADC_Resolution12Bit;
  ADC_Init(ADCx, &adcConfigStrcut);               //配置ADC工作模式
  
  
  ADC_EnableHardwareTrigger(ADCx, true);          //使能硬件触发模式 
 
  
  /*进行硬件校准*/
  while(kStatus_Success != ADC_DoAutoCalibration(ADCx))
  {
    PRINTF("校准失败\r\n");
  }
  PRINTF("校准完成 Done.\r\n");
  
  
}

/*配置为允许外部触发*/
void ADC_ETC_Config(void)
{
  adc_etc_config_t adcEtcConfig;//ADC_ETC 配置
  adc_etc_trigger_config_t adcEtcTriggerConfig; //ADC_ETC 外部触发配置
  adc_etc_trigger_chain_config_t adcEtcTriggerChainConfig; // ADC_ETC 触发链配置
  
  ADC_ETC_GetDefaultConfig(&adcEtcConfig);
  adcEtcConfig.XBARtriggerMask = 1U; /* Enable the external XBAR trigger0.允许外部触发*/
  ADC_ETC_Init(DEMO_ADC_ETC_BASE, &adcEtcConfig);
  
  
  /* 
   *Set the external XBAR trigger0 configuration. 
   *配置ADC外部触发源属性。
  */
  adcEtcTriggerConfig.enableSyncMode = false;                        //使能同步模式，同步：ADC1和ADC2被相同的触发源控制
  adcEtcTriggerConfig.enableSWTriggerMode = true;                    //使能软件触发模式
  adcEtcTriggerConfig.triggerChainLength = DEMO_ADC_ETC_CHAIN_LENGTH; /* Chain length is 1.设置有多少个ADC通道参与转换 */
  adcEtcTriggerConfig.triggerPriority = 0U;                          //外部触发优先级
  adcEtcTriggerConfig.sampleIntervalDelay = 0U;                      //设置采样时间间隔                
  adcEtcTriggerConfig.initialDelay = 0U;                             //设置触发器初始延时
  //DEMO_ADC_ETC_BASE 触发器基地址，触发器组 触发器配置
  ADC_ETC_SetTriggerConfig(DEMO_ADC_ETC_BASE, 0U, &adcEtcTriggerConfig);//设置外部XBAR触发器配置
  
  
  
  /*将转换通道指定到chainGroup */
  adcEtcTriggerChainConfig.enableB2BMode = true;
  adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U<< DEMO_ADC_CHANNEL_GROUP0;       //选择要触发的ADC_HC1寄存器
  
  
  adcEtcTriggerChainConfig.ADCChannelSelect = DEMO_ADC_ETC_CHANNEL1;                   // ADC采样通道
  adcEtcTriggerChainConfig.InterruptEnable = kADC_ETC_Done0InterruptEnable;            // 使能该通道的转换完成中断，这样的中断总共有三个. 
  ADC_ETC_SetTriggerChainConfig(DEMO_ADC_ETC_BASE, 0U, 1U, &adcEtcTriggerChainConfig); //配置触发chain1.  在前面定义了 chainl 的数量为1
  
 /*****************************************************************************************************************************/


  /* 使能中断. */
  EnableIRQ(ADC_ETC_IRQ0_IRQn);


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
  ADC_IO_Mode_Config();   //设置引脚模式，输入/输出，是否开启中断等
  ADC_Mode_Config();      //初始化ADC工作模式，并且进行硬件校准。
  
  ADC_ETC_Config();

}

/*中断服务函数*/
void EXAMPLE_ADC_ETC_DONE0_Handler(void)
{
  ADC_ETC_ClearInterruptStatusFlags(DEMO_ADC_ETC_BASE, kADC_ETC_Trg0TriggerSource, kADC_ETC_Done0StatusFlagMask);
  g_AdcConversionValue0 = ADC_ETC_GetADCConversionValue(DEMO_ADC_ETC_BASE, 0U, 0U); /* Get trigger0 chain0 result. */
  b_Value0_Conversion_complete_flag = true;
}



