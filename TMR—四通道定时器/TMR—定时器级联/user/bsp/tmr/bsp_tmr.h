#ifndef __BSP_TMR_H
#define __BSP_TMR_H

#include "fsl_common.h"
#include "fsl_qtmr.h"


/* ���� TMR ��ʱ����ͨ���͹���ģʽinstance/channel used for board */
#define BOARD_QTMR_BASEADDR TMR3
#define BOARD_FIRST_QTMR_CHANNEL kQTMR_Channel_0
#define BOARD_SECOND_QTMR_CHANNEL kQTMR_Channel_1
#define QTMR_ClockCounterOutput kQTMR_ClockCounter0Output

/* �жϺź��жϷ��������� */
#define QTMR_IRQ_ID TMR3_IRQn
#define QTMR_IRQ_HANDLER TMR3_IRQHandler

/* �õ�TMR��ʱ����ʱ��Ƶ�� */
#define QTMR_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_IpgClk)


/*�趨��ʱ�¼�����λ��ms��*
*�ó���ʹ���˶�ʱ���ļ�������һ����ʱ����ͨ��1����ʱʱ��Ϊ1ms, ����������������ʱ����ͨ��2���ļ���ֵ��
*TMR_TIMIER = N ��ʾ��ʱN(ms) ,N����ȡ65535
*/
#define TMR_TIMIER 1000

void TMR_Init(void);


#endif /* __BSP_TMR_H */





