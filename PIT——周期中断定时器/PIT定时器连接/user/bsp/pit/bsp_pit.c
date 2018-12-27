#include "./pit/bsp_pit.h"
#include "./led/bsp_led.h"


unsigned int  k = 0;
void PIT_TIMER_Init(void)
{
  pit_config_t pitConfig;//����PIT��ʼ���ṹ��
  
  /*���Ĭ�����ò���*/
  PIT_GetDefaultConfig(&pitConfig);
  /*��ʼ��PIT */
  PIT_Init(PIT, &pitConfig);
  
  /* ����PIT��ʱ��ͨ��2�Զ���װ��ֵ */
  PIT_SetTimerPeriod(PIT, PIT_CHANNEL_2, TIME_2_COUNT);
   
  /*���ͨ��2���жϱ�־λ*/
   PIT_ClearStatusFlags(PIT, PIT_CHANNEL_2, kPIT_TimerFlag);
  /* ʹ��ͨ��2�ļ�ʱ����ж� */
  PIT_EnableInterrupts(PIT, PIT_CHANNEL_2, kPIT_TimerInterruptEnable);
  
  /*PIT����Ϊ��ʱ������ģʽ*/
  PIT_SetTimerChainMode(PIT, PIT_CHANNEL_2 , 1);
  

  /* ����PIT��ʱ��ͨ��1�Զ���װ��ֵ */
  PIT_SetTimerPeriod(PIT, PIT_CHANNEL_1, USEC_TO_COUNT(TIME_1, PIT_SOURCE_CLOCK));
  
  


  /* ʹ��PIT��ʱ���ж� */
  EnableIRQ(PIT_IRQ_ID);
}



void PIT_LED_HANDLER(void)
{
  k++;
  
  /* ����жϱ�־λ.*/
  PIT_ClearStatusFlags(PIT, kPIT_Chnl_2, kPIT_TimerFlag);

  if(k%2)
  {
    RGB_RED_LED_ON;
  }
  else
  {
    RGB_RED_LED_OFF;
  }


}

