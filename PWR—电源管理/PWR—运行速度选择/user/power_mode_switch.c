/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "power_mode_switch.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "lpm.h"

//#include "FreeRTOS.h"
//#include "task.h"
//#include "semphr.h"

#include "fsl_gpio.h"

#include "pin_mux.h"
#include "fsl_gpt.h"
#include "fsl_lpuart.h"
#include "fsl_iomuxc.h"
//#include "fsl_tickless_generic.h"
#include "fsl_semc.h"

#include "fsl_gpio.h"
#include "clock_config.h"
#include "fsl_dcdc.h"
#include "./bsp/nvic/bsp_nvic.h"
#include "./bsp/gpt/bsp_gpt.h"
#include "./led/bsp_led.h"
#include "./pwr/bsp_wakeup_button.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_SDRAM0_BASE_ADDRESS 0x80000000U

/*******************************************************************************
* Prototypes
******************************************************************************/
#if defined(__GNUC__)
void Board_CopyToRam();
#endif


void LpmListener(lpm_power_mode_t curMode, lpm_power_mode_t newMode);
/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t s_wakeupTimeout;            /* 唤醒超时. (Unit: Second) */
static wakeup_source_t s_wakeupSource; /* 唤醒来源.                 */

static lpm_power_mode_t s_targetPowerMode;//目标模式
static lpm_power_mode_t s_curRunMode = LPM_PowerModeOverRun;//当前模式    默认值为LPM_PowerModeOverRun
//static SemaphoreHandle_t s_wakeupSig;

volatile int wakeupSig=0;//代替系统中的信号量

//static const char *s_modeNames[] = {"Over RUN",    "Full Run",       "Low Speed Run", "Low Power Run",
//                                    "System Idle", "Low Power Idle", "Suspend",       "SNVS"
//                                   };
//static const char *s_modeNames[] = {"过度运行",    "满载运行",       "低速运行", "低功耗运行",
//                                    "系统空闲", "低功耗空闲", "暂停模式",       "SNVS"
//                                   };

int32_t is_suspend_reset = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/

lpm_power_mode_t Fun_s_curRunMode()
{
    return s_curRunMode;
}

void BOARD_SetLPClockGate(void)
{
    CCM->CCGR0 = 0x014000C5U;
    CCM->CCGR1 = 0x54100000U;
    CCM->CCGR2 = 0x00150010U;
    CCM->CCGR3 = 0x50040110U;

    CCM->CCGR4 = 0x00005514U;
    CCM->CCGR5 = 0x51001105U;
    /* 我们可以在需要配置时启用DCDC，并在配置后关闭它 */
    CCM->CCGR6 = 0x00540540U;
}

#if defined(__MCUXPRESSO) && defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
#elif defined(__GNUC__) && defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
extern uint32_t __ram_function_flash_start[];
#define __RAM_FUNCTION_FLASH_START __ram_function_flash_start
extern uint32_t __ram_function_ram_start[];
#define __RAM_FUNCTION_RAM_START __ram_function_ram_start
extern uint32_t __ram_function_size[];
#define __RAM_FUNCTION_SIZE __ram_function_size
void Board_CopyToRam()
{
    unsigned char *source;
    unsigned char *destiny;
    unsigned int size;

    source = (unsigned char *)(__RAM_FUNCTION_FLASH_START);
    destiny = (unsigned char *)(__RAM_FUNCTION_RAM_START);
    size = (unsigned long)(__RAM_FUNCTION_SIZE);

    while (size--)
    {
        *destiny++ = *source++;
    }
}
#endif

void Board_SdramInitSequence(uint32_t bl, uint32_t cl)
{
    SEMC_SendIPCommand(SEMC, kSEMC_MemType_SDRAM, BOARD_SDRAM0_BASE_ADDRESS, kSEMC_SDRAMCM_Prechargeall, 0, NULL);
    SEMC_SendIPCommand(SEMC, kSEMC_MemType_SDRAM, BOARD_SDRAM0_BASE_ADDRESS, kSEMC_SDRAMCM_AutoRefresh, 0, NULL);
    SEMC_SendIPCommand(SEMC, kSEMC_MemType_SDRAM, BOARD_SDRAM0_BASE_ADDRESS, kSEMC_SDRAMCM_AutoRefresh, 0, NULL);
    SEMC_SendIPCommand(SEMC, kSEMC_MemType_SDRAM, BOARD_SDRAM0_BASE_ADDRESS, kSEMC_SDRAMCM_Modeset, bl | (cl << 4),
                       NULL);
}

