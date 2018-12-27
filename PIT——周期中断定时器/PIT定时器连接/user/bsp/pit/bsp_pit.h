#ifndef __BSP_PIT_H
#define __BSP_PIT_H

#include "fsl_common.h"
#include "fsl_pit.h"

/*定时器时钟频率宏定义*/
#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_OscClk)

/*中断宏定义*/
#define PIT_LED_HANDLER PIT_IRQHandler
#define PIT_IRQ_ID PIT_IRQn

/*定时器通道宏定义*/
#define PIT_CHANNEL_X kPIT_Chnl_0

#define PIT_CHANNEL_0 kPIT_Chnl_0
#define PIT_CHANNEL_1 kPIT_Chnl_1
#define PIT_CHANNEL_2 kPIT_Chnl_2
#define PIT_CHANNEL_3 kPIT_Chnl_3

/*计时时间宏定义（单位：us)*/
#define TIME_x 1000000U
#define TIME_0 1000000U
#define TIME_1 1000000U
#define TIME_2 1000000U
#define TIME_3 1000000U

/*定义自动重装载值*/
#define TIME_2_COUNT 9


void PIT_TIMER_Init(void);



#endif /* __BSP_PIT_H */

