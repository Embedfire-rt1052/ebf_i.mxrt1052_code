/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   ELCDIF—液晶显示（显示英文）
  ******************************************************************
  * @attention
  *
  * 实验平台:野火  i.MXRT1052开发板
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************
  */
#include <stdio.h>

#include "fsl_debug_console.h"

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_semc.h"

#include "./led/bsp_led.h"
#include "./lcd/bsp_lcd.h"
#include "./systick/bsp_systick.h"
#include "./freq/bsp_pwr_freq.h"

#include "fsl_common.h"

#include "board.h"
#include "fsl_debug_console.h"
#include "lpm.h"
#include "fsl_gpc.h"
#include "fsl_dcdc.h"
#include "fsl_gpt.h"

#include "fsl_iomuxc.h"
#include "power_mode_switch.h"



/*******************************************************************
 * Prototypes
 *******************************************************************/

extern void LCD_Test(void);


/*******************************************************************
 * Variables
 *******************************************************************/


/*******************************************************************
 * Code
 *******************************************************************/


/*!
 * @brief 主函数
 */
int main(void)
{
    if (PGC->CPU_SR & PGC_CPU_SR_PSR_MASK)
    {
        /* MPU 配置 */
        BOARD_ConfigMPU();

        /* 对于ARM GCC，需要将文本从闪存复制到RAM。 这也可以在启动代码中完成. */
#if defined(__MCUXPRESSO) && defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
#elif defined(__GNUC__) && defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
        Board_CopyToRam();
#endif

        /* Boot ROM确实初始化了XTAL，这里我们只设置外部XTAL OSC freq */
        CLOCK_SetXtalFreq(BOARD_XTAL0_CLK_HZ);
        CLOCK_SetRtcXtalFreq(BOARD_XTAL32K_CLK_HZ);

        /* 从挂起重置唤醒时恢复 */
        LPM_SystemResumeDsm();

        /* 恢复握手 */
        IOMUXC_GPR->GPR4 = 0x00000000;
        IOMUXC_GPR->GPR7 = 0x00000000;
        IOMUXC_GPR->GPR8 = 0x00000000;
        IOMUXC_GPR->GPR12 = 0x00000000;

        CCM->CCR &= ~CCM_CCR_REG_BYPASS_COUNT_MASK;

        /* 设置标志以从SUSPEND标记系统重置 */
        is_suspend_reset = 1;

        EnableIRQ(APP_WAKEUP_BUTTON_IRQ);
        EnableIRQ(APP_WAKEUP_GPT_IRQn);

        APP_PowerPostSwitchHook(LPM_PowerModeSuspend);

        PRINTF("\r\nWakeup from suspend reset!\r\n");//从暂停重置唤醒！

        /* 暂停重置后恢复为Over Run */
        LPM_SystemOverRun();
    }
    else
    {
        /* 初始板硬件. */
        BOARD_ConfigMPU();
#if defined(__MCUXPRESSO) && defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
#elif defined(__GNUC__) && defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
        Board_CopyToRam();
#endif
        BOARD_InitPins();
        BOARD_BootClockRUN();
        //Board_SdramInit(3, 3);

        /* 将UART分频器配置为默认值 */
        CLOCK_SetMux(kCLOCK_UartMux, 1); /* 将UART源设置为OSC 24M */
        CLOCK_SetDiv(kCLOCK_UartDiv, 0); /* 将UART分频器设置为1 */

        /* 选择OSC作为PIT时钟源 */
        CLOCK_SetMux(kCLOCK_PerclkMux, 1); /* 将PIT时钟源设置为OSC 24M */
        CLOCK_SetDiv(kCLOCK_PerclkDiv, 0); /* 将PIT时钟分频器设置为1 */

#if !(defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1))
        /* 在低功耗下，更好地使用flexpi的核心pll
        		使用AXI时钟用于flexspi，AXI时钟来自SEMC时钟，100MHz */
        CLOCK_SetMux(kCLOCK_FlexspiMux, 0x0);
        CLOCK_SetDiv(kCLOCK_FlexspiDiv, 0x0);

        /* 在低功耗下，最好使用核心pll进行semc */
        CLOCK_SetMux(kCLOCK_SemcMux, 0x0); /* 使用外设时钟作为semc时钟源 */
        CLOCK_SetDiv(kCLOCK_SemcDiv, 0x5); /* Semc 100MHz */
#endif

        BOARD_InitDebugConsole();
    }

    // PRINTF("\r\nCPU wakeup source 0x%x...\r\n", SRC->SRSR);

    PRINTF("\r\n***********************************************************\r\n");
    PRINTF("\tPower Mode Switch Demo for iMXRT1050\r\n");
    PRINTF("***********************************************************\r\n");
    //APP_PrintRunFrequency(0);

    if (true != LPM_Init(Fun_s_curRunMode()))
    {
        PRINTF("LPM Init Failed!\r\n");//初始化失败
        return -1;
    }
    PowerModeSwitchTask();

    /* 应用程序永远不会达到这一点。 */
    while(1)
    {

    }
}

/****************************END OF FILE**********************/
