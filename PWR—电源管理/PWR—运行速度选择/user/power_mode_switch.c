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
static uint8_t s_wakeupTimeout;            /* ���ѳ�ʱ. (Unit: Second) */
static wakeup_source_t s_wakeupSource; /* ������Դ.                 */

static lpm_power_mode_t s_targetPowerMode;//Ŀ��ģʽ
static lpm_power_mode_t s_curRunMode = LPM_PowerModeOverRun;//��ǰģʽ    Ĭ��ֵΪLPM_PowerModeOverRun
//static SemaphoreHandle_t s_wakeupSig;

volatile int wakeupSig=0;//����ϵͳ�е��ź���

//static const char *s_modeNames[] = {"Over RUN",    "Full Run",       "Low Speed Run", "Low Power Run",
//                                    "System Idle", "Low Power Idle", "Suspend",       "SNVS"
//                                   };
//static const char *s_modeNames[] = {"��������",    "��������",       "��������", "�͹�������",
//                                    "ϵͳ����", "�͹��Ŀ���", "��ͣģʽ",       "SNVS"
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
    /* ���ǿ�������Ҫ����ʱ����DCDC���������ú�ر��� */
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
    SEMC->IOCR |= SEMC_IOCR_MUX_CSX0(1); // ��SEMC_CCSX0����ΪSDRAM_CS1

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
        /* �����ж�. */
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
        /* �ȴ����Կ���̨������. */
        while (!(kLPUART_TransmissionCompleteFlag & LPUART_GetStatusFlags((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR)))
        {
        }
        DbgConsole_Deinit();

        /*
         * ��������©������
         * ���Կ���̨RX���ţ�����Ϊpinmux��GPIO���롣
         * ���Կ���̨TX���ţ�����Ҫ���ġ�
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
         * ���Կ���̨RX��������ΪGPIO���룬��Ҫ��������pinmux.
         * ���Կ���̨TX���ţ�����Ҫ����.
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
 * @brief ��ȡ�û����ڻ��ѳ�ʱ������
 */
static uint8_t GetWakeupTimeout(void)
{
    uint8_t timeout;

    while (1)
    {
        PRINTF("Select the wake up timeout in seconds.\r\n");//ѡ����Դ
        PRINTF("The allowed range is 1s ~ 9s.\r\n");//���뷶Χ 1s-9s
        PRINTF("Eg. enter 5 to wake up in 5 seconds.\r\n");//����5s  ���������ڻ���
        PRINTF("\r\nWaiting for input timeout value...\r\n\r\n");//�ȴ�����ʱ�䳬ʱ��ֵ

        timeout = GETCHAR();
        PRINTF("%c\r\n", timeout);
        if ((timeout > '0') && (timeout <= '9'))
        {
            return timeout - '0';
        }
        PRINTF("Wrong value!\r\n");//��ֵ����
    }
}

/* ͨ���û�����ѡ����Ҫ���ѵ�Դ��. */
static wakeup_source_t GetWakeupSource(lpm_power_mode_t targetMode)
{
    uint8_t ch;

    while (1)
    {
        LpmListener(s_curRunMode,s_targetPowerMode);

        PRINTF("Select the wake up source:\r\n");//ѡ����Դ
        PRINTF("Press T for GPT - GPT Timer\r\n");//T
        PRINTF("Press S for switch/button %s. \r\n", WAKEUP_BUTTON_NAME);//S

        PRINTF("\r\nWaiting for key press..\r\n\r\n");//�ȴ�����

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
            PRINTF("Wrong value!\r\n");//��ֵ����
        }
    }
}

/* ��ȡ���ѳ�ʱ�ͻ���Դ. */
static void GetWakeupConfig(lpm_power_mode_t targetMode)
{
    /* �ȴ��û������ȡ����Դ. */
    s_wakeupSource = GetWakeupSource(targetMode);

    if (kWakeupSourceGPT == s_wakeupSource)
    {
        /* ����Դ��GPT���û�Ӧ���뻽�ѳ�ʱֵ. */
        s_wakeupTimeout = GetWakeupTimeout();
        PRINTF("Will wakeup in %d seconds.\r\n", s_wakeupTimeout);//���� %d �뻽��
    }

    else
    {
        PRINTF("Switch %s from off to on to wake up.\r\n", WAKEUP_BUTTON_NAME);//�ӹر��л�Ϊ�����Ի���
    }
}

