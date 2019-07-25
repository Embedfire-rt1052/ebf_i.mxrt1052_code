/**
  ******************************************************************
  * @file    bsp_power_mode_switch_bm.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   PWR-电源模式选择
  ******************************************************************
  * @attention
  *
  * 实验平台:野火  i.MXRT1052开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************
  */
#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "fsl_gpt.h"
#include "fsl_lpuart.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "./pwr/bsp_power_mode_switch_bm.h"
#include "./pwr/bsp_specific.h"
#include "./pwr/bsp_lpm.h"

uint8_t s_wakeupTimeout;            /* 唤醒超时。 （单位：秒）*/
app_wakeup_source_t s_wakeupSource; /*唤醒来源。                 */
lpm_power_mode_t s_targetPowerMode;
lpm_power_mode_t s_curRunMode = LPM_PowerModeOverRun;
const char *s_modeNames[] = {"Over RUN", "Full Run", "Low Speed Run", "Low Power Run",
                                    "System Idle", "Low Power Idle", "Suspend",
#if (HAS_WAKEUP_PIN)
                                    "SNVS"
#endif
};

/*******************************************************************************
 * Code
 ******************************************************************************/

/**
 * @brief GPT 中断服务函数
 * @return 无
 *   @retval 无
 */
void APP_WAKEUP_GPT_IRQn_HANDLER(void)
{
    /* 清除GPT标志 */
    GPT_ClearStatusFlags(APP_WAKEUP_GPT_BASE, kGPT_OutputCompare1Flag);
    /* 停止GPT计时器 */
    GPT_StopTimer(APP_WAKEUP_GPT_BASE);
    /* LPM关闭唤醒源 */
    LPM_DisableWakeupSource(APP_WAKEUP_GPT_IRQn);
}

/**
 * @brief 按键唤醒服务函数
 * @return 无
 *   @retval 无
 */
void APP_WAKEUP_BUTTON_IRQ_HANDLER(void)
{
    if ((1U << APP_WAKEUP_BUTTON_GPIO_PIN) & GPIO_GetPinsInterruptFlags(APP_WAKEUP_BUTTON_GPIO))
    {
         /* 禁用中断. */
        GPIO_DisableInterrupts(APP_WAKEUP_BUTTON_GPIO, 1U << APP_WAKEUP_BUTTON_GPIO_PIN);
         /* LPM关闭唤醒源 */
        GPIO_ClearPinsInterruptFlags(APP_WAKEUP_BUTTON_GPIO, 1U << APP_WAKEUP_BUTTON_GPIO_PIN);
         /* LPM关闭唤醒源 */
        LPM_DisableWakeupSource(APP_WAKEUP_BUTTON_IRQ);
    }
}

/**
 * @brief 获取用户关于唤醒超时的输入
 * @return 无
 *   @retval 无
 */
static uint8_t APP_GetWakeupTimeout(void)
{
    uint8_t timeout;

    while (1)
    {
        PRINTF("Select the wake up timeout in seconds.\r\n");
        PRINTF("The allowed range is 1s ~ 9s.\r\n");
        PRINTF("Eg. enter 5 to wake up in 5 seconds.\r\n");
        PRINTF("\r\nWaiting for input timeout value...\r\n\r\n");

        timeout = GETCHAR();
        PRINTF("%c\r\n", timeout);
        if ((timeout > '0') && (timeout <= '9'))
        {
            return timeout - '0';
        }
        PRINTF("Wrong value!\r\n");
    }
}

/**
 * @brief 通过用户输入获取唤醒源
 * @param targetMode 当前模式
 * @return 无
 *   @retval 无
 */
static app_wakeup_source_t APP_GetWakeupSource(lpm_power_mode_t targetMode)
{
    uint8_t ch;

    while (1)
    {
        PRINTF("Select the wake up source:\r\n");
        PRINTF("Press T for GPT - GPT Timer\r\n");
#if (HAS_WAKEUP_PIN)
        PRINTF("Press S for switch/button %s. \r\n", APP_WAKEUP_BUTTON_NAME);
#endif

        PRINTF("\r\nWaiting for key press..\r\n\r\n");

        ch = GETCHAR();

        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }

        if (ch == 'T')
        {
            return kAPP_WakeupSourceGPT;
        }
#if (HAS_WAKEUP_PIN)
        else if (ch == 'S')
        {
            return kAPP_WakeupSourcePin;
        }
#endif
        else
        {
            PRINTF("Wrong value!\r\n");
        }
    }
}

