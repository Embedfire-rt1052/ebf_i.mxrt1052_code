
#include "./gpt/bsp_gpt.h"
#include "./led/bsp_led.h" 

unsigned int i = 0; //���ڿ���RGB�Ƶķ�ת
  
void GPT_Config(void)
{
  uint32_t gptFreq;
  gpt_config_t gptConfig;
  
  /*GPT��ʱ������*/
  CLOCK_SetMux(kCLOCK_PerclkMux, EXAMPLE_GPT_CLOCK_SOURCE_SELECT);
  CLOCK_SetDiv(kCLOCK_PerclkDiv, EXAMPLE_GPT_CLOCK_DIVIDER_SELECT);
  
  /*��ʼ��GPT*/
  GPT_GetDefaultConfig(&gptConfig);
  gptConfig.enableFreeRun = true;
  GPT_Init(EXAMPLE_GPT, &gptConfig);
  
  
  /* ����ʱ�ӷ�Ƶ */
  GPT_SetClockDivider(EXAMPLE_GPT, 1);
  
  
  
   /* ��GPT��ʱ��ʱ��ԴƵ�� */
  gptFreq = EXAMPLE_GPT_CLK_FREQ;

  /* �õ�GPT��ʱ����ʱƵ��*/
  gptFreq /= 3;

  /* ����GPT��ʱ���ļ���ֵ*/
  GPT_SetOutputCompareValue(EXAMPLE_GPT, kGPT_OutputCompare_Channel1, gptFreq);

  /* ����GPT2��ʱ���Ƚ�ͨ��1���ж� */
  GPT_EnableInterrupts(EXAMPLE_GPT, kGPT_OutputCompare1InterruptEnable);

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

