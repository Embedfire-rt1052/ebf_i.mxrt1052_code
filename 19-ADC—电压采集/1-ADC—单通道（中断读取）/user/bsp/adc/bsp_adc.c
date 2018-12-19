
#include "fsl_debug_console.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"  
#include "fsl_adc.h"
  
#include "pad_config.h"  
#include "./adc/bsp_adc.h" 

extern volatile bool g_AdcConversionDoneFlag;
extern volatile uint32_t g_AdcConversionValue;
extern volatile uint32_t g_AdcInterruptCounter;
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
  /*定义ADC通道配置结构体*/
  adc_channel_config_t adcChannelConfigStruct;
  
   /** ADC，GPIO配置 **/   
  adc_config.direction = kGPIO_DigitalInput; //输入模式
//  adc_config.outputLogic =  1;                //默认高电平
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

  
  ADC_GetDefaultConfig(&adcConfigStrcut); //获取ADC 默认工作模式
  ADC_Init(ADCx, &adcConfigStrcut); //配置ADC工作模式
  
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

 /**
  * @brief  启动转换，并打印转换结果
  * @param  无
  * @retval 无
  */
static void ADC_start(void)
{
  adc_channel_config_t adcChannelConfigStruct;//定义ADC 通道配置结构体
    /*配置用户通道*/
  adcChannelConfigStruct.channelNumber = DEMO_ADC_USER_CHANNEL;
  adcChannelConfigStruct.enableInterruptOnConversionCompleted = true;
  while (1)
  {
//      PRINTF("Press any key to get user channel's ADC value.\r\n");
//      GETCHAR();
      g_AdcConversionDoneFlag = false;
      /*
          When in software trigger mode, each conversion would be launched once calling the "ADC16_ChannelConfigure()"
          function, which works like writing a conversion command and executing it. For another channel's conversion,
          just to change the "channelNumber" field in channel configuration structure, and call the function
          "ADC_ChannelConfigure()"" again.
          Also, the "enableInterruptOnConversionCompleted" inside the channel configuration structure is a parameter
         for
          the conversion command. It takes affect just for the current conversion. If the interrupt is still required
          for the following conversion, it is necessary to assert the "enableInterruptOnConversionCompleted" every
         time
          for each command.
         */
      ADC_SetChannelConfig(ADCx, DEMO_ADC_CHANNEL_GROUP, &adcChannelConfigStruct);
      while (g_AdcConversionDoneFlag == false)
      {
       }
      PRINTF("ADC Value: %d\r\n", g_AdcConversionValue);
      //PRINTF("ADC Interrupt Counter: %d\r\n", g_AdcInterruptCounter);
  }
}


/**
  * @brief  初始化控制ADC的IO
  * @param  无
  * @retval 无
  */
void ADC_Config(void)
{
  ADC_IOMUXC_MUX_Config();
  ADC_IOMUXC_PAD_Config();
  ADC_IO_Mode_Config();
  
  /*开启中断*/
  EnableIRQ(ADC_IRQ);
}


void ADC_IRQHandler(void)
{
    g_AdcConversionDoneFlag = true;
    /* Read conversion result to clear the conversion completed flag. */
    g_AdcConversionValue = ADC_GetChannelConversionValue(ADCx, DEMO_ADC_CHANNEL_GROUP);
    //g_AdcInterruptCounter++;
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}