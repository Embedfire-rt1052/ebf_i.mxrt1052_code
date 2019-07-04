/**
  ******************************************************************
  * @file    
  * @author  fire
  * @version V2.0
  * @date    2018-xx-xx
  * @brief
  ******************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  i.MXRT1052������
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************
  */
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#include "fsl_elcdif.h"
#include "fsl_clock.h"
#include "fsl_debug_console.h"

#include "pad_config.h"

#include "./freq/bsp_pwr_freq.h"
#include "lpm.h"

static lpm_power_mode_t s_targetPowerMode;

static lpm_power_mode_t s_curRunMode = LPM_PowerModeOverRun;

static const char *s_modeNames[] = {"Over RUN",    "Full Run",       "Low Speed Run", "Low Power Run",
                                    "System Idle", "Low Power Idle", "Suspend",       "SNVS"
                                   };
//static const char *s_modeNames[] = {"��������",    "��������",       "��������", "�͹�������",
//                                    "ϵͳ����", "�͹��Ŀ���", "��ͣģʽ",       "SNVS"
//                                   };


void PrintfCoreClock()
{
    uint8_t ch;
    uint32_t freq;

    if (true != LPM_Init(s_curRunMode))
    {
        PRINTF("LPM Init Failed!\r\n");//��ʼ��ʧ��

    }

    while(1)
    {
        freq = CLOCK_GetFreq(kCLOCK_CpuClk);

        PRINTF("\r\n########## Power Mode Switch Demo (build %s) ###########\n\r\n", __DATE__);//��Դ����ѡ�� DEMO
        PRINTF("    Core Clock = %dHz \r\n", freq);

//    APP_ShowPowerMode(s_curRunMode);//ϵͳʱ�Ӵ�ӡ

        PRINTF("\r\nSelect the desired operation \n\r\n");//ѡ������Ĳ���
        PRINTF("Press  %c for enter: Over RUN       - System Over Run mode (600MHz)\r\n",
               (uint8_t)'A' + (uint8_t)LPM_PowerModeOverRun);
        PRINTF("Press  %c for enter: Full RUN       - System Full Run mode (528MHz)\r\n",
               (uint8_t)'A' + (uint8_t)LPM_PowerModeFullRun);
        PRINTF("Press  %c for enter: Low Speed RUN  - System Low Speed Run mode (132MHz)\r\n",
               (uint8_t)'A' + (uint8_t)LPM_PowerModeLowSpeedRun);
        PRINTF("Press  %c for enter: Low Power RUN  - System Low Power Run mode (24MHz)\r\n",
               (uint8_t)'A' + (uint8_t)LPM_PowerModeLowPowerRun);
        PRINTF("Press  %c for enter: System Idle    - System Wait mode\r\n",
               (uint8_t)'A' + (uint8_t)LPM_PowerModeSysIdle);
        PRINTF("Press  %c for enter: Low Power Idle - Low Power Idle mode\r\n",
               (uint8_t)'A' + (uint8_t)LPM_PowerModeLPIdle);
        PRINTF("Press  %c for enter: Suspend        - Suspend mode\r\n", (uint8_t)'A' + (uint8_t)LPM_PowerModeSuspend);
        PRINTF("Press  %c for enter: SNVS           - Shutdown the system\r\n",
               (uint8_t)'A' + (uint8_t)LPM_PowerModeSNVS);

        PRINTF("\r\nWaiting for power mode select..\r\n\r\n");//�ȴ���Դ����ģʽѡ�� ..


        /* �ȴ��û�Ӧ��  �������մ�����Ϣ */
        ch = GETCHAR();

        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }

        s_targetPowerMode = (lpm_power_mode_t)(ch - 'A');

        if (s_targetPowerMode <= LPM_PowerModeSNVS)
        {
            if (!LPM_SetPowerMode(s_targetPowerMode))
            {
                PRINTF("Some task doesn't allow to enter mode %s\r\n", s_modeNames[s_targetPowerMode]);//ĳЩ�����������ģʽ
            }
            else
            {
                if (s_targetPowerMode <= LPM_PowerModeRunEnd)
                {
                    switch (s_targetPowerMode)
                    {
                    case LPM_PowerModeOverRun:
                        LPM_SystemOverRun();//600
                        break;
                    case LPM_PowerModeFullRun:
                        LPM_SystemFullRun();//528
                        break;
                    case LPM_PowerModeLowSpeedRun:
                        LPM_SystemLowSpeedRun();
                        break;
                    case LPM_PowerModeLowPowerRun:
                        LPM_SystemLowPowerRun();
                        break;
                    default:
                        break;
                    }
                    s_curRunMode = s_targetPowerMode;
                    continue;
                }


            }


        }
    }
}
