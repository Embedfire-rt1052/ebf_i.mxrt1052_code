/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _LPM_H_
#define _LPM_H_

#include "fsl_common.h"
#include "fsl_clock.h"
#include "fsl_device_registers.h"
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
extern void vPortGPTIsr(void);

#define vPortGptIsr GPT1_IRQHandler

#define CLOCK_SET_MUX(mux, value)                                                                        \
    \
do                                                                                                \
    {                                                                                                    \
        CCM_TUPLE_REG(CCM, mux) = (CCM_TUPLE_REG(CCM, mux) & (~CCM_TUPLE_MASK(mux))) |                   \
                                  (((uint32_t)((value) << CCM_TUPLE_SHIFT(mux))) & CCM_TUPLE_MASK(mux)); \
        while (CCM->CDHIPR != 0)                                                                         \
        {                                                                                                \
        }                                                                                                \
    \
}                                                                                                 \
    while (0)

#define CLOCK_SET_DIV(divider, value)                                                                                \
    \
do                                                                                                            \
    {                                                                                                                \
        CCM_TUPLE_REG(CCM, divider) = (CCM_TUPLE_REG(CCM, divider) & (~CCM_TUPLE_MASK(divider))) |                   \
                                      (((uint32_t)((value) << CCM_TUPLE_SHIFT(divider))) & CCM_TUPLE_MASK(divider)); \
        while (CCM->CDHIPR != 0)                                                                                     \
        {                                                                                                            \
        }                                                                                                            \
    \
}                                                                                                             \
    while (0)

#define CLOCK_CCM_HANDSHAKE_WAIT() \
    \
do                          \
    {                              \
        while (CCM->CDHIPR != 0)   \
        {                          \
        }                          \
    \
}                           \
    while (0)

#define LPM_DELAY(value)                         \
    \
do                                        \
    {                                            \
        for (uint32_t i = 0; i < 5 * value; i++) \
        {                                        \
            __NOP();                             \
        }                                        \
    \
}                                         \
    while (0)

#if defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
#define LPM_EnterCritical()                        \
    \
do                                          \
    {                                              \
        __disable_irq();                           \
        SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; \
    \
}                                           \
    while (0)

#define LPM_ExitCritical()                        \
    \
do                                         \
    {                                             \
        __enable_irq();                           \
        SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; \
    \
}                                          \
    while (0)

#else
#define LPM_EnterCritical()
#define LPM_ExitCritical()
#endif

/* 低功耗管理的功耗模式定义。
 * 唤醒持续时间关闭> Dsm>空闲>等待>运行。
 */
typedef enum _lpm_power_mode
{
    LPM_PowerModeOverRun = 0, /* 在RUN模式下，CPU不会停止运行 */

    LPM_PowerModeFullRun, /* 完全运行模式，CPU不会停止运行 */

    LPM_PowerModeLowSpeedRun,

    LPM_PowerModeLowPowerRun,

    LPM_PowerModeRunEnd = LPM_PowerModeLowPowerRun,
    /* 在系统等待模式下，cpu时钟被门控。
     *所有外设都可以保持活动状态，时钟门控由CCGR设置决定。
     * 当没有访问权限时，DRAM进入自动刷新模式。
     */
    LPM_PowerModeSysIdle, /* 系统等待模式，也是系统低速空闲 */

    /* 在低功耗空闲模式下，所有PLL / PFD都关闭，cpu电源关闭。
     * 模拟模块在低功耗模式下运行。
     * 所有高速外设都是电源门控
     * 低速外设可以保持低频运行
     * DRAM在自我刷新。
     */
    LPM_PowerModeLPIdle, /* Low Power Idle mode */

    /* In deep sleep mode, all PLL/PFD is off, XTAL is off, cpu power is off.
     * All clocks are shut off except 32K RTC clock
     * All high-speed peripherals are power gated
     * Low speed peripherals are clock gated
     * DRAM in self-refresh.
     * If RTOS is used, systick will be disabled in DSM
     */
    LPM_PowerModeSuspend, /* 深度睡眠模式，暂停。 */

    LPM_PowerModeSNVS, /* 电源关闭模式或关机模式 */

    LPM_PowerModeEnd = LPM_PowerModeSNVS
} lpm_power_mode_t;

typedef bool (*lpm_power_mode_callback_t)(lpm_power_mode_t curMode, lpm_power_mode_t newMode, void *data);

