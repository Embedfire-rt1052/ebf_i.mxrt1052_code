/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _POWER_MODE_SWITCH_H_
#define _POWER_MODE_SWITCH_H_

#include "fsl_common.h"
#include "lpm.h"
#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

// #define WAKEUP_GPT_BASE GPT2
// #define WAKEUP_GPT_IRQn GPT2_IRQn
// #define WAKEUP_GPT_IRQn_HANDLER GPT2_IRQHandler

typedef enum _wakeup_source
{
    kWakeupSourceGPT, /*!< Wakeup by PIT.        */
    kWakeupSourcePin, /*!< Wakeup by external pin. */
}wakeup_source_t;

static const char *s_modeNames[] = {"Over RUN",    "Full Run",       "Low Speed Run", "Low Power Run",
                                    "System Idle", "Low Power Idle", "Suspend",       "SNVS"
                                   };


#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

extern int32_t is_suspend_reset;

void PowerPreSwitchHook(lpm_power_mode_t targetMode);
void PowerPostSwitchHook(lpm_power_mode_t targetMode);
lpm_power_mode_t GetLPMPowerMode(void);
void BOARD_SetLPClockGate(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/
extern lpm_power_mode_t Fun_s_curRunMode(void);
extern void PowerModeSwitchTask(void);
extern void vPortPRE_SLEEP_PROCESSING();

#endif /* _POWER_MODE_SWITCH_H_ */
