#include "./tmr/bsp_tmr.h"


volatile bool qtmrIsrFlag = false;


void TMR_Init(void)
{

  qtmr_config_t qtmrConfig; /*����TMR ��ʱ����ʼ���ṹ��*/
  
  /*��ʼ��TMR ��ʱ��*/
  QTMR_GetDefaultConfig(&qtmrConfig);
  
  /*��ʼ����һ��ͨ��*/
  qtmrConfig.primarySource = kQTMR_ClockDivide_128;
  QTMR_Init(BOARD_QTMR_BASEADDR, BOARD_FIRST_QTMR_CHANNEL, &qtmrConfig);
  
  /* ��ʼ���ڶ���ͨ�� */
  qtmrConfig.primarySource = QTMR_ClockCounterOutput;
  QTMR_Init(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, &qtmrConfig);
  
  /* ����ͨ1�ļ�ʱ����Ϊ1ms */
  QTMR_SetTimerPeriod(BOARD_QTMR_BASEADDR, BOARD_FIRST_QTMR_CHANNEL, MSEC_TO_COUNT(1U, (QTMR_SOURCE_CLOCK / 128)));
  
  /* ����ͨ��2�ļ���ֵΪ */
  QTMR_SetTimerPeriod(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, TMR_TIMIER);
  
  
  /*ʹ��ͨ��2�ıȽ��ж� Enable the second channel compare interrupt */
  QTMR_EnableInterrupts(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, kQTMR_CompareInterruptEnable);

  /* ��cascaseģʽ������ͨ��2�����ӵ�ǰ�����õ�ͨ��1.ͨ��2������ʱ����ͨ��1�ǴӶ�ʱ��*/
  QTMR_StartTimer(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, kQTMR_CascadeCount);

  /* ����ͨ��1����������λ��ʱ�ӵ������ؼ���*/
  QTMR_StartTimer(BOARD_QTMR_BASEADDR, BOARD_FIRST_QTMR_CHANNEL, kQTMR_PriSrcRiseEdge);

  /*�����ж� */
  EnableIRQ(QTMR_IRQ_ID);
}


/*TMR��ʱ���жϷ�����*/
void QTMR_IRQ_HANDLER(void)
{
    /* ����жϱ�־λ*/
    QTMR_ClearStatusFlags(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, kQTMR_CompareFlag);
    
    qtmrIsrFlag = true;
  
}








