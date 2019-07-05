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

//static lpm_power_mode_t s_targetPowerMode;

//static lpm_power_mode_t s_curRunMode = LPM_PowerModeOverRun;

//static const char *s_modeNames[] = {"Over RUN",    "Full Run",       "Low Speed Run", "Low Power Run",
//                                    "System Idle", "Low Power Idle", "Suspend",       "SNVS"
//                                   };
////static const char *s_modeNames[] = {"过度运行",    "满载运行",       "低速运行", "低功耗运行",
////                                    "系统空闲", "低功耗空闲", "暂停模式",       "SNVS"
////                                   };

//int32_t is_suspend_reset = 0;

///*
//打印时钟频率
// */
//static void APP_PrintRunFrequency(int32_t run_freq_only)
//{
//    PRINTF("\r\n");
//    PRINTF("***********************************************************\r\n");
//    PRINTF("CPU:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
//    PRINTF("AHB:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AhbClk));
//    PRINTF("SEMC:            %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
//    PRINTF("IPG:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_IpgClk));
//    PRINTF("OSC:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_OscClk));
//    PRINTF("RTC:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_RtcClk));
//    PRINTF("ARMPLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_ArmPllClk));
//    if (!run_freq_only)
//    {
//        PRINTF("USB1PLL:         %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllClk));
//        PRINTF("USB1PLLPFD0:     %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk));
//        PRINTF("USB1PLLPFD1:     %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd1Clk));
//        PRINTF("USB1PLLPFD2:     %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd2Clk));
//        PRINTF("USB1PLLPFD3:     %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd3Clk));
//        PRINTF("USB2PLL:         %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb2PllClk));
//        PRINTF("SYSPLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
//        PRINTF("SYSPLLPFD0:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk));
//        PRINTF("SYSPLLPFD1:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd1Clk));
//        PRINTF("SYSPLLPFD2:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
//        PRINTF("SYSPLLPFD3:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd3Clk));
//        PRINTF("ENETPLL0:        %d Hz\r\n", CLOCK_GetFreq(kCLOCK_EnetPll0Clk));
//        PRINTF("ENETPLL1:        %d Hz\r\n", CLOCK_GetFreq(kCLOCK_EnetPll1Clk));
//        PRINTF("AUDIOPLL:        %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AudioPllClk));
//        PRINTF("VIDEOPLL:        %d Hz\r\n", CLOCK_GetFreq(kCLOCK_VideoPllClk));
//    }
//    PRINTF("***********************************************************\r\n");
//    PRINTF("\r\n");
//}
//																	 
///*
//显示
//频率 以及
//当前的电源管理模式
// */
//static void APP_ShowPowerMode(lpm_power_mode_t powerMode)
//{
//    if (powerMode <= LPM_PowerModeEnd)
//    {
//        PRINTF("    Power mode: %s\r\n", s_modeNames[powerMode]);//电源管理模式
//        APP_PrintRunFrequency(1);
//    }
//    else
//    {
//        assert(0);
//    }
//}			 
//void PrintfCoreClock()
//{
//	uint8_t ch;
//    uint32_t freq;
////    gpt_config_t gptConfig;
////    /* 定义输入开关引脚的init结构 */
////    gpio_pin_config_t swConfig = {
////        kGPIO_DigitalInput, 0, kGPIO_IntRisingEdge,
////    };

////    /* 唤醒初始GPT作为FreeRTOS告诉我们 */
////    GPT_GetDefaultConfig(&gptConfig);
////    gptConfig.clockSource = kGPT_ClockSource_LowFreq; /* 32K RTC OSC */
////    // gptConfig.enableMode = false;                     /* 停止时保持计数器 */
////    gptConfig.enableMode = true; /* 停止时不要保持计数器 */
////    gptConfig.enableRunInDoze = true;
////    /* 初始化GPT模块 */
////    GPT_Init(APP_WAKEUP_GPT_BASE, &gptConfig);
////    GPT_SetClockDivider(APP_WAKEUP_GPT_BASE, 1);

////    /* Init输入开关GPIO. */
////    GPIO_PinInit(APP_WAKEUP_BUTTON_GPIO, APP_WAKEUP_BUTTON_GPIO_PIN, &swConfig);
////    GPIO_EnableInterrupts(APP_WAKEUP_BUTTON_GPIO, 1U << APP_WAKEUP_BUTTON_GPIO_PIN);

