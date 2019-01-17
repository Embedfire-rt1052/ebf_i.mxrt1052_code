#include "./encoder/bsp_encoder.h"
#include "./led/bsp_led.h"  


volatile uint64_t interrupt_counter = 0;


void Quadrature_Count_Init(void)
{

  qtmr_config_t qtmrConfig; /*����TMR ��ʱ����ʼ���ṹ��*/
  
  /*��ʼ��TMR ��ʱ��*/
  QTMR_GetDefaultConfig(&qtmrConfig);
  qtmrConfig.primarySource = kQTMR_ClockDivide_128;
  QTMR_Init(QTMR_BASEADDR_2, QTMR_CH0, &qtmrConfig);
  
  /*�����Զ���װ��ֵ*/
  QTMR_SetTimerPeriod(QTMR_BASEADDR_2, QTMR_CH0, MSEC_TO_COUNT(TMR_TIMIER, (QTMR_SOURCE_CLOCK / 128)));

  /*ʹ���ж�*/
  EnableIRQ(QTMR2_IRQ_ID);
  
  /*ʹ�ܱȽ��ж�*/
  QTMR_EnableInterrupts(QTMR_BASEADDR_2, QTMR_CH0, kQTMR_CompareInterruptEnable);
  
  /*����ͨ��2�ļ�ʱ����ʱ�ӵ������ؼ���*/
  QTMR_StartTimer(QTMR_BASEADDR_2, QTMR_CH0, kQTMR_PriSrcRiseEdge);

}


/*TMR��ʱ���жϷ�����*/
void QTMR2_IRQ_HANDLER(void)
{
  interrupt_counter++;
  
  /*ģ�������ʱ��*/
  if(interrupt_counter % 2)
  {
    if((interrupt_counter/2)%2)
    {
      RGB_RED_LED_OFF
    }
    else
    {
      RGB_RED_LED_ON
    }
  }
  else
  {
    if((interrupt_counter/2)%2)
    {
      RGB_GREEN_LED_ON
    }
    else
    {
      RGB_GREEN_LED_OFF
    }
  }
    /* ����жϱ�־λ*/
  QTMR_ClearStatusFlags(QTMR_BASEADDR_2,QTMR_CH0, kQTMR_CompareFlag);

}








