#ifndef __BSP_PIT_H
#define __BSP_PIT_H

#include "fsl_common.h"
#include "fsl_pit.h"
#include "fsl_clock.h"


#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_OscClk)
#define PIT_BASE_ADDR  PIT
#define PIT_CHANNEL_NUMBER kPIT_Chnl_0
#define PIT_TIMER_us 1000000
// 

void PIT_Configuration(void);

void PIT_start_timer(void);
#endif /* __BSP_PIT_H */