static void SetWakeupConfig(lpm_power_mode_t targetMode)
{
    /* ����GPT��ʱֵ. */
    if (kWakeupSourceGPT == s_wakeupSource)
    {
        GPT_StopTimer(WAKEUP_GPT_BASE);
        /* ���±Ƚ�channel1ֵ�����ü����� */
        GPT_SetOutputCompareValue(WAKEUP_GPT_BASE, kGPT_OutputCompare_Channel1,
                                  (CLOCK_GetRtcFreq() * s_wakeupTimeout) - 1U);

        /* ����GPT���Compare1�ж� */
        GPT_EnableInterrupts(WAKEUP_GPT_BASE, kGPT_OutputCompare1InterruptEnable);
        //NVIC_SetPriority(WAKEUP_GPT_IRQn, 0 + 2);
        set_IRQn_Priority(WAKEUP_GPT_IRQn,Group4_PreemptPriority_6, Group4_SubPriority_0);
        EnableIRQ(WAKEUP_GPT_IRQn);

        /* ������ʱ�� */
        GPT_StartTimer(WAKEUP_GPT_BASE);

        LPM_EnableWakeupSource(WAKEUP_GPT_IRQn);
    }
    else
    {
        GPIO_ClearPinsInterruptFlags(WAKEUP_BUTTON_GPIO, 1U << WAKEUP_BUTTON_GPIO_PIN);
        /* ʹ��GPIO�����ж� */
        GPIO_EnableInterrupts(WAKEUP_BUTTON_GPIO, 1U << WAKEUP_BUTTON_GPIO_PIN);
        //NVIC_SetPriority(WAKEUP_BUTTON_IRQ, 0 + 2);
        set_IRQn_Priority(WAKEUP_BUTTON_IRQ,Group4_PreemptPriority_6, Group4_SubPriority_0);

        /* �����ж� */
        EnableIRQ(WAKEUP_BUTTON_IRQ);
        /* ����GPC�ж� */
        LPM_EnableWakeupSource(WAKEUP_BUTTON_IRQ);
    }
    is_suspend_reset = 0;
}
/*
��ӡʱ��Ƶ��
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
��ʾ
Ƶ�� �Լ�
��ǰ�ĵ�Դ����ģʽ
 */
static void ShowPowerMode(lpm_power_mode_t powerMode)
{
    if (powerMode <= LPM_PowerModeEnd)
    {
        PRINTF("    Power mode: %s\r\n", s_modeNames[powerMode]);//��Դ����ģʽ
        PrintRunFrequency(1);
    }
    else
    {
        assert(0);
    }
}

/* �ڿ��������е��� */
//static bool LpmListener(lpm_power_mode_t curMode, lpm_power_mode_t newMode)
//{
//    //PRINTF("WorkingTask %d: Transfer from %s to %s\r\n", (uint32_t)data, s_modeNames[curMode], s_modeNames[newMode]);
//	PRINTF("Working is: Transfer from %s to %s\r\n",s_modeNames[curMode], s_modeNames[newMode]);

//    /*Ϊ��ģʽ��������Ҫ��׼�� */

//    return true; /* ����˿��� */
//}
void LpmListener(lpm_power_mode_t curMode, lpm_power_mode_t newMode)
{
    //PRINTF("WorkingTask %d: Transfer from %s to %s\r\n", (uint32_t)data, s_modeNames[curMode], s_modeNames[newMode]);
    PRINTF("Working is: Transfer from %s to %s\r\n",s_modeNames[curMode], s_modeNames[newMode]);

    /*Ϊ��ģʽ��������Ҫ��׼�� */
}

/*!
 * @brief ��Դģʽѡ�� ������.
 */

__IO int test_a=0;

