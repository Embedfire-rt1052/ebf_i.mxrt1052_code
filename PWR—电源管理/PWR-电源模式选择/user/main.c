/**
 ******************************************************************
* @file    main.c
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
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
    /****************************第一部分**********************/
    __IO uint8_t ch;
    uint32_t freq;
    bool needSetWakeup; /* 需要设置唤醒. */
    gpt_config_t gptConfig;
    /* 定义输入开关管脚的初始化结构*/
    gpio_pin_config_t swConfig = {
        kGPIO_DigitalInput,
        0,
        kGPIO_IntRisingEdge,
    };
    /* 初始化内存保护单元 */
    BOARD_ConfigMPU();
    /* 初始化开发板引脚 */
    BOARD_InitPins();
    /* 初始化开发板时钟 */
    BOARD_BootClockRUN();
    /* 将UART分频器配置为默认值 */
    CLOCK_SetMux(kCLOCK_UartMux, 1); /*将UART源设置为OSC 24M */
    CLOCK_SetDiv(kCLOCK_UartDiv, 0); /* 将UART分频器设置为1 */
    /* 初始化调试串口 */
    BOARD_InitDebugConsole();
    /* 初始化SNVS_PMIC_STBY_REQ_GPIO5_IO02相关配置 */
    BOARD_Init_PMIC_STBY_REQ();
    /* 初始GPT模块用于唤醒 */
    GPT_GetDefaultConfig(&gptConfig);
    gptConfig.clockSource = kGPT_ClockSource_LowFreq; /* 32K RTC OSC */
    gptConfig.enableMode = true;                      /*停止时不要保持计数器 */
    gptConfig.enableRunInDoze = true;
    /* 初始化GPT模块 */
    GPT_Init(APP_WAKEUP_GPT_BASE, &gptConfig);
    GPT_SetClockDivider(APP_WAKEUP_GPT_BASE, 1);
    /* Init输入开关GPIO。 */
    GPIO_PinInit(APP_WAKEUP_BUTTON_GPIO, APP_WAKEUP_BUTTON_GPIO_PIN, &swConfig);
    /****************************第二部分**********************/
    /* 打印信息 */
		PRINTF("**************欢迎使用 野火i.MX RT1052 开发板**************\r\n");
    PRINTF("\r\nCPU wakeup source 0x%x...\r\n", SRC->SRSR);
    PRINTF("\r\n***********************************************************\r\n");
    PRINTF("\tPower Mode Switch Demo for %s\r\n", CPU_NAME);
    PRINTF("***********************************************************\r\n");
    APP_PrintRunFrequency(0); //输出运行时的频率
		 
    /* LPM初始化 */
    LPM_Init();
//    /*上电后将电源模式设置为过载 */
//		APP_SetRunMode(LPM_PowerModeOverRun);
//		LPM_OverDriveRun();
    while (1)
    {
        /****************************第三部分**********************/
        /* 获取时钟频率 */
        freq = CLOCK_GetFreq(kCLOCK_CpuClk);
        /* 打印时钟相关信息 */
        PRINTF("\r\n########## Power Mode Switch (build %s) ###########\n\r\n", __DATE__);
        PRINTF("    Core Clock = %dHz \r\n", freq);
        APP_ShowPowerMode(s_curRunMode);
        /* 打印界面信息 */
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
        /****************************第四部分**********************/
        /* 等待用户响应 */
        ch = GETCHAR();
        /* 判断是否为字母 */
        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }
        /* 将字母与枚举类型对应，算出当前电源模式 */
        s_targetPowerMode = (lpm_power_mode_t)(ch - 'A');
        /****************************第五部分**********************/
        /* 判断当前模式在枚举类型内 */
        if (s_targetPowerMode <= LPM_PowerModeEnd)
        {
            /*如果无法设置目标电源模式，则循环继续。 */
            if (!APP_CheckPowerMode(s_curRunMode, s_targetPowerMode))
            {
                continue;
            }
            /*如果目标模式是运行模式，则不需要设置唤醒源。 */
            if (s_targetPowerMode <= LPM_PowerModeLowPowerRun)
            {
                needSetWakeup = false;
            }
            else
            {
                needSetWakeup = true;
            }
            /* 判断是否需要唤醒 */
            /****************************第六部分**********************/
            if (needSetWakeup)
            {
                /* 获取唤醒超时和唤醒源的配置*/
                APP_GetWakeupConfig(s_targetPowerMode);
                /* 根据唤醒源配置 */
                APP_SetWakeupConfig(s_targetPowerMode);
            }
            /****************************第七部分**********************/
            APP_PowerPreSwitchHook(s_targetPowerMode);
            /* 电源模式选择 */
            APP_PowerModeSwitch(s_targetPowerMode);
            APP_PowerPostSwitchHook(s_targetPowerMode);
        }
        /* 一个循环结束 */
        PRINTF("\r\nNext loop\r\n");
    }
}

/****************************END OF FILE**********************/
