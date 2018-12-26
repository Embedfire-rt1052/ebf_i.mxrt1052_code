#ifndef __BSP_PIT_H
#define __BSP_PIT_H

#include "fsl_common.h"
#include "fsl_pit.h"

#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_OscClk)
#define PIT_IRQ_ID PIT_IRQn
#define PIT_CHANNEL_X kPIT_Chnl_0
#define PIT_LED_HANDLER PIT_IRQHandler





void PIT_TIMER_Init(void);











#endif /* __BSP_PIT_H */

