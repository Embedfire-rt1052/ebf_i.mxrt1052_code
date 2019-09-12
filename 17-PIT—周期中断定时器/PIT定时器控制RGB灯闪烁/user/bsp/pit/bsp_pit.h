#ifndef __BSP_PIT_H
#define __BSP_PIT_H

#include "fsl_common.h"
#include "fsl_pit.h"

/*定时器时钟频率宏定义*/
#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_OscClk) 

/*中断宏定义*/
#define PIT_IRQ_ID PIT_IRQn
#define PIT_LED_HANDLER PIT_IRQHandler

/*定时器通道宏定义*/
#define PIT_CHANNEL_X kPIT_Chnl_0

/*计时时间宏定义（单位：us)*/
#define TIME_0 1000000U

void PIT_TIMER_Init(void);


#endif /* __BSP_PIT_H */

