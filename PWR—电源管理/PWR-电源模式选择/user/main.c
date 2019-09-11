/**
 ******************************************************************
* @file    main.c
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
#include "fsl_debug_console.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_common.h"
#include "fsl_gpt.h"
#include "fsl_lpuart.h"

#include "./pwr/bsp_power_mode_switch_bm.h"
#include "./pwr/bsp_lpm.h"
#include "./pwr/bsp_specific.h"

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
    /****************************��һ����**********************/
    __IO uint8_t ch;
    uint32_t freq;
    bool needSetWakeup; /* ��Ҫ���û���. */
    gpt_config_t gptConfig;
    /* �������뿪�عܽŵĳ�ʼ���ṹ*/
    gpio_pin_config_t swConfig = {
        kGPIO_DigitalInput,
        0,
        kGPIO_IntRisingEdge,
    };
    /* ��ʼ���ڴ汣����Ԫ */
    BOARD_ConfigMPU();
    /* ��ʼ������������ */
    BOARD_InitPins();
    /* ��ʼ��������ʱ�� */
    BOARD_BootClockRUN();
    /* ��UART��Ƶ������ΪĬ��ֵ */
    CLOCK_SetMux(kCLOCK_UartMux, 1); /*��UARTԴ����ΪOSC 24M */
    CLOCK_SetDiv(kCLOCK_UartDiv, 0); /* ��UART��Ƶ������Ϊ1 */
    /* ��ʼ�����Դ��� */
    BOARD_InitDebugConsole();
    /* ��ʼ��SNVS_PMIC_STBY_REQ_GPIO5_IO02������� */
    BOARD_Init_PMIC_STBY_REQ();
    /* ��ʼGPTģ�����ڻ��� */
    GPT_GetDefaultConfig(&gptConfig);
    gptConfig.clockSource = kGPT_ClockSource_LowFreq; /* 32K RTC OSC */
    gptConfig.enableMode = true;                      /*ֹͣʱ��Ҫ���ּ����� */
    gptConfig.enableRunInDoze = true;
    /* ��ʼ��GPTģ�� */
    GPT_Init(APP_WAKEUP_GPT_BASE, &gptConfig);
    GPT_SetClockDivider(APP_WAKEUP_GPT_BASE, 1);
    /* Init���뿪��GPIO�� */
    GPIO_PinInit(APP_WAKEUP_BUTTON_GPIO, APP_WAKEUP_BUTTON_GPIO_PIN, &swConfig);
    /****************************�ڶ�����**********************/
    /* ��ӡ��Ϣ */
		PRINTF("**************��ӭʹ�� Ұ��i.MX RT1052 ������**************\r\n");
    PRINTF("\r\nCPU wakeup source 0x%x...\r\n", SRC->SRSR);
    PRINTF("\r\n***********************************************************\r\n");
    PRINTF("\tPower Mode Switch Demo for %s\r\n", CPU_NAME);
    PRINTF("***********************************************************\r\n");
    APP_PrintRunFrequency(0); //�������ʱ��Ƶ��
		 
    /* LPM��ʼ�� */
    LPM_Init();
//    /*�ϵ�󽫵�Դģʽ����Ϊ���� */
//		APP_SetRunMode(LPM_PowerModeOverRun);
//		LPM_OverDriveRun();
    while (1)
    {
        /****************************��������**********************/
        /* ��ȡʱ��Ƶ�� */
        freq = CLOCK_GetFreq(kCLOCK_CpuClk);
        /* ��ӡʱ�������Ϣ */
        PRINTF("\r\n########## Power Mode Switch (build %s) ###########\n\r\n", __DATE__);
        PRINTF("    Core Clock = %dHz \r\n", freq);
        APP_ShowPowerMode(s_curRunMode);
        /* ��ӡ������Ϣ */
        PRINTF("\r\nSelect the desired operation \n\r\n");
        PRINTF("Press  %c for enter: Over RUN       - System Over Run mode\r\n",
               (uint8_t)'A' + (uint8_t)LPM_PowerModeOverRun);
        PRINTF("Press  %c for enter: Full RUN       - System Full Run mode\r\n",
               (uint8_t)'A' + (uint8_t)LPM_PowerModeFullRun);
        PRINTF("Press  %c for enter: Low Speed RUN  - System Low Speed Run mode\r\n",
               (uint8_t)'A' + (uint8_t)LPM_PowerModeLowSpeedRun);
        PRINTF("Press  %c for enter: Low Power RUN  - System Low Power Run mode\r\n",
               (uint8_t)'A' + (uint8_t)LPM_PowerModeLowPowerRun);
        PRINTF("Press  %c for enter: System Idle    - System Wait mode\r\n",
               (uint8_t)'A' + (uint8_t)LPM_PowerModeSysIdle);
        PRINTF("Press  %c for enter: Low Power Idle - Low Power Idle mode\r\n",
               (uint8_t)'A' + (uint8_t)LPM_PowerModeLPIdle);
        PRINTF("Press  %c for enter: Suspend        - Suspend mode\r\n", (uint8_t)'A' + (uint8_t)LPM_PowerModeSuspend);
#if (HAS_WAKEUP_PIN)
        PRINTF("Press  %c for enter: SNVS           - Shutdown the system\r\n",
               (uint8_t)'A' + (uint8_t)LPM_PowerModeSNVS);
#endif
        PRINTF("\r\nWaiting for power mode select...\r\n\r\n");
        /****************************���Ĳ���**********************/
        /* �ȴ��û���Ӧ */
        ch = GETCHAR();
        /* �ж��Ƿ�Ϊ��ĸ */
        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }
        /* ����ĸ��ö�����Ͷ�Ӧ�������ǰ��Դģʽ */
        s_targetPowerMode = (lpm_power_mode_t)(ch - 'A');
        /****************************���岿��**********************/
        /* �жϵ�ǰģʽ��ö�������� */
        if (s_targetPowerMode <= LPM_PowerModeEnd)
        {
            /*����޷�����Ŀ���Դģʽ����ѭ�������� */
            if (!APP_CheckPowerMode(s_curRunMode, s_targetPowerMode))
            {
                continue;
            }
            /*���Ŀ��ģʽ������ģʽ������Ҫ���û���Դ�� */
            if (s_targetPowerMode <= LPM_PowerModeLowPowerRun)
            {
                needSetWakeup = false;
            }
            else
            {
                needSetWakeup = true;
            }
            /* �ж��Ƿ���Ҫ���� */
            /****************************��������**********************/
            if (needSetWakeup)
            {
                /* ��ȡ���ѳ�ʱ�ͻ���Դ������*/
                APP_GetWakeupConfig(s_targetPowerMode);
                /* ���ݻ���Դ���� */
                APP_SetWakeupConfig(s_targetPowerMode);
            }
            /****************************���߲���**********************/
            APP_PowerPreSwitchHook(s_targetPowerMode);
            /* ��Դģʽѡ�� */
            APP_PowerModeSwitch(s_targetPowerMode);
            APP_PowerPostSwitchHook(s_targetPowerMode);
        }
        /* һ��ѭ������ */
        PRINTF("\r\nNext loop\r\n");
    }
}

/****************************END OF FILE**********************/
