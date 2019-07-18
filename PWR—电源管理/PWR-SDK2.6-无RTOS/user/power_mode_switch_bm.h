/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _POWER_MODE_SWITCH_H_
#define _POWER_MODE_SWITCH_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef enum _app_wakeup_source
{
    kAPP_WakeupSourceGPT, /*!< PIT���ѡ�        */
    kAPP_WakeupSourcePin, /*!<ͨ���ⲿ���Ż��ѡ� */
} app_wakeup_source_t;

#endif /* _POWER_MODE_SWITCH_H_ */