//static void Board_SdramInit(uint32_t bl, uint32_t cl)
void Board_SdramInit(uint32_t bl, uint32_t cl)
{
    CLOCK_EnableClock(kCLOCK_Semc);

    SEMC->MCR &= ~SEMC_MCR_MDIS_MASK;

    SEMC->MCR = 0x10000004;

    SEMC->BMCR0 = 0x00030524;

    SEMC->BMCR1 = 0x06030524;

    SEMC->BR[0] = (BOARD_SDRAM0_BASE_ADDRESS & 0xfffff000) | (SEMC_BR_MS(0xD)) | (SEMC_BR_VLD(0x1));
    SEMC->IOCR |= SEMC_IOCR_MUX_CSX0(1); // 将SEMC_CCSX0配置为SDRAM_CS1

    SEMC->SDRAMCR0 &= (~SEMC_SDRAMCR0_BL_MASK) | (~SEMC_SDRAMCR0_CL_MASK);
    SEMC->SDRAMCR0 = SEMC_SDRAMCR0_BL(bl) | SEMC_SDRAMCR0_CL(cl) | 0x301;
    SEMC->SDRAMCR1 = 0x652922;
    SEMC->SDRAMCR2 = 0x10920;
    SEMC->SDRAMCR3 = 0x50210A08;

    SEMC->DBICR0 = 0x00000021;
    SEMC->DBICR1 = 0x00888888;
    SEMC->IPCR1 = 0x2;
    SEMC->IPCR2 = 0;

    Board_SdramInitSequence(bl, cl);
}

void WAKEUP_GPT_IRQn_HANDLER(void)
{
    GPT_ClearStatusFlags(WAKEUP_GPT_BASE, kGPT_OutputCompare1Flag);
    GPT_StopTimer(WAKEUP_GPT_BASE);
    LPM_DisableWakeupSource(WAKEUP_GPT_IRQn);

    if (!is_suspend_reset)
    {
        wakeupSig=1;
//        xSemaphoreGiveFromISR(s_wakeupSig, NULL);
//        portYIELD_FROM_ISR(pdTRUE);
    }
   // __DSB();
}

void WAKEUP_BUTTON_IRQ_HANDLER(void)
{
    if ((1U << WAKEUP_BUTTON_GPIO_PIN) & GPIO_GetPinsInterruptFlags(WAKEUP_BUTTON_GPIO))
    {
        /* 禁用中断. */
        GPIO_DisableInterrupts(WAKEUP_BUTTON_GPIO, 1U << WAKEUP_BUTTON_GPIO_PIN);
        GPIO_ClearPinsInterruptFlags(WAKEUP_BUTTON_GPIO, 1U << WAKEUP_BUTTON_GPIO_PIN);
        LPM_DisableWakeupSource(WAKEUP_BUTTON_IRQ);
    }

    if (!is_suspend_reset)
    {
        wakeupSig = 1;
//        xSemaphoreGiveFromISR(s_wakeupSig, NULL);
//        portYIELD_FROM_ISR(pdTRUE);
    }
   // __DSB();
}//

void PowerPreSwitchHook(lpm_power_mode_t targetMode)
{
    if (targetMode == LPM_PowerModeSuspend || targetMode == LPM_PowerModeSNVS)
    {
        /* 等待调试控制台输出完成. */
        while (!(kLPUART_TransmissionCompleteFlag & LPUART_GetStatusFlags((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR)))
        {
        }
        DbgConsole_Deinit();

        /*
         * 设置引脚漏电流。
         * 调试控制台RX引脚：设置为pinmux至GPIO输入。
         * 调试控制台TX引脚：不需要更改。
         */
        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_13_GPIO1_IO13, 0);
        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_13_GPIO1_IO13, IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                            IOMUXC_SW_PAD_CTL_PAD_PUS(2) |
                            IOMUXC_SW_PAD_CTL_PAD_PUE_MASK);
    }
}

