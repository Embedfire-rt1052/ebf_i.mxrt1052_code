#ifndef __BSP_GPT_H
#define __BSP_GPT_H

#include "fsl_iomuxc.h"
#include "fsl_common.h"
#include "fsl_gpt.h"


/* Ñ¡Ôñ IPG Clock as PERCLK_CLK clock source */
#define EXAMPLE_GPT_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for PERCLK_CLK clock source */
#define EXAMPLE_GPT_CLOCK_DIVIDER_SELECT (0U)


/* Get source clock for GPT driver (GPT prescaler = 0) */
#define EXAMPLE_GPT_CLK_FREQ (CLOCK_GetFreq(kCLOCK_IpgClk) / (EXAMPLE_GPT_CLOCK_DIVIDER_SELECT + 1U))



void GPT_GPIO_Config(void);
void GPT_Config(void);

#define WAKEUP_GPT_BASE GPT2
#define WAKEUP_GPT_IRQn GPT2_IRQn
#define WAKEUP_GPT_IRQn_HANDLER GPT2_IRQHandler


#endif /* __BSP_GPT_H */


