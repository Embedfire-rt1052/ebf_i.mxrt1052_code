#ifndef _BSP_POWER_MODE_SWITCH_H_
#define _BSP_POWER_MODE_SWITCH_H_

#include "fsl_common.h"
#include "bsp_power_mode_switch_bm.h"
#include "bsp_lpm.h"

/* GPIO相关宏定义 */
#define CPU_NAME "iMXRT1052"
#define APP_WAKEUP_BUTTON_GPIO BOARD_USER_BUTTON_GPIO
#define APP_WAKEUP_BUTTON_GPIO_PIN BOARD_USER_BUTTON_GPIO_PIN
#define APP_WAKEUP_BUTTON_IRQ BOARD_USER_BUTTON_IRQ
#define APP_WAKEUP_BUTTON_IRQ_HANDLER BOARD_USER_BUTTON_IRQ_HANDLER
#define APP_WAKEUP_BUTTON_NAME BOARD_USER_BUTTON_NAME
/* GPT相关宏定义 */
#define APP_WAKEUP_GPT_BASE GPT2    
#define APP_WAKEUP_GPT_IRQn GPT2_IRQn
#define APP_WAKEUP_GPT_IRQn_HANDLER GPT2_IRQHandler
/* 唤醒源枚举 */
typedef enum _app_wakeup_source
{
    kAPP_WakeupSourceGPT, /*!< GPT唤醒。        */
    kAPP_WakeupSourcePin, /*!<通过外部引脚唤醒。 */
} app_wakeup_source_t;
/* 外部调用 */
extern uint8_t s_wakeupTimeout;            /* 唤醒超时。 （单位：秒）*/
extern app_wakeup_source_t s_wakeupSource; /*唤醒来源。                 */
extern lpm_power_mode_t s_targetPowerMode;
extern lpm_power_mode_t s_curRunMode;
/* 外部调用 */
extern void APP_ShowPowerMode(lpm_power_mode_t powerMode);
extern bool APP_CheckPowerMode(lpm_power_mode_t originPowerMode, lpm_power_mode_t targetPowerMode);
extern void APP_GetWakeupConfig(lpm_power_mode_t targetMode);
extern void APP_SetWakeupConfig(lpm_power_mode_t targetMode);
extern void APP_PowerPostSwitchHook(lpm_power_mode_t targetMode);
extern void APP_PowerModeSwitch(lpm_power_mode_t targetPowerMode);
extern void APP_PowerPreSwitchHook(lpm_power_mode_t targetMode);

#endif /* _BSP_POWER_MODE_SWITCH_H_ */
