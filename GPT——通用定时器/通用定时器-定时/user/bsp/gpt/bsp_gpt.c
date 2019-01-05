
#include "./gpt/bsp_gpt.h"
#include "./led/bsp_led.h" 

unsigned int i = 0; //用于控制RGB灯的翻转
  
void GPT_Config(void)
{
  uint32_t gptFreq;
  gpt_config_t gptConfig;
  
  /*GPT的时钟设置*/
  CLOCK_SetMux(kCLOCK_PerclkMux, EXAMPLE_GPT_CLOCK_SOURCE_SELECT);
  CLOCK_SetDiv(kCLOCK_PerclkDiv, EXAMPLE_GPT_CLOCK_DIVIDER_SELECT);
  
  /*初始化GPT*/
  GPT_GetDefaultConfig(&gptConfig);
  gptConfig.enableFreeRun = true;
  GPT_Init(EXAMPLE_GPT, &gptConfig);
  
  
  /* 设置时钟分频 */
  GPT_SetClockDivider(EXAMPLE_GPT, 1);
  
  
  
   /* 得GPT定时器时钟源频率 */
  gptFreq = EXAMPLE_GPT_CLK_FREQ;

  /* 得到GPT定时器计时频率*/
  gptFreq /= 3;

  /* 设置GPT定时器的计数值*/
  GPT_SetOutputCompareValue(EXAMPLE_GPT, kGPT_OutputCompare_Channel1, gptFreq);

  /* 开启GPT2定时器比较通道1的中断 */
  GPT_EnableInterrupts(EXAMPLE_GPT, kGPT_OutputCompare1InterruptEnable);

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

