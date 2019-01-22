#include "fsl_gpio.h"
#include "pad_config.h"

#include "./bsp/nvic/bsp_nvic.h"
#include "./bsp/gpt/bsp_gpt.h"
#include "./bsp/led/bsp_led.h" 

unsigned int i = 0; //用于控制RGB灯的翻转
  
  
/**
* @brief  配置GPT相关引脚功能
* @param  无
* @retval 无
*/
void GPT_GPIO_Config(void)
{
  /*定义GPIO引脚配置结构体*/
  gpio_pin_config_t gpt_config;
  
  IOMUXC_SetPinMux(GPT2_COMPARE1_IOMUXC, 0U);
  IOMUXC_SetPinConfig(GPT2_COMPARE1_IOMUXC, GPT_COMPARE_PAD_CONFIG_DATA);
  
  gpt_config.direction = kGPIO_DigitalOutput; //输出模式
  gpt_config.outputLogic =  0;                //默认高电平
  gpt_config.interruptMode = kGPIO_NoIntmode; //不使用中断
  
  /* 初始化 GPT COMPARE1 GPIO. */
  GPIO_PinInit(GPT2_COMPARE1_GPIO, GPT2_COMPARE1_GPIO_PIN, &gpt_config);
  
}

  
  
/**
* @brief  配置GPT工作模式
* @param  无
* @retval 无
*/  
void GPT_Config(void)
{
  uint32_t gptFreq;
  gpt_config_t gptConfig;
  
  /*初始化GPT引脚*/
  GPT_GPIO_Config();
  
  /*GPT的时钟设置*/
  CLOCK_SetMux(kCLOCK_PerclkMux, EXAMPLE_GPT_CLOCK_SOURCE_SELECT);
  CLOCK_SetDiv(kCLOCK_PerclkDiv, EXAMPLE_GPT_CLOCK_DIVIDER_SELECT);
  
  /*初始化GPT*/
  GPT_GetDefaultConfig(&gptConfig);
  GPT_Init(EXAMPLE_GPT, &gptConfig);
  
  
  /* 设置时钟分频 */
  GPT_SetClockDivider(EXAMPLE_GPT, 1);
  
   /* 得GPT定时器时钟源频率 */
  gptFreq = EXAMPLE_GPT_CLK_FREQ;



  /* 设置GPT定时器的计数值*/
  GPT_SetOutputCompareValue(EXAMPLE_GPT, kGPT_OutputCompare_Channel1, gptFreq);

  /*设置输出模式*/
  GPT_SetOutputOperationMode(EXAMPLE_GPT,kGPT_OutputCompare_Channel1,kGPT_OutputOperation_Toggle);
  /* 开启GPT2定时器比较通道1的中断 */
  GPT_EnableInterrupts(EXAMPLE_GPT, kGPT_OutputCompare1InterruptEnable);

  
  /*设置中断优先级,*/
  set_IRQn_Priority(GPT_IRQ_ID,Group4_PreemptPriority_6, Group4_SubPriority_0);
  /*使能中断*/
  EnableIRQ(GPT_IRQ_ID);

  /* 开启定时器 */
  GPT_StartTimer(EXAMPLE_GPT);
}







/*定义中断服务函数*/
void EXAMPLE_GPT_IRQHandler(void)
{
  /* 清除中断标志位.*/
  GPT_ClearStatusFlags(EXAMPLE_GPT, kGPT_OutputCompare1Flag);
  i++;
  
  if(i%2)
  {
     RGB_BLUE_LED_OFF;
  }
  else
  {
     RGB_BLUE_LED_ON;
  } 
}

