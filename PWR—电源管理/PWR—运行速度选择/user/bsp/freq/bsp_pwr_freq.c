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
  * 实验平台:野火  i.MXRT1052开发板
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
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
//static const char *s_modeNames[] = {"过度运行",    "满载运行",       "低速运行", "低功耗运行",
//                                    "系统空闲", "低功耗空闲", "暂停模式",       "SNVS"
//                                   };


void PrintfCoreClock()
{
    uint8_t ch;
    uint32_t freq;

    if (true != LPM_Init(s_curRunMode))
    {
        PRINTF("LPM Init Failed!\r\n");//初始化失败

    }

    while(1)
    {
        freq = CLOCK_GetFreq(kCLOCK_CpuClk);

        PRINTF("\r\n########## Power Mode Switch Demo (build %s) ###########\n\r\n", __DATE__);//电源管理选择 DEMO
        PRINTF("    Core Clock = %dHz \r\n", freq);

//    APP_ShowPowerMode(s_curRunMode);//系统时钟打印

        PRINTF("\r\nSelect the desired operation \n\r\n");//选择所需的操作
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

        PRINTF("\r\nWaiting for power mode select..\r\n\r\n");//等待电源管理模式选择 ..


        /* 等待用户应答  卡死接收串口消息 */
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
                PRINTF("Some task doesn't allow to enter mode %s\r\n", s_modeNames[s_targetPowerMode]);//某些任务不允许进入模式
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