void PowerModeSwitchTask()
{
    uint8_t ch=0;
    uint32_t freq;    wakeupSig=0;
    gpt_config_t gptConfig;
    /* �������뿪�����ŵ�init�ṹ */
    gpio_pin_config_t swConfig = {
        kGPIO_DigitalInput, 0, kGPIO_IntRisingEdge,
    };

    /* ���ѳ�ʼGPT��ΪFreeRTOS�������� */
    GPT_GetDefaultConfig(&gptConfig);
    gptConfig.clockSource = kGPT_ClockSource_LowFreq; /* 32K RTC OSC */
    // gptConfig.enableMode = false;                     /* ֹͣʱ���ּ����� */
    gptConfig.enableMode = true; /* ֹͣʱ��Ҫ���ּ����� */
    gptConfig.enableRunInDoze = true;
    /* ��ʼ��GPTģ�� */
    GPT_Init(WAKEUP_GPT_BASE, &gptConfig);
    GPT_SetClockDivider(WAKEUP_GPT_BASE, 1);

    /* Init���뿪��GPIO. */
    GPIO_PinInit(WAKEUP_BUTTON_GPIO, WAKEUP_BUTTON_GPIO_PIN, &swConfig);
    GPIO_EnableInterrupts(WAKEUP_BUTTON_GPIO, 1U << WAKEUP_BUTTON_GPIO_PIN);


//    GPT_Config();//GPTģ���ʼ������
//    Wakeup_Button_Config();//���Ѱ����жϳ�ʼ��

    while (1)
    {
        /* ��ȡʱ����Ƶ�� */
        freq = CLOCK_GetFreq(kCLOCK_CpuClk);

        PRINTF("\r\n########## Power Mode Switch Demo (build %s) ###########\n\r\n", __DATE__);//��Դ����ѡ�� DEMO
        PRINTF("    Core Clock = %dHz \r\n", freq);
        /* ��ӡϵͳ��ص�ʱ�� */
        ShowPowerMode(s_curRunMode);
        /* ��ӡ ����ѡ��Ĳ��� ����*/
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

        PRINTF("\r\nWaiting for power mode select..\r\n\r\n");//�ȴ���Դ����ģʽѡ�� ..

        /* �ȴ������û�Ӧ����Ϣ */
        ch = GETCHAR();

        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }
        /* ���ַ��������ж��䵽ö�٣�����ʹ��*/
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
                    /* ѡ��ģʽ���� ʱ�ӵ�Ƶ��ģʽѡ�� */
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
                    s_curRunMode = s_targetPowerMode;//����Ŀ��ģʽ
                    continue;
                }
                else if (LPM_PowerModeSNVS == s_targetPowerMode)
                {   /* �ػ�ģʽ ֻ��ʹ���ⲿ�������� */
                    PRINTF("Now shutting down the system...\r\n");//���ڹر�ϵͳ...
                    PowerPreSwitchHook(s_targetPowerMode);
                    CLOCK_EnableClock(kCLOCK_SnvsHp);
                    SNVS->LPCR |= SNVS_LPCR_DP_EN_MASK;
                    SNVS->LPCR |= SNVS_LPCR_TOP_MASK;
                    while (1) /* �ر� */
                    {
                    }
                }
                else
                {
                    GetWakeupConfig(s_targetPowerMode);//��ȡ����Դ  GPT��ʱ�� �� WAKEUP����
                    SetWakeupConfig(s_targetPowerMode);//���ݻ�ȡ�Ļ���Դ������ص�����
                    /*	����ö�ٽ��� ��Ӧ��ģʽ �������� */
                    vPortPRE_SLEEP_PROCESSING();
                    while (!wakeupSig)
                    {
                        RGB_LED_COLOR_RED;//ģʽ�� �������
                        //test_a++;
                    }
                    /*	����ö�ٽ��� ��Ӧ��ģʽ ȡ������ */
                    vPortPOST_SLEEP_PROCESSING();
                    RGB_LED_COLOR_OFF;//�˳�ģʽ�رյ�Դ��
                    wakeupSig=0;//���ѿ�����־����
                }
                LPM_SetPowerMode(s_curRunMode);//���õ�Դ����ģʽ
                s_targetPowerMode = s_curRunMode;
            }
        }
        PRINTF("\r\nNext loop\r\n");
    }
}

///*!
// * @brief ģ�⹤������
// */
//static void WorkingTask(void *pvParameters)
//{
////    LPM_RegisterPowerListener(LpmListener, pvParameters);//��������

////    for (;;)
////    {
////        /* ʹ��App�����߼��滻vTaskDelay */
////        PRINTF("Task %d is working now\r\n", (uint32_t)pvParameters);
////        vTaskDelay(portMAX_DELAY);
////    }
//}


