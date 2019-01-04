#include "./gpt/bsp_gpt.h"


void GPT_Config(void)
{
  uint32_t gptFreq;
  gpt_config_t gptConfig;
  
  CLOCK_SetMux(kCLOCK_PerclkMux, EXAMPLE_GPT_CLOCK_SOURCE_SELECT);
  CLOCK_SetDiv(kCLOCK_PerclkDiv, EXAMPLE_GPT_CLOCK_DIVIDER_SELECT);
  
  GPT_GetDefaultConfig(&gptConfig);
  GPT_Init(EXAMPLE_GPT, &gptConfig);
  
  
  /* Divide GPT clock source frequency by 3 inside GPT module */
  GPT_SetClockDivider(EXAMPLE_GPT, 3);
  
  
  
    /* Get GPT clock frequency */
  gptFreq = EXAMPLE_GPT_CLK_FREQ;

  /* GPT frequency is divided by 3 inside module */
  gptFreq /= 3;

  /* Set both GPT modules to 1 second duration */
  GPT_SetOutputCompareValue(EXAMPLE_GPT, kGPT_OutputCompare_Channel1, gptFreq);

  /* Enable GPT Output Compare1 interrupt */
  GPT_EnableInterrupts(EXAMPLE_GPT, kGPT_OutputCompare1InterruptEnable);

  /* Enable at the Interrupt */
  EnableIRQ(GPT_IRQ_ID);

  /* Start Timer */
 
  GPT_StartTimer(EXAMPLE_GPT);
  
  
}

/*定义中断服务函数*/
void EXAMPLE_GPT_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    GPT_ClearStatusFlags(EXAMPLE_GPT, kGPT_OutputCompare1Flag);

//    gptIsrFlag = true;
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F, Cortex-M7, Cortex-M7F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U || __CORTEX_M == 7U)
    __DSB();
#endif
}

