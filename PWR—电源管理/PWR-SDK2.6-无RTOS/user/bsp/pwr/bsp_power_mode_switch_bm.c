/**
  ******************************************************************
  * @file    bsp_power_mode_switch_bm.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   PWR-��Դģʽѡ��
  ******************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
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

uint8_t s_wakeupTimeout;            /* ���ѳ�ʱ�� ����λ���룩*/
app_wakeup_source_t s_wakeupSource; /*������Դ��                 */
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
 * @brief GPT �жϷ�����
 * @return ��
 *   @retval ��
 */
void APP_WAKEUP_GPT_IRQn_HANDLER(void)
{
    /* ���GPT��־ */
    GPT_ClearStatusFlags(APP_WAKEUP_GPT_BASE, kGPT_OutputCompare1Flag);
    /* ֹͣGPT��ʱ�� */
    GPT_StopTimer(APP_WAKEUP_GPT_BASE);
    /* LPM�رջ���Դ */
    LPM_DisableWakeupSource(APP_WAKEUP_GPT_IRQn);
}

/**
 * @brief �������ѷ�����
 * @return ��
 *   @retval ��
 */
void APP_WAKEUP_BUTTON_IRQ_HANDLER(void)
{
    if ((1U << APP_WAKEUP_BUTTON_GPIO_PIN) & GPIO_GetPinsInterruptFlags(APP_WAKEUP_BUTTON_GPIO))
    {
         /* �����ж�. */
        GPIO_DisableInterrupts(APP_WAKEUP_BUTTON_GPIO, 1U << APP_WAKEUP_BUTTON_GPIO_PIN);
         /* LPM�رջ���Դ */
        GPIO_ClearPinsInterruptFlags(APP_WAKEUP_BUTTON_GPIO, 1U << APP_WAKEUP_BUTTON_GPIO_PIN);
         /* LPM�رջ���Դ */
        LPM_DisableWakeupSource(APP_WAKEUP_BUTTON_IRQ);
    }
}

/**
 * @brief ��ȡ�û����ڻ��ѳ�ʱ������
 * @return ��
 *   @retval ��
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
 * @brief ͨ���û������ȡ����Դ
 * @param targetMode ��ǰģʽ
 * @return ��
 *   @retval ��
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
 * @brief ��ȡ���ѳ�ʱ�ͻ���Դ
 * @param targetMode ��ǰģʽ
 * @return ��
 *   @retval ��
 */
void APP_GetWakeupConfig(lpm_power_mode_t targetMode)
{
    if (targetMode == LPM_PowerModeSNVS)
    {
        /* ��SNVSģʽ�£�ֻ��SNVS�򹩵磬GPT�޷������� */
        s_wakeupSource = kAPP_WakeupSourcePin;
    }
    else
    {
        /*ͨ���û������ȡ����Դ�� */
        s_wakeupSource = APP_GetWakeupSource(targetMode);
    }

    if (kAPP_WakeupSourceGPT == s_wakeupSource)
    {
        /* ����Դ��GPT���û�Ӧ���뻽�ѳ�ʱֵ�� */
        s_wakeupTimeout = APP_GetWakeupTimeout();
        PRINTF("Will wakeup in %d seconds.\r\n", s_wakeupTimeout);
    }
    else
    {
        PRINTF("Switch %s from off to on to wake up.\r\n", APP_WAKEUP_BUTTON_NAME);
    }
}

/**
 * @brief ���û�������
 * @param targetMode ��ǰģʽ
 * @return ��
 *   @retval ��
 */
void APP_SetWakeupConfig(lpm_power_mode_t targetMode)
{
    /*����GPT��ʱֵ�� */
    if (kAPP_WakeupSourceGPT == s_wakeupSource)
    {
        GPT_StopTimer(APP_WAKEUP_GPT_BASE);
        /* ���±Ƚ�channel1ֵ�����ü����� */
        GPT_SetOutputCompareValue(APP_WAKEUP_GPT_BASE, kGPT_OutputCompare_Channel1,
                                  (CLOCK_GetRtcFreq() * s_wakeupTimeout) - 1U);

        /*����GPT���Compare1�ж� */
        GPT_EnableInterrupts(APP_WAKEUP_GPT_BASE, kGPT_OutputCompare1InterruptEnable);
        NVIC_EnableIRQ(APP_WAKEUP_GPT_IRQn);
        EnableIRQ(APP_WAKEUP_GPT_IRQn);

        /* ������ʱ�� */
        GPT_StartTimer(APP_WAKEUP_GPT_BASE);

        LPM_EnableWakeupSource(APP_WAKEUP_GPT_IRQn);
    }
    else
    {
        GPIO_ClearPinsInterruptFlags(APP_WAKEUP_BUTTON_GPIO, 1U << APP_WAKEUP_BUTTON_GPIO_PIN);
        /* ʹ��GPIO�����ж� */
        GPIO_EnableInterrupts(APP_WAKEUP_BUTTON_GPIO, 1U << APP_WAKEUP_BUTTON_GPIO_PIN);
        NVIC_EnableIRQ(APP_WAKEUP_BUTTON_IRQ);
        /* �����ж�*/
        EnableIRQ(APP_WAKEUP_BUTTON_IRQ);
        /* ����GPC�ж�*/
        LPM_EnableWakeupSource(APP_WAKEUP_BUTTON_IRQ);
    }
}

