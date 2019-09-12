#ifndef __BSP_PIT_H
#define __BSP_PIT_H

#include "fsl_common.h"
#include "fsl_pit.h"
#include "fsl_clock.h"

/* PIT配置参数宏定义 */
#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_OscClk) //得到PIT的时钟频率
#define PIT_BASE_ADDR  PIT                            //PIT 基址
#define PIT_CHANNEL_NUMBER kPIT_Chnl_0                //PIT定时器的通道定义
#define PIT_TIMER_us 500000                          //设定PIT定时器的计时周期


void PIT_Configuration(void);
void PIT_start_timer(void);
#endif /* __BSP_PIT_H */

