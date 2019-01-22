#include "./bsp/nvic/bsp_nvic.h"
#include "./bsp/gpt/bsp_gpt.h"
#include "./bsp/led/bsp_led.h" 

unsigned int i = 0; //用于控制RGB灯的翻转
  
void GPT_Config(void)
{
  gpt_config_t gptConfig;
  
  /*设置GPT使用到的外部时钟*/
  CLOCK_SetMux(kCLOCK_PerclkMux, EXAMPLE_GPT_CLOCK_SOURCE_SELECT);
  CLOCK_SetDiv(kCLOCK_PerclkDiv, EXAMPLE_GPT_CLOCK_DIVIDER_SELECT);
  
  /*初始化GPT*/
  GPT_GetDefaultConfig(&gptConfig);
  gptConfig.enableFreeRun = false;
  GPT_Init(EXAMPLE_GPT, &gptConfig);
  
  /* 设置时钟分频 */
  GPT_SetClockDivider(EXAMPLE_GPT, GPT_DIVIDER);
  
  /* 设置GPT定时器的计数值*/
  GPT_SetOutputCompareValue(EXAMPLE_GPT, kGPT_OutputCompare_Channel1, (uint32_t)COUNGER_NUMBER);

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










