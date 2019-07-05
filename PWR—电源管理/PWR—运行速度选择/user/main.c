/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   ELCDIF��Һ����ʾ����ʾӢ�ģ�
  ******************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  i.MXRT1052������
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
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
 * @brief ������
 */
int main(void)
{
    if (PGC->CPU_SR & PGC_CPU_SR_PSR_MASK)
    {
        /* MPU ���� */
        BOARD_ConfigMPU();

        /* ����ARM GCC����Ҫ���ı������渴�Ƶ�RAM�� ��Ҳ�������������������. */
#if defined(__MCUXPRESSO) && defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
#elif defined(__GNUC__) && defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
        Board_CopyToRam();
#endif

        /* Boot ROMȷʵ��ʼ����XTAL����������ֻ�����ⲿXTAL OSC freq */
        CLOCK_SetXtalFreq(BOARD_XTAL0_CLK_HZ);
        CLOCK_SetRtcXtalFreq(BOARD_XTAL32K_CLK_HZ);

        /* �ӹ������û���ʱ�ָ� */
        LPM_SystemResumeDsm();

        /* �ָ����� */
        IOMUXC_GPR->GPR4 = 0x00000000;
        IOMUXC_GPR->GPR7 = 0x00000000;
        IOMUXC_GPR->GPR8 = 0x00000000;
        IOMUXC_GPR->GPR12 = 0x00000000;

        CCM->CCR &= ~CCM_CCR_REG_BYPASS_COUNT_MASK;

        /* ���ñ�־�Դ�SUSPEND���ϵͳ���� */
        is_suspend_reset = 1;

        EnableIRQ(APP_WAKEUP_BUTTON_IRQ);
        EnableIRQ(APP_WAKEUP_GPT_IRQn);

        APP_PowerPostSwitchHook(LPM_PowerModeSuspend);

        PRINTF("\r\nWakeup from suspend reset!\r\n");//����ͣ���û��ѣ�

        /* ��ͣ���ú�ָ�ΪOver Run */
        LPM_SystemOverRun();
    }
    else
    {
        /* ��ʼ��Ӳ��. */
        BOARD_ConfigMPU();
#if defined(__MCUXPRESSO) && defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
#elif defined(__GNUC__) && defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
        Board_CopyToRam();
#endif
        BOARD_InitPins();
        BOARD_BootClockRUN();
        //Board_SdramInit(3, 3);

        /* ��UART��Ƶ������ΪĬ��ֵ */
        CLOCK_SetMux(kCLOCK_UartMux, 1); /* ��UARTԴ����ΪOSC 24M */
        CLOCK_SetDiv(kCLOCK_UartDiv, 0); /* ��UART��Ƶ������Ϊ1 */

        /* ѡ��OSC��ΪPITʱ��Դ */
        CLOCK_SetMux(kCLOCK_PerclkMux, 1); /* ��PITʱ��Դ����ΪOSC 24M */
        CLOCK_SetDiv(kCLOCK_PerclkDiv, 0); /* ��PITʱ�ӷ�Ƶ������Ϊ1 */

#if !(defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1))
        /* �ڵ͹����£����õ�ʹ��flexpi�ĺ���pll
        		ʹ��AXIʱ������flexspi��AXIʱ������SEMCʱ�ӣ�100MHz */
        CLOCK_SetMux(kCLOCK_FlexspiMux, 0x0);
        CLOCK_SetDiv(kCLOCK_FlexspiDiv, 0x0);

        /* �ڵ͹����£����ʹ�ú���pll����semc */
        CLOCK_SetMux(kCLOCK_SemcMux, 0x0); /* ʹ������ʱ����Ϊsemcʱ��Դ */
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
        PRINTF("LPM Init Failed!\r\n");//��ʼ��ʧ��
        return -1;
    }
    PowerModeSwitchTask();

    /* Ӧ�ó�����Զ����ﵽ��һ�㡣 */
    while(1)
    {

    }
}

/****************************END OF FILE**********************/