//    while (1)
//    {
//        freq = CLOCK_GetFreq(kCLOCK_CpuClk);

//        PRINTF("\r\n########## Power Mode Switch Demo (build %s) ###########\n\r\n", __DATE__);//电源管理选择 DEMO
//        PRINTF("    Core Clock = %dHz \r\n", freq);

//        APP_ShowPowerMode(s_curRunMode);//系统时钟打印

//        PRINTF("\r\nSelect the desired operation \n\r\n");//选择所需的操作
//        PRINTF("Press  %c for enter: Over RUN       - System Over Run mode (600MHz)\r\n",
//               (uint8_t)'A' + (uint8_t)LPM_PowerModeOverRun);
//        PRINTF("Press  %c for enter: Full RUN       - System Full Run mode (528MHz)\r\n",
//               (uint8_t)'A' + (uint8_t)LPM_PowerModeFullRun);
//        PRINTF("Press  %c for enter: Low Speed RUN  - System Low Speed Run mode (132MHz)\r\n",
//               (uint8_t)'A' + (uint8_t)LPM_PowerModeLowSpeedRun);
//        PRINTF("Press  %c for enter: Low Power RUN  - System Low Power Run mode (24MHz)\r\n",
//               (uint8_t)'A' + (uint8_t)LPM_PowerModeLowPowerRun);
//        PRINTF("Press  %c for enter: System Idle    - System Wait mode\r\n",
//               (uint8_t)'A' + (uint8_t)LPM_PowerModeSysIdle);
//        PRINTF("Press  %c for enter: Low Power Idle - Low Power Idle mode\r\n",
//               (uint8_t)'A' + (uint8_t)LPM_PowerModeLPIdle);
//        PRINTF("Press  %c for enter: Suspend        - Suspend mode\r\n", (uint8_t)'A' + (uint8_t)LPM_PowerModeSuspend);
//        PRINTF("Press  %c for enter: SNVS           - Shutdown the system\r\n",
//               (uint8_t)'A' + (uint8_t)LPM_PowerModeSNVS);
//							 
//							 

//        PRINTF("\r\nWaiting for power mode select..\r\n\r\n");//等待电源管理模式选择 ..

//        /* 等待用户应答  卡死接收串口消息 */
//        ch = GETCHAR();

//        if ((ch >= 'a') && (ch <= 'z'))
//        {
//            ch -= 'a' - 'A';
//        }

//        s_targetPowerMode = (lpm_power_mode_t)(ch - 'A');

//        if (s_targetPowerMode <= LPM_PowerModeSNVS)
//        {
//            if (!LPM_SetPowerMode(s_targetPowerMode))
//            {
//                PRINTF("Some task doesn't allow to enter mode %s\r\n", s_modeNames[s_targetPowerMode]);//某些任务不允许进入模式
//            }
//            else
//            {
//                if (s_targetPowerMode <= LPM_PowerModeRunEnd)
//                {
//                    switch (s_targetPowerMode)
//                    {
//                    case LPM_PowerModeOverRun:
//                        LPM_SystemOverRun();
//                        break;
//                    case LPM_PowerModeFullRun:
//                        LPM_SystemFullRun();
//                        break;
//                    case LPM_PowerModeLowSpeedRun:
//                        LPM_SystemLowSpeedRun();
//                        break;
//                    case LPM_PowerModeLowPowerRun:
//                        LPM_SystemLowPowerRun();
//                        break;
//                    default:
//                        break;
//                    }
//                    s_curRunMode = s_targetPowerMode;
//                    continue;
//                }
////                else if (LPM_PowerModeSNVS == s_targetPowerMode)
////                {
////                    PRINTF("Now shutting down the system...\r\n");//现在关闭系统...
////                    APP_PowerPreSwitchHook(s_targetPowerMode);
////                    CLOCK_EnableClock(kCLOCK_SnvsHp);
////                    SNVS->LPCR |= SNVS_LPCR_DP_EN_MASK;
////                    SNVS->LPCR |= SNVS_LPCR_TOP_MASK;
////                    while (1) /* 关闭 */
////                    {
////                    }
////                }
////                else
////                {
////                    APP_GetWakeupConfig(s_targetPowerMode);
////                    APP_SetWakeupConfig(s_targetPowerMode);
////                    xSemaphoreTake(s_wakeupSig, portMAX_DELAY);
////                }
//                LPM_SetPowerMode(s_curRunMode);
//                s_targetPowerMode = s_curRunMode;
//            }
//        }
//        PRINTF("\r\nNext loop\r\n");
//    }
//	
//	
//	
////    uint8_t ch;
////    uint32_t freq;

