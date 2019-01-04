#ifndef __BSP_GPT_H
#define __BSP_GPT_H

#include "fsl_common.h"
#include "fsl_gpt.h"


/* Select IPG Clock as PERCLK_CLK clock source */
#define EXAMPLE_GPT_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for PERCLK_CLK clock source */
#define EXAMPLE_GPT_CLOCK_DIVIDER_SELECT (0U)


/* Get source clock for GPT driver (GPT prescaler = 0) */
#define EXAMPLE_GPT_CLK_FREQ (CLOCK_GetFreq(kCLOCK_IpgClk) / (EXAMPLE_GPT_CLOCK_DIVIDER_SELECT + 1U))

#define EXAMPLE_GPT GPT2

#define GPT_IRQ_ID GPT2_IRQn

void GPT_Config(void);


#endif /* __BSP_GPT_H */


