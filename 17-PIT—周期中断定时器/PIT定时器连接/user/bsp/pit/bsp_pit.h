#ifndef __BSP_PIT_H
#define __BSP_PIT_H

#include "fsl_common.h"
#include "fsl_pit.h"

/*��ʱ��ʱ��Ƶ�ʺ궨��*/
#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_OscClk)

/*�жϺ궨��*/
#define PIT_LED_HANDLER PIT_IRQHandler
#define PIT_IRQ_ID PIT_IRQn

/*��ʱ��ͨ���궨��*/
#define PIT_CHANNEL_X kPIT_Chnl_0

#define PIT_CHANNEL_0 kPIT_Chnl_0
#define PIT_CHANNEL_1 kPIT_Chnl_1
#define PIT_CHANNEL_2 kPIT_Chnl_2
#define PIT_CHANNEL_3 kPIT_Chnl_3

/*��ʱʱ��궨�壨��λ��us)*/
#define TIME_x 1000000U
#define TIME_0 1000000U
#define TIME_1 1000000U
#define TIME_2 1000000U
#define TIME_3 1000000U

/*�����Զ���װ��ֵ*/
#define TIME_2_COUNT 9


void PIT_TIMER_Init(void);



#endif /* __BSP_PIT_H */

