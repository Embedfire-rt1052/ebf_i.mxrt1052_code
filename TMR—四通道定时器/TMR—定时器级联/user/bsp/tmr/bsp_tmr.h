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


/*���嶨ʱʱ�䣨��λ��ms��*
*ע��:TMR��ʱ���ļ����Ĵ�����16λ��ʹ��kCLOCK_IpgClkʱ�ӣ�ѡ�����ʱ�ӷ�Ƶ��128��Ƶ��
*���ʵ��63ms�Ķ�ʱ��
*/
#define TMR_TIMIER 50

void TMR_Init(void);


#endif /* __BSP_TMR_H */





