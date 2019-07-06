#ifndef __BSP_WAKEUP_BUTTON_H
#define __BSP_WAKEUP_BUTTON_H

#include "fsl_iomuxc.h"
#include "fsl_common.h"
#include "fsl_gpt.h"

#define WAKEUP_BUTTON_GPIO GPIO5
#define WAKEUP_BUTTON_GPIO_PIN (0U)

#define WAKEUP_BUTTON_IRQ GPIO5_Combined_0_15_IRQn
#define WAKEUP_BUTTON_IRQ_HANDLER GPIO5_Combined_0_15_IRQHandler
#define WAKEUP_BUTTON_NAME "WAKEUP"



#endif /* _BSP_WAKEUP_BUTTON  */


