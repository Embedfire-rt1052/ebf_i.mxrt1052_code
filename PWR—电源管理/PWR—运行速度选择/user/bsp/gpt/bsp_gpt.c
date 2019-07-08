#include "fsl_gpio.h"
#include "pad_config.h"

#include "./bsp/nvic/bsp_nvic.h"
#include "./bsp/gpt/bsp_gpt.h"
#include "./bsp/led/bsp_led.h"

/**
* @brief  ����GPT����ģʽ
* @param  ��
* @retval ��
*/
void GPT_Config(void)
{
    gpt_config_t gptConfig;

//    /*GPT��ʱ������*/
  CLOCK_SetMux(kCLOCK_PerclkMux, EXAMPLE_GPT_CLOCK_SOURCE_SELECT);
  CLOCK_SetDiv(kCLOCK_PerclkDiv, EXAMPLE_GPT_CLOCK_DIVIDER_SELECT);
//    CLOCK_SetMux(kCLOCK_PerclkMux, 1); /* ��PITʱ��Դ����ΪOSC 24M */
//    CLOCK_SetDiv(kCLOCK_PerclkDiv, 0); /* ��PITʱ�ӷ�Ƶ������Ϊ1 */

    /*��ʼ��GPT*/
    GPT_GetDefaultConfig(&gptConfig);
    gptConfig.clockSource = kGPT_ClockSource_LowFreq; /* 32K RTC OSC */
    // gptConfig.enableMode = false;                     /* ֹͣʱ���ּ����� */
    gptConfig.enableMode = true; /* ֹͣʱ��Ҫ���ּ����� */
    gptConfig.enableRunInDoze = true;
    /* ��ʼ��GPTģ�� */
    GPT_Init(WAKEUP_GPT_BASE, &gptConfig);
    /* ����ʱ�ӷ�Ƶ */
    GPT_SetClockDivider(WAKEUP_GPT_BASE, 1);
}


void SetCompValue()
{
//    /* ��GPT��ʱ��ʱ��ԴƵ�� */
//    gptFreq = EXAMPLE_GPT_CLK_FREQ;

//    /* ����GPT��ʱ���ļ���ֵ*/
//    GPT_SetOutputCompareValue(EXAMPLE_GPT, kGPT_OutputCompare_Channel1, gptFreq);

//    /*�������ģʽ*/
//    GPT_SetOutputOperationMode(EXAMPLE_GPT,kGPT_OutputCompare_Channel1,kGPT_OutputOperation_Toggle);
//    /* ����GPT2��ʱ���Ƚ�ͨ��1���ж� */
//    GPT_EnableInterrupts(EXAMPLE_GPT, kGPT_OutputCompare1InterruptEnable);

//    /*�����ж����ȼ�,*/
//    set_IRQn_Priority(GPT_IRQ_ID,Group4_PreemptPriority_6, Group4_SubPriority_0);
//    /*ʹ���ж�*/
//    EnableIRQ(GPT_IRQ_ID);

//    /* ������ʱ�� */
//    GPT_StartTimer(EXAMPLE_GPT);

}

///*�����жϷ�����*/
//void WAKEUP_GPT_IRQn_HANDLER(void)
//{
//  /* ����жϱ�־λ.*/
//  GPT_ClearStatusFlags(EXAMPLE_GPT, kGPT_OutputCompare1Flag);
//  i++;
//
//  if(i%2)
//  {
//     RGB_BLUE_LED_OFF;
//  }
//  else
//  {
//     RGB_BLUE_LED_ON;
//  }
//}

