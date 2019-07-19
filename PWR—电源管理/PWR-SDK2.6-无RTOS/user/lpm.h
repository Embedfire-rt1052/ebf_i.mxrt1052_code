/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _LPM_H_
#define _LPM_H_

#include <stdint.h>
#include "fsl_clock.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define CLOCK_CCM_HANDSHAKE_WAIT() \
                                   \
    do                             \
    {                              \
        while (CCM->CDHIPR != 0)   \
        {                          \
        }                          \
                                   \
    } while (0)

/* 低功耗管理的功耗模式定义。
 * 唤醒持续时间关闭> Dsm>空闲>等待>运行。
 */
typedef enum _lpm_power_mode
{
    LPM_PowerModeOverRun = 0, /* 在RUN模式下，CPU不会停止运行 */

    LPM_PowerModeFullRun, /* 完全运行模式，CPU不会停止运行*/

    LPM_PowerModeLowSpeedRun,

    LPM_PowerModeLowPowerRun,

    LPM_PowerModeRunEnd = LPM_PowerModeLowPowerRun,
    /* In system wait mode, cpu clock is gated.
     * All peripheral can remain active, clock gating decided by CCGR setting.
     * DRAM enters auto-refresh mode when there is no access.
     */
		/*
			译文：在系统等待模式下，cpu时钟被门控。
      *所有外设都可以保持活动状态，时钟门控由CCGR设置决定。
      *当没有访问权限时，DRAM进入自动刷新模式。
		*/
    LPM_PowerModeSysIdle, /* 系统等待模式，也是系统低速空闲 */

    /* In low power idle mode, all PLL/PFD is off, cpu power is off.
     * Analog modules running in low power mode.
     * All high-speed peripherals are power gated
     * Low speed peripherals can remain running at low frequency
     * DRAM in self-refresh.
     */
		 /*
			译文：在低功耗空闲模式下，所有PLL / PFD都关闭，cpu电源关闭。
      *模拟模块在低功耗模式下运行。
      *所有高速外设都是电源门控
      *低速外设可以保持低频运行
      * DRAM自刷新。
		 */
    LPM_PowerModeLPIdle, /* 低功耗空闲模式 */

    /* In deep sleep mode, all PLL/PFD is off, XTAL is off, cpu power is off.
     * All clocks are shut off except 32K RTC clock
     * All high-speed peripherals are power gated
     * Low speed peripherals are clock gated
     * DRAM in self-refresh.
     * If RTOS is used, systick will be disabled in DSM
     */
		 /*
		 译文：在深度睡眠模式下，所有PLL / PFD都关闭，XTAL关闭，cpu电源关闭。
      *除32K RTC时钟外，所有时钟均关闭
      *所有高速外设都是电源门控
      *低速外设是时钟门控的
      * DRAM自刷新。
      *如果使用RTOS，将在DSM中禁用systick
		 */
    LPM_PowerModeSuspend, /* 深度睡眠模式，暂停。 */

    LPM_PowerModeSNVS, /*电源关闭模式或关机模式 */

    LPM_PowerModeEnd = LPM_PowerModeSNVS
} lpm_power_mode_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

AT_QUICKACCESS_SECTION_CODE(void CLOCK_SET_MUX(clock_mux_t mux, uint32_t value));
AT_QUICKACCESS_SECTION_CODE(void CLOCK_SET_DIV(clock_div_t divider, uint32_t value));
void ClockSelectXtalOsc(void);
void ClockSelectRcOsc(void);
void LPM_Init(void);
void LPM_EnableWakeupSource(uint32_t irq);
void LPM_DisableWakeupSource(uint32_t irq);
void LPM_PreEnterWaitMode(void);
void LPM_PostExitWaitMode(void);
void LPM_PreEnterStopMode(void);
void LPM_PostExitStopMode(void);
void LPM_OverDriveRun(void);
void LPM_FullSpeedRun(void);
void LPM_LowSpeedRun(void);
void LPM_LowPowerRun(void);
void LPM_EnterSystemIdle(void);
void LPM_ExitSystemIdle(void);
void LPM_EnterLowPowerIdle(void);
void LPM_ExitLowPowerIdle(void);
void LPM_EnterSuspend(void);
void LPM_EnterSNVS(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _LPM_H_ */
