#ifndef __BSP_PIT_H
#define __BSP_PIT_H

#include "fsl_common.h"
#include "fsl_pit.h"
#include "fsl_clock.h"

/* PIT���ò����궨�� */
#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_OscClk) //�õ�PIT��ʱ��Ƶ��
#define PIT_BASE_ADDR  PIT                            //PIT ��ַ
#define PIT_CHANNEL_NUMBER kPIT_Chnl_0                //PIT��ʱ����ͨ������
#define PIT_TIMER_us 500000                          //�趨PIT��ʱ���ļ�ʱ����


void PIT_Configuration(void);
void PIT_start_timer(void);
#endif /* __BSP_PIT_H */

