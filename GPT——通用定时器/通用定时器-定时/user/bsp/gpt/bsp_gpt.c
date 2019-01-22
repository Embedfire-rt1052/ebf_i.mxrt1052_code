#include "./bsp/nvic/bsp_nvic.h"
#include "./bsp/gpt/bsp_gpt.h"
#include "./bsp/led/bsp_led.h" 

unsigned int i = 0; //���ڿ���RGB�Ƶķ�ת
  
void GPT_Config(void)
{
  gpt_config_t gptConfig;
  
  /*����GPTʹ�õ����ⲿʱ��*/
  CLOCK_SetMux(kCLOCK_PerclkMux, EXAMPLE_GPT_CLOCK_SOURCE_SELECT);
  CLOCK_SetDiv(kCLOCK_PerclkDiv, EXAMPLE_GPT_CLOCK_DIVIDER_SELECT);
  
  /*��ʼ��GPT*/
  GPT_GetDefaultConfig(&gptConfig);
  gptConfig.enableFreeRun = false;
  GPT_Init(EXAMPLE_GPT, &gptConfig);
  
  /* ����ʱ�ӷ�Ƶ */
  GPT_SetClockDivider(EXAMPLE_GPT, GPT_DIVIDER);
  
  /* ����GPT��ʱ���ļ���ֵ*/
  GPT_SetOutputCompareValue(EXAMPLE_GPT, kGPT_OutputCompare_Channel1, (uint32_t)COUNGER_NUMBER);

  /* ����GPT2��ʱ���Ƚ�ͨ��1���ж� */
  GPT_EnableInterrupts(EXAMPLE_GPT, kGPT_OutputCompare1InterruptEnable);

   /*�����ж����ȼ�,*/
  set_IRQn_Priority(GPT_IRQ_ID,Group4_PreemptPriority_6, Group4_SubPriority_0);
  /*ʹ���ж�*/
  EnableIRQ(GPT_IRQ_ID);

  /* ������ʱ�� */
  GPT_StartTimer(EXAMPLE_GPT);
}



/*�����жϷ�����*/
void EXAMPLE_GPT_IRQHandler(void)
{
  /* ����жϱ�־λ.*/
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










