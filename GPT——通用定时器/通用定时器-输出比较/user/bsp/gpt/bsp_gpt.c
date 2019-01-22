#include "fsl_gpio.h"
#include "pad_config.h"

#include "./bsp/nvic/bsp_nvic.h"
#include "./bsp/gpt/bsp_gpt.h"
#include "./bsp/led/bsp_led.h" 

unsigned int i = 0; //���ڿ���RGB�Ƶķ�ת
  
  
/**
* @brief  ����GPT������Ź���
* @param  ��
* @retval ��
*/
void GPT_GPIO_Config(void)
{
  /*����GPIO�������ýṹ��*/
  gpio_pin_config_t gpt_config;
  
  IOMUXC_SetPinMux(GPT2_COMPARE1_IOMUXC, 0U);
  IOMUXC_SetPinConfig(GPT2_COMPARE1_IOMUXC, GPT_COMPARE_PAD_CONFIG_DATA);
  
  gpt_config.direction = kGPIO_DigitalOutput; //���ģʽ
  gpt_config.outputLogic =  0;                //Ĭ�ϸߵ�ƽ
  gpt_config.interruptMode = kGPIO_NoIntmode; //��ʹ���ж�
  
  /* ��ʼ�� GPT COMPARE1 GPIO. */
  GPIO_PinInit(GPT2_COMPARE1_GPIO, GPT2_COMPARE1_GPIO_PIN, &gpt_config);
  
}

  
  
/**
* @brief  ����GPT����ģʽ
* @param  ��
* @retval ��
*/  
void GPT_Config(void)
{
  uint32_t gptFreq;
  gpt_config_t gptConfig;
  
  /*��ʼ��GPT����*/
  GPT_GPIO_Config();
  
  /*GPT��ʱ������*/
  CLOCK_SetMux(kCLOCK_PerclkMux, EXAMPLE_GPT_CLOCK_SOURCE_SELECT);
  CLOCK_SetDiv(kCLOCK_PerclkDiv, EXAMPLE_GPT_CLOCK_DIVIDER_SELECT);
  
  /*��ʼ��GPT*/
  GPT_GetDefaultConfig(&gptConfig);
  GPT_Init(EXAMPLE_GPT, &gptConfig);
  
  
  /* ����ʱ�ӷ�Ƶ */
  GPT_SetClockDivider(EXAMPLE_GPT, 1);
  
   /* ��GPT��ʱ��ʱ��ԴƵ�� */
  gptFreq = EXAMPLE_GPT_CLK_FREQ;



  /* ����GPT��ʱ���ļ���ֵ*/
  GPT_SetOutputCompareValue(EXAMPLE_GPT, kGPT_OutputCompare_Channel1, gptFreq);

  /*�������ģʽ*/
  GPT_SetOutputOperationMode(EXAMPLE_GPT,kGPT_OutputCompare_Channel1,kGPT_OutputOperation_Toggle);
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