/*! @name Time sensitive region */
/* @{ */
#if defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
#if (defined(__ICCARM__))
#define QUICKACCESS_SECTION_CODE(func) __ramfunc func
#elif(defined(__ARMCC_VERSION))
#define QUICKACCESS_SECTION_CODE(func) __attribute__((section("RamFunction"))) func
#elif defined(__MCUXPRESSO)
#define QUICKACCESS_SECTION_CODE(func) __attribute__((section(".ramfunc.$SRAM_ITC"))) func
#elif(defined(__GNUC__))
#define QUICKACCESS_SECTION_CODE(func) __attribute__((section("RamFunction"))) func
#else
#error Toolchain not supported.
#endif /* defined(__ICCARM__) */
#else
#if (defined(__ICCARM__))
#define QUICKACCESS_SECTION_CODE(func) func
#elif(defined(__ARMCC_VERSION))
#define QUICKACCESS_SECTION_CODE(func) func
#elif(defined(__MCUXPRESSO))
#define QUICKACCESS_SECTION_CODE(func) func
#elif(defined(__GNUC__))
#define QUICKACCESS_SECTION_CODE(func) func
#else
#error Toolchain not supported.
#endif
#endif /* __FSL_SDK_DRIVER_QUICK_ACCESS_ENABLE */

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

QUICKACCESS_SECTION_CODE(void LPM_SwitchBandgap(void));
QUICKACCESS_SECTION_CODE(void LPM_RestoreBandgap(void));
QUICKACCESS_SECTION_CODE(void LPM_SwitchToXtalOSC(void));
QUICKACCESS_SECTION_CODE(void LPM_SwitchToRcOSC(void));
QUICKACCESS_SECTION_CODE(void LPM_SwitchFlexspiClock(lpm_power_mode_t power_mode));
QUICKACCESS_SECTION_CODE(void LPM_RestoreFlexspiClock(void));

/* 初始化低功耗管理*/
bool LPM_Init(lpm_power_mode_t run_mode);

/* 取消初始化低功耗管理 */
void LPM_Deinit(void);

/* 在低功耗模式下启用唤醒源 */
void LPM_EnableWakeupSource(uint32_t irq);

/* 在低功耗模式下禁用唤醒源 */
void LPM_DisableWakeupSource(uint32_t irq);

/* 设置电源模式，将通知所有已注册的听众。
 * 如果所有已注册的侦听器都返回true，则返回true。
 */
bool LPM_SetPowerMode(lpm_power_mode_t mode);

/* LPM_SetPowerMode() won't switch system power status immediately,
 * instead, such operation is done by LPM_WaitForInterrupt().
 * It can be callled in idle task of FreeRTOS, or main loop in bare
 * metal application. The sleep depth of this API depends
 * on current power mode set by LPM_SetPowerMode().
 * The timeoutMilliSec means if no interrupt occurs before timeout, the
 * system will be waken up by systick. If timeout exceeds hardware timer
 * limitation, timeout will be reduced to maximum time of hardware.
 * timeoutMilliSec only works in FreeRTOS, in bare metal application,
 * it's just ignored.
 * 
 * LPM_SetPowerMode（）不会立即切换系统电源状态，
 * 而是由LPM_WaitForInterrupt（）完成此类操作。
 * 它可以在FreeRTOS的空闲任务中调用，
 * 或者在裸机应用程序中进行主循环调用。 
 * 此API的睡眠深度取决于LPM_SetPowerMode（）设置的当前功耗模式。 
 * timeoutMilliSec意味着如果在超时之前没有发生中断，
 * 系统将被systick唤醒。 如果超时超过硬件定时器限制，
 * 则超时将减少到硬件的最大时间。 timeoutMilliSec只适用于FreeRTOS，
 * 在裸机应用程序中，它只是被忽略了。 
 */
void LPM_WaitForInterrupt(uint32_t timeoutMilliSec);

//#ifdef FSL_RTOS_FREE_RTOS
/* Register power mode switch listener. When LPM_SetPowerMode()
 * is called, all the registered listeners will be invoked. The
 * listener returns true if it allows the power mode switch,
 * otherwise returns FALSE.
 */
void LPM_RegisterPowerListener(lpm_power_mode_callback_t callback, void *data);

/* Unregister power mode switch listener */
extern void LPM_UnregisterPowerListener(lpm_power_mode_callback_t callback, void *data);
 
extern void LPM_SystemRestoreDsm(void);
extern void LPM_SystemRestoreIdle(void);
extern void LPM_SystemResumeDsm(void);
 
extern void LPM_RestorePLLs(lpm_power_mode_t power_mode);
extern void LPM_DisablePLLs(lpm_power_mode_t power_mode);
 
extern void LPM_SystemFullRun(void);
extern void LPM_SystemOverRun(void);
extern void LPM_SystemLowSpeedRun(void);
extern void LPM_SystemLowPowerRun(void);


extern void vPortPOST_SLEEP_PROCESSING(void);

//#endif

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _LPM_H_ */