void PowerPostSwitchHook(lpm_power_mode_t targetMode)
{
    if (targetMode == LPM_PowerModeSuspend)
    {
        /*
         * 调试控制台RX引脚设置为GPIO输入，需要重新配置pinmux.
         * 调试控制台TX引脚：不需要更改.
         */
        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_13_LPUART1_RX, 0);
        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_13_LPUART1_RX, IOMUXC_SW_PAD_CTL_PAD_SPEED(2));

        BOARD_InitDebugConsole();
    }
}

lpm_power_mode_t GetLPMPowerMode(void)
{
    return s_targetPowerMode;
}

/*!
 * @brief 获取用户关于唤醒超时的输入
 */
static uint8_t GetWakeupTimeout(void)
{
    uint8_t timeout;

    while (1)
    {
        PRINTF("Select the wake up timeout in seconds.\r\n");//选择唤醒源
        PRINTF("The allowed range is 1s ~ 9s.\r\n");//输入范围 1s-9s
        PRINTF("Eg. enter 5 to wake up in 5 seconds.\r\n");//输入5s  将在五秒内唤醒
        PRINTF("\r\nWaiting for input timeout value...\r\n\r\n");//等待输入时间超时数值

        timeout = GETCHAR();
        PRINTF("%c\r\n", timeout);
        if ((timeout > '0') && (timeout <= '9'))
        {
            return timeout - '0';
        }
        PRINTF("Wrong value!\r\n");//数值错误
    }
}

/* 通过用户输入选择需要唤醒的源。. */
static wakeup_source_t GetWakeupSource(lpm_power_mode_t targetMode)
{
    uint8_t ch;

    while (1)
    {
        LpmListener(s_curRunMode,s_targetPowerMode);

        PRINTF("Select the wake up source:\r\n");//选择唤醒源
        PRINTF("Press T for GPT - GPT Timer\r\n");//T
        PRINTF("Press S for switch/button %s. \r\n", WAKEUP_BUTTON_NAME);//S

        PRINTF("\r\nWaiting for key press..\r\n\r\n");//等待按键

        ch = GETCHAR();

        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }

        if (ch == 'T')
        {
            return kWakeupSourceGPT;
        }
        else if (ch == 'S')
        {
            return kWakeupSourcePin;
        }
        else
        {
            PRINTF("Wrong value!\r\n");//数值错误
        }
    }
}

/* 获取唤醒超时和唤醒源. */
static void GetWakeupConfig(lpm_power_mode_t targetMode)
{
    /* 等待用户输入获取唤醒源. */
    s_wakeupSource = GetWakeupSource(targetMode);

    if (kWakeupSourceGPT == s_wakeupSource)
    {
        /* 唤醒源是GPT，用户应输入唤醒超时值. */
        s_wakeupTimeout = GetWakeupTimeout();
        PRINTF("Will wakeup in %d seconds.\r\n", s_wakeupTimeout);//将在 %d 秒唤醒
    }

    else
    {
        PRINTF("Switch %s from off to on to wake up.\r\n", WAKEUP_BUTTON_NAME);//从关闭切换为开启以唤醒
    }
}

static void SetWakeupConfig(lpm_power_mode_t targetMode)
{
    /* 设置GPT超时值. */
    if (kWakeupSourceGPT == s_wakeupSource)
    {
        GPT_StopTimer(WAKEUP_GPT_BASE);
        /* 更新比较channel1值将重置计数器 */
        GPT_SetOutputCompareValue(WAKEUP_GPT_BASE, kGPT_OutputCompare_Channel1,
                                  (CLOCK_GetRtcFreq() * s_wakeupTimeout) - 1U);

        /* 启用GPT输出Compare1中断 */
        GPT_EnableInterrupts(WAKEUP_GPT_BASE, kGPT_OutputCompare1InterruptEnable);
        //NVIC_SetPriority(WAKEUP_GPT_IRQn, 0 + 2);
        set_IRQn_Priority(WAKEUP_GPT_IRQn,Group4_PreemptPriority_6, Group4_SubPriority_0);
        EnableIRQ(WAKEUP_GPT_IRQn);

        /* 重启计时器 */
        GPT_StartTimer(WAKEUP_GPT_BASE);

        LPM_EnableWakeupSource(WAKEUP_GPT_IRQn);
    }
    else
    {
        GPIO_ClearPinsInterruptFlags(WAKEUP_BUTTON_GPIO, 1U << WAKEUP_BUTTON_GPIO_PIN);
        /* 使能GPIO引脚中断 */
        GPIO_EnableInterrupts(WAKEUP_BUTTON_GPIO, 1U << WAKEUP_BUTTON_GPIO_PIN);
        //NVIC_SetPriority(WAKEUP_BUTTON_IRQ, 0 + 2);
        set_IRQn_Priority(WAKEUP_BUTTON_IRQ,Group4_PreemptPriority_6, Group4_SubPriority_0);

        /* 启用中断 */
        EnableIRQ(WAKEUP_BUTTON_IRQ);
        /* 启用GPC中断 */
        LPM_EnableWakeupSource(WAKEUP_BUTTON_IRQ);
    }
    is_suspend_reset = 0;
}
/*
打印时钟频率
 */
