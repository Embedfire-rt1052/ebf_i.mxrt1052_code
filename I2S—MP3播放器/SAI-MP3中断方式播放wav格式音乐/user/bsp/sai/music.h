/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MUSIC_H_
#define _MUSIC_H_

#if defined(__GNUC__) /* GNU Compiler */
#ifndef __ALIGN_END
#define __ALIGN_END __attribute__((aligned(4)))
#endif
#ifndef __ALIGN_BEGIN
#define __ALIGN_BEGIN
#endif
#else
#ifndef __ALIGN_END
#define __ALIGN_END
#endif
#ifndef __ALIGN_BEGIN
#if defined(__CC_ARM) || defined(__ARMCC_VERSION) /* ARM Compiler */
#define __ALIGN_BEGIN __attribute__((aligned(4)))
#elif defined(__ICCARM__) /* IAR Compiler */
#define __ALIGN_BEGIN
#endif
#endif
#endif

#if defined(__ICCARM__)
#pragma data_alignment = 4
#endif
__ALIGN_BEGIN int a __ALIGN_END;

#define MUSIC_LEN (3000)

#if defined FSL_FEATURE_SOC_MMDC_COUNT && FSL_FEATURE_SOC_MMDC_COUNT
AT_NONCACHEABLE_SECTION_ALIGN_INIT(uint8_t music[], 4) =
#else
__ALIGN_BEGIN volatile  uint8_t music1[MUSIC_LEN] __ALIGN_END;
__ALIGN_BEGIN volatile  uint8_t music2[MUSIC_LEN] __ALIGN_END;
#endif

       

#endif
