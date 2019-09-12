#ifndef __BSP_QUADRATURE_H
#define __BSP_QUADRATURE_H

#include "fsl_common.h"
#include "fsl_qtmr.h"


/* ���� QTMR ��ʱ����ͨ�� */
#define QTMR_BASEADDR_2 TMR2
#define QTMR_CH0 kQTMR_Channel_0



/* �жϺź��жϷ��������� */
#define QTMR2_IRQ_ID TMR2_IRQn
#define QTMR2_IRQ_HANDLER TMR2_IRQHandler

/* �õ�TMR��ʱ����ʱ��Ƶ�� */
#define QTMR_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_IpgClk)


/*���嶨ʱʱ�䣨��λ��ms��*
*ע��:TMR��ʱ���ļ����Ĵ�����16λ��ʹ��kCLOCK_IpgClkʱ�ӣ�ѡ�����ʱ�ӷ�Ƶ��128��Ƶ��
*���ʵ��63ms�Ķ�ʱ��
*/   
#define TMR_TIMIER 10


void Quadrature_Count_Init(void);

#endif /* __BSP_TMR_H */





