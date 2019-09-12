#ifndef __BSP_GPT_H
#define __BSP_GPT_H

#include "fsl_common.h"
#include "fsl_gpt.h"


/* 选择外部时钟源 */
#define EXAMPLE_GPT_CLOCK_SOURCE_SELECT (0U)
/* 时钟源的时钟分频 */
#define EXAMPLE_GPT_CLOCK_DIVIDER_SELECT (0U)

/*GPT 时钟分频(1-4096)*/
#define GPT_DIVIDER 100

/* 得到GPT定时器的计数频率*/
#define EXAMPLE_GPT_CLK_FREQ ( (CLOCK_GetFreq(kCLOCK_IpgClk) / (EXAMPLE_GPT_CLOCK_DIVIDER_SELECT + 1U))/GPT_DIVIDER )
#define COUNGER_NUMBER  ((TIME_ms/1000.0)*EXAMPLE_GPT_CLK_FREQ)
#define TIME_ms 1000

/*定义使用的GPT*/
#define EXAMPLE_GPT GPT2

/*中断号和中断服务函数宏定义*/
#define GPT_IRQ_ID GPT2_IRQn
#define EXAMPLE_GPT_IRQHandler GPT2_IRQHandler

void GPT_Config(void);


#endif /* __BSP_GPT_H */