/**
 * @brief ��ȡ����ģʽ
 */
lpm_power_mode_t APP_GetRunMode(void)
{
    return s_curRunMode;
}

/**
 * @brief ��������ģʽ
 * @param powerMode ��Դģʽ
 * @return ��
 *   @retval ��
 */
void APP_SetRunMode(lpm_power_mode_t powerMode)
{
    s_curRunMode = powerMode;
}

/**
 * @brief ��ʾ��Դģʽ
 * @param powerMode ��Դģʽ
 * @return ��
 *   @retval ��
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
 * ����Ƿ���Դӵ�ǰģʽ�л���Ŀ���Դģʽ��
 *   ��������л��򷵻�true������޷��л��򷵻�false��
 */
/**
 * @brief ��鵱ǰ��Դģʽ
 * @param originPowerMode ԭ����Դģʽ  targetPowerMode Ŀ���Դģʽ
 * @return ��
 *   @retval ��
 */
bool APP_CheckPowerMode(lpm_power_mode_t originPowerMode, lpm_power_mode_t targetPowerMode)
{
    bool modeValid = true;

    /* �����ǰģʽΪ�͹�������ģʽ����Ŀ��ģʽ��ӦΪϵͳ����ģʽ. */
    if ((originPowerMode == LPM_PowerModeLowPowerRun) && (targetPowerMode == LPM_PowerModeSysIdle))
    {
        PRINTF("Low Power Run mode can't enter System Idle mode.\r\n");
        modeValid = false;
    }

    /* �����ǰģʽ�Ѿ���Ŀ��ģʽ����������ĵ�Դģʽ�� */
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
        /* �ȴ����Կ���̨�����ɡ�*/
        while (!(kLPUART_TransmissionCompleteFlag & LPUART_GetStatusFlags((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR)))
        {
        }
        DbgConsole_Deinit();

        /*
         * ��������©������
         * ���Կ���̨RX���ţ���pinmux����ΪGPIO���롣
         * ���Կ���̨TX���ţ�����Ҫ���ġ�
         */
        ConfigUartRxPinToGpio();
    }
}


void APP_PowerPostSwitchHook(lpm_power_mode_t targetMode)
{
    if (targetMode > LPM_PowerModeRunEnd)
    {
        /*
         *���Կ���̨RX��������ΪGPIO���룬��Ҫ��������pinmux��
         * ���Կ���̨TX���ţ�����Ҫ���ġ�
         */
        ReConfigUartRxPin();
        BOARD_InitDebugConsole();

        /* �ָ�����ǰ������ģʽ */
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
        /* ���µ�ǰ����ģʽ */
        APP_SetRunMode(targetMode);
    }
}

/**
 * @brief ��Դ����ģʽѡ��
 * @param targetPowerMode ��ǰģʽ
 * @return ��
 *   @retval ��
 */
void APP_PowerModeSwitch(lpm_power_mode_t targetPowerMode)
{
    switch (targetPowerMode)
    {
    case LPM_PowerModeOverRun://��������ģʽ A
        LPM_OverDriveRun();
        break;
    case LPM_PowerModeFullRun://��������ģʽ B
        LPM_FullSpeedRun();
        break;
    case LPM_PowerModeLowSpeedRun://��������ģʽ C
        LPM_LowSpeedRun();
        break;
    case LPM_PowerModeLowPowerRun://�͹�������ģʽ D
        LPM_LowPowerRun();
        break;
    case LPM_PowerModeSysIdle://ϵͳ����ģʽ E
        LPM_PreEnterWaitMode();
        LPM_EnterSystemIdle();
        LPM_ExitSystemIdle();
        LPM_PostExitWaitMode();
        break;
    case LPM_PowerModeLPIdle://�͹��Ŀ���ģʽ F
        LPM_PreEnterWaitMode();
        LPM_EnterLowPowerIdle();
        LPM_ExitLowPowerIdle();
        LPM_PostExitWaitMode();
        break;
    case LPM_PowerModeSuspend://��ͣģʽ G
        LPM_PreEnterStopMode();
        LPM_EnterSuspend();
        LPM_PostExitStopMode();
        break;
#if (HAS_WAKEUP_PIN)
    case LPM_PowerModeSNVS://��SNVS���ػ�ģʽ H
        LPM_EnterSNVS();
        break;
#endif
    default:
        assert(false);//����
        break;
    }
}
