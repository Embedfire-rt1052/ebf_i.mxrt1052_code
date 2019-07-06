#ifndef __BSP_PWR_FREQ_H
#define	__BSP_PWR_FREQ_H
#include "fsl_common.h"
#include "lpm.h"
#include "board.h"

extern void PrintfCoreClock(void);
extern int32_t is_suspend_reset;


#define WAKEUP_BUTTON_GPIO BOARD_USER_BUTTON_GPIO
#define WAKEUP_BUTTON_GPIO_PIN BOARD_USER_BUTTON_GPIO_PIN
#define WAKEUP_BUTTON_IRQ BOARD_USER_BUTTON_IRQ
#define WAKEUP_BUTTON_IRQ_HANDLER BOARD_USER_BUTTON_IRQ_HANDLER
#define WAKEUP_BUTTON_NAME BOARD_USER_BUTTON_NAME

#define WAKEUP_GPT_BASE GPT2
#define WAKEUP_GPT_IRQn GPT2_IRQn
#define WAKEUP_GPT_IRQn_HANDLER GPT2_IRQHandler

#endif /* __BSP_PWR_FREQ_H */