static void PrintRunFrequency(int32_t run_freq_only)
{
    PRINTF("\r\n");
    PRINTF("***********************************************************\r\n");
    PRINTF("CPU:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
    PRINTF("AHB:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AhbClk));
    PRINTF("SEMC:            %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
    PRINTF("IPG:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_IpgClk));
    PRINTF("OSC:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_OscClk));
    PRINTF("RTC:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_RtcClk));
    PRINTF("ARMPLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_ArmPllClk));
    if (!run_freq_only)
    {
        PRINTF("USB1PLL:         %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllClk));
        PRINTF("USB1PLLPFD0:     %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk));
        PRINTF("USB1PLLPFD1:     %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd1Clk));
        PRINTF("USB1PLLPFD2:     %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd2Clk));
        PRINTF("USB1PLLPFD3:     %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd3Clk));
        PRINTF("USB2PLL:         %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb2PllClk));
        PRINTF("SYSPLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
        PRINTF("SYSPLLPFD0:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk));
        PRINTF("SYSPLLPFD1:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd1Clk));
        PRINTF("SYSPLLPFD2:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
        PRINTF("SYSPLLPFD3:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd3Clk));
        PRINTF("ENETPLL0:        %d Hz\r\n", CLOCK_GetFreq(kCLOCK_EnetPll0Clk));
        PRINTF("ENETPLL1:        %d Hz\r\n", CLOCK_GetFreq(kCLOCK_EnetPll1Clk));
        PRINTF("AUDIOPLL:        %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AudioPllClk));
        PRINTF("VIDEOPLL:        %d Hz\r\n", CLOCK_GetFreq(kCLOCK_VideoPllClk));
    }
    PRINTF("***********************************************************\r\n");
    PRINTF("\r\n");
}

/*
显示
频率 以及
当前的电源管理模式
 */
static void ShowPowerMode(lpm_power_mode_t powerMode)
{
    if (powerMode <= LPM_PowerModeEnd)
    {
        PRINTF("    Power mode: %s\r\n", s_modeNames[powerMode]);//电源管理模式
        PrintRunFrequency(1);
    }
    else
    {
        assert(0);
    }
}

/* 在空闲任务中调用 */
//static bool LpmListener(lpm_power_mode_t curMode, lpm_power_mode_t newMode)
//{
//    //PRINTF("WorkingTask %d: Transfer from %s to %s\r\n", (uint32_t)data, s_modeNames[curMode], s_modeNames[newMode]);
//	PRINTF("Working is: Transfer from %s to %s\r\n",s_modeNames[curMode], s_modeNames[newMode]);

//    /*为此模式更改做必要的准备 */

//    return true; /* 允许此开关 */
//}
void LpmListener(lpm_power_mode_t curMode, lpm_power_mode_t newMode)
{
    //PRINTF("WorkingTask %d: Transfer from %s to %s\r\n", (uint32_t)data, s_modeNames[curMode], s_modeNames[newMode]);
    PRINTF("Working is: Transfer from %s to %s\r\n",s_modeNames[curMode], s_modeNames[newMode]);

    /*为此模式更改做必要的准备 */
}

/*!
 * @brief 电源模式选择 主任务.
 */

__IO int test_a=0;

void PowerModeSwitchTask()
{
    uint8_t ch=0;
    uint32_t freq;    wakeupSig=0;
    gpt_config_t gptConfig;
    /* 定义输入开关引脚的init结构 */
    gpio_pin_config_t swConfig = {
        kGPIO_DigitalInput, 0, kGPIO_IntRisingEdge,
    };

    /* 唤醒初始GPT作为FreeRTOS告诉我们 */
    GPT_GetDefaultConfig(&gptConfig);
    gptConfig.clockSource = kGPT_ClockSource_LowFreq; /* 32K RTC OSC */
    // gptConfig.enableMode = false;                     /* 停止时保持计数器 */
    gptConfig.enableMode = true; /* 停止时不要保持计数器 */
    gptConfig.enableRunInDoze = true;
    /* 初始化GPT模块 */
    GPT_Init(WAKEUP_GPT_BASE, &gptConfig);
    GPT_SetClockDivider(WAKEUP_GPT_BASE, 1);

    /* Init输入开关GPIO. */
    GPIO_PinInit(WAKEUP_BUTTON_GPIO, WAKEUP_BUTTON_GPIO_PIN, &swConfig);
    GPIO_EnableInterrupts(WAKEUP_BUTTON_GPIO, 1U << WAKEUP_BUTTON_GPIO_PIN);


//    GPT_Config();//GPT模块初始化配置
//    Wakeup_Button_Config();//唤醒按键中断初始化

    while (1)
    {
        /* 获取时钟陪频率 */
        freq = CLOCK_GetFreq(kCLOCK_CpuClk);

        PRINTF("\r\n########## Power Mode Switch Demo (build %s) ###########\n\r\n", __DATE__);//电源管理选择 DEMO
        PRINTF("    Core Clock = %dHz \r\n", freq);
        /* 打印系统相关的时钟 */
        ShowPowerMode(s_curRunMode);
        /* 打印 所需选择的操作 界面*/
        PRINTF("\r\nSelect the desired operation \n\r\n");//
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

        /* 等待接收用户应答消息 */
        ch = GETCHAR();

        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }
        /* 将字符串的序列对其到枚举，方便使用*/
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
                    /* 选择模式部分 时钟的频率模式选择 */
                    switch (s_targetPowerMode)
                    {
                    case LPM_PowerModeOverRun://600
                        LPM_SystemOverRun();
                        break;
                    case LPM_PowerModeFullRun://528
                        LPM_SystemFullRun();
                        break;
                    case LPM_PowerModeLowSpeedRun://132
                        LPM_SystemLowSpeedRun();
                        break;
                    case LPM_PowerModeLowPowerRun://240
                        LPM_SystemLowPowerRun();
                        break;
                    default:
                        break;
                    }
                    s_curRunMode = s_targetPowerMode;//传递目标模式
                    continue;
                }
                else if (LPM_PowerModeSNVS == s_targetPowerMode)
                {   /* 关机模式 只能使用外部按键唤醒 */
                    PRINTF("Now shutting down the system...\r\n");//现在关闭系统...
                    PowerPreSwitchHook(s_targetPowerMode);
                    CLOCK_EnableClock(kCLOCK_SnvsHp);
                    SNVS->LPCR |= SNVS_LPCR_DP_EN_MASK;
                    SNVS->LPCR |= SNVS_LPCR_TOP_MASK;
                    while (1) /* 关闭 */
                    {
                    }
                }
                else
                {
                    GetWakeupConfig(s_targetPowerMode);//获取唤醒源  GPT定时器 和 WAKEUP按键
                    SetWakeupConfig(s_targetPowerMode);//根据获取的唤醒源设置相关的配置
                    /*	根据枚举进入 对应的模式 进行配置 */
                    vPortPRE_SLEEP_PROCESSING();
                    while (!wakeupSig)
                    {
                        RGB_LED_COLOR_RED;//模式中 红灯亮起
                        //test_a++;
                    }
                    /*	根据枚举进入 对应的模式 取消配置 */
                    vPortPOST_SLEEP_PROCESSING();
                    RGB_LED_COLOR_OFF;//退出模式关闭电源灯
                    wakeupSig=0;//唤醒卡死标志清零
                }
                LPM_SetPowerMode(s_curRunMode);//设置电源管理模式
                s_targetPowerMode = s_curRunMode;
            }
        }
        PRINTF("\r\nNext loop\r\n");
    }
}

///*!
// * @brief 模拟工作任务。
// */
//static void WorkingTask(void *pvParameters)
//{
////    LPM_RegisterPowerListener(LpmListener, pvParameters);//空闲任务

////    for (;;)
////    {
////        /* 使用App任务逻辑替换vTaskDelay */
////        PRINTF("Task %d is working now\r\n", (uint32_t)pvParameters);
////        vTaskDelay(portMAX_DELAY);
////    }
//}