////    if (true != LPM_Init(s_curRunMode))
////    {
////        PRINTF("LPM Init Failed!\r\n");//初始化失败

////    }

////    while(1)
////    {
////        freq = CLOCK_GetFreq(kCLOCK_CpuClk);

////        PRINTF("\r\n########## Power Mode Switch Demo (build %s) ###########\n\r\n", __DATE__);//电源管理选择 DEMO
////        //PRINTF("    Core Clock = %dHz \r\n", freq);

//////    APP_ShowPowerMode(s_curRunMode);//系统时钟打印

//////        PRINTF("\r\nSelect the desired operation \n\r\n");//选择所需的操作
//////        PRINTF("Press  %c for enter: Over RUN       - System Over Run mode (600MHz)\r\n",
//////               (uint8_t)'A' + (uint8_t)LPM_PowerModeOverRun);
//////        PRINTF("Press  %c for enter: Full RUN       - System Full Run mode (528MHz)\r\n",
//////               (uint8_t)'A' + (uint8_t)LPM_PowerModeFullRun);
//////        PRINTF("Press  %c for enter: Low Speed RUN  - System Low Speed Run mode (132MHz)\r\n",
//////               (uint8_t)'A' + (uint8_t)LPM_PowerModeLowSpeedRun);
//////        PRINTF("Press  %c for enter: Low Power RUN  - System Low Power Run mode (24MHz)\r\n",
//////               (uint8_t)'A' + (uint8_t)LPM_PowerModeLowPowerRun);
//////        PRINTF("Press  %c for enter: System Idle    - System Wait mode\r\n",
//////               (uint8_t)'A' + (uint8_t)LPM_PowerModeSysIdle);
//////        PRINTF("Press  %c for enter: Low Power Idle - Low Power Idle mode\r\n",
//////               (uint8_t)'A' + (uint8_t)LPM_PowerModeLPIdle);
//////        PRINTF("Press  %c for enter: Suspend        - Suspend mode\r\n", (uint8_t)'A' + (uint8_t)LPM_PowerModeSuspend);
//////        PRINTF("Press  %c for enter: SNVS           - Shutdown the system\r\n",
//////               (uint8_t)'A' + (uint8_t)LPM_PowerModeSNVS);

//////        PRINTF("\r\nWaiting for power mode select..\r\n\r\n");//等待电源管理模式选择 ..


////        /* 等待用户应答  卡死接收串口消息 */
////        ch = GETCHAR();

////        if ((ch >= 'a') && (ch <= 'z'))
////        {
////            ch -= 'a' - 'A';
////        }

////        s_targetPowerMode = (lpm_power_mode_t)(ch - 'A');

////        if (s_targetPowerMode <= LPM_PowerModeSNVS)
////        {
////            if (!LPM_SetPowerMode(s_targetPowerMode))
////            {
////                PRINTF("Some task doesn't allow to enter mode %s\r\n", s_modeNames[s_targetPowerMode]);//某些任务不允许进入模式
////            }
////            else
////            {
////                if (s_targetPowerMode <= LPM_PowerModeRunEnd)
////                {
////                    switch (s_targetPowerMode)
////                    {
////                    case LPM_PowerModeOverRun:
////                        LPM_SystemOverRun();//600
////                        break;
////                    case LPM_PowerModeFullRun:
////                        LPM_SystemFullRun();//528
////                        break;
////                    case LPM_PowerModeLowSpeedRun:
////                        LPM_SystemLowSpeedRun();
////                        break;
////                    case LPM_PowerModeLowPowerRun:
////                        LPM_SystemLowPowerRun();
////                        break;
////                    default:
////                        break;
////                    }
////										PRINTF("    Core Clock = %dHz \r\n", freq);
////                    s_curRunMode = s_targetPowerMode;
////                    continue;
////                }


////            }


////        }
////    }
//}