/**
 * @brief 获取唤醒超时和唤醒源
 * @param targetMode 当前模式
 * @return 无
 *   @retval 无
 */
void APP_GetWakeupConfig(lpm_power_mode_t targetMode)
{
    if (targetMode == LPM_PowerModeSNVS)
    {
        /* 在SNVS模式下，只有SNVS域供电，GPT无法工作。 */
        s_wakeupSource = kAPP_WakeupSourcePin;
    }
    else
    {
        /*通过用户输入获取唤醒源。 */
        s_wakeupSource = APP_GetWakeupSource(targetMode);
    }

    if (kAPP_WakeupSourceGPT == s_wakeupSource)
    {
        /* 唤醒源是GPT，用户应输入唤醒超时值。 */
        s_wakeupTimeout = APP_GetWakeupTimeout();
        PRINTF("Will wakeup in %d seconds.\r\n", s_wakeupTimeout);
    }
    else
    {
        PRINTF("Switch %s from off to on to wake up.\r\n", APP_WAKEUP_BUTTON_NAME);
    }
}

/**
 * @brief 设置唤醒配置
 * @param targetMode 当前模式
 * @return 无
 *   @retval 无
 */
void APP_SetWakeupConfig(lpm_power_mode_t targetMode)
{
    /*设置GPT超时值。 */
    if (kAPP_WakeupSourceGPT == s_wakeupSource)
    {
        GPT_StopTimer(APP_WAKEUP_GPT_BASE);
        /* 更新比较channel1值将重置计数器 */
        GPT_SetOutputCompareValue(APP_WAKEUP_GPT_BASE, kGPT_OutputCompare_Channel1,
                                  (CLOCK_GetRtcFreq() * s_wakeupTimeout) - 1U);

        /*启用GPT输出Compare1中断 */
        GPT_EnableInterrupts(APP_WAKEUP_GPT_BASE, kGPT_OutputCompare1InterruptEnable);
        NVIC_EnableIRQ(APP_WAKEUP_GPT_IRQn);
        EnableIRQ(APP_WAKEUP_GPT_IRQn);

        /* 重启计时器 */
        GPT_StartTimer(APP_WAKEUP_GPT_BASE);

        LPM_EnableWakeupSource(APP_WAKEUP_GPT_IRQn);
    }
    else
    {
        GPIO_ClearPinsInterruptFlags(APP_WAKEUP_BUTTON_GPIO, 1U << APP_WAKEUP_BUTTON_GPIO_PIN);
        /* 使能GPIO引脚中断 */
        GPIO_EnableInterrupts(APP_WAKEUP_BUTTON_GPIO, 1U << APP_WAKEUP_BUTTON_GPIO_PIN);
        NVIC_EnableIRQ(APP_WAKEUP_BUTTON_IRQ);
        /* 启用中断*/
        EnableIRQ(APP_WAKEUP_BUTTON_IRQ);
        /* 启用GPC中断*/
        LPM_EnableWakeupSource(APP_WAKEUP_BUTTON_IRQ);
    }
}

/**
 * @brief 获取运行模式
 */
lpm_power_mode_t APP_GetRunMode(void)
{
    return s_curRunMode;
}

/**
 * @brief 设置运行模式
 * @param powerMode 电源模式
 * @return 无
 *   @retval 无
 */
void APP_SetRunMode(lpm_power_mode_t powerMode)
{
    s_curRunMode = powerMode;
}

/**
 * @brief 显示电源模式
 * @param powerMode 电源模式
 * @return 无
 *   @retval 无
 */
void APP_ShowPowerMode(lpm_power_mode_t powerMode)
{
    if (powerMode <= LPM_PowerModeRunEnd)
    {
        PRINTF("    Power mode: %s\r\n", s_modeNames[powerMode]);
        APP_PrintRunFrequency(1);
    }
    else
    {
        assert(0);
    }
}

/*
 * 检查是否可以从当前模式切换到目标电源模式。
 *   如果可以切换则返回true，如果无法切换则返回false。
 */
/**
 * @brief 检查当前电源模式
 * @param originPowerMode 原来电源模式  targetPowerMode 目标电源模式
 * @return 无
 *   @retval 无
 */
bool APP_CheckPowerMode(lpm_power_mode_t originPowerMode, lpm_power_mode_t targetPowerMode)
{
    bool modeValid = true;

    /* 如果当前模式为低功率运行模式，则目标模式不应为系统空闲模式. */
    if ((originPowerMode == LPM_PowerModeLowPowerRun) && (targetPowerMode == LPM_PowerModeSysIdle))
    {
        PRINTF("Low Power Run mode can't enter System Idle mode.\r\n");
        modeValid = false;
    }

    /* 如果当前模式已经是目标模式，则无需更改电源模式。 */
    if (originPowerMode == targetPowerMode)
    {
        PRINTF("Already in the target power mode.\r\n");
        modeValid = false;
    }

    return modeValid;
}

void APP_PowerPreSwitchHook(lpm_power_mode_t targetMode)
{
    if (targetMode == LPM_PowerModeSNVS)
    {
        PRINTF("Now shutting down the system...\r\n");
    }

    if (targetMode > LPM_PowerModeRunEnd)
    {
        /* 等待调试控制台输出完成。*/
        while (!(kLPUART_TransmissionCompleteFlag & LPUART_GetStatusFlags((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR)))
        {
        }
        DbgConsole_Deinit();

        /*
         * 设置引脚漏电流。
         * 调试控制台RX引脚：将pinmux设置为GPIO输入。
         * 调试控制台TX引脚：不需要更改。
         */
        ConfigUartRxPinToGpio();
    }
}


void APP_PowerPostSwitchHook(lpm_power_mode_t targetMode)
{
    if (targetMode > LPM_PowerModeRunEnd)
    {
        /*
         *调试控制台RX引脚设置为GPIO输入，需要重新配置pinmux。
         * 调试控制台TX引脚：不需要更改。
         */
        ReConfigUartRxPin();
        BOARD_InitDebugConsole();

        /* 恢复到以前的运行模式 */
        switch (APP_GetRunMode())
        {
        case LPM_PowerModeOverRun:
            LPM_OverDriveRun();
            break;
        case LPM_PowerModeFullRun:
            LPM_FullSpeedRun();
            break;
        case LPM_PowerModeLowSpeedRun:
            LPM_LowSpeedRun();
            break;
        case LPM_PowerModeLowPowerRun:
            LPM_LowPowerRun();
            break;
        default:
            break;
        }
    }
    else
    {
        /* 更新当前运行模式 */
        APP_SetRunMode(targetMode);
    }
}

/**
 * @brief 电源管理模式选择
 * @param targetPowerMode 当前模式
 * @return 无
 *   @retval 无
 */
void APP_PowerModeSwitch(lpm_power_mode_t targetPowerMode)
{
    switch (targetPowerMode)
    {
    case LPM_PowerModeOverRun://超载运行模式 A
        LPM_OverDriveRun();
        break;
    case LPM_PowerModeFullRun://满载运行模式 B
        LPM_FullSpeedRun();
        break;
    case LPM_PowerModeLowSpeedRun://低速运行模式 C
        LPM_LowSpeedRun();
        break;
    case LPM_PowerModeLowPowerRun://低功耗运行模式 D
        LPM_LowPowerRun();
        break;
    case LPM_PowerModeSysIdle://系统空闲模式 E
        LPM_PreEnterWaitMode();
        LPM_EnterSystemIdle();
        LPM_ExitSystemIdle();
        LPM_PostExitWaitMode();
        break;
    case LPM_PowerModeLPIdle://低功耗空闲模式 F
        LPM_PreEnterWaitMode();
        LPM_EnterLowPowerIdle();
        LPM_ExitLowPowerIdle();
        LPM_PostExitWaitMode();
        break;
    case LPM_PowerModeSuspend://暂停模式 G
        LPM_PreEnterStopMode();
        LPM_EnterSuspend();
        LPM_PostExitStopMode();
        break;
#if (HAS_WAKEUP_PIN)
    case LPM_PowerModeSNVS://（SNVS）关机模式 H
        LPM_EnterSNVS();
        break;
#endif
    default:
        assert(false);//断言
        break;
    }
}
