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

#include "./led/bsp_led.h"
#include "./lcd/bsp_lcd.h"
#include "./systick/bsp_systick.h"
#include "./freq/bsp_pwr_freq.h"
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


/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
//    /* 初始化内存保护单元 */
//    BOARD_ConfigMPU();
//    /* 初始化开发板引脚 */
//    BOARD_InitPins();
//    /* 初始化开发板时钟 */
//    BOARD_BootClockRUN();
//    /* 初始化调试串口 */
//    BOARD_InitDebugConsole();
//    /* 打印系统时钟 */
//    PRINTF("\r\n");
//    PRINTF("*****欢迎使用 野火i.MX RT1052 开发板*****\r\n");
//    PRINTF("CPU:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
//    PRINTF("AHB:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AhbClk));
//    PRINTF("SEMC:            %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
//    PRINTF("SYSPLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
//    PRINTF("SYSPLLPFD0:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk));
//    PRINTF("SYSPLLPFD1:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd1Clk));
//    PRINTF("SYSPLLPFD2:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
//    PRINTF("SYSPLLPFD3:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd3Clk));

    PRINTF("*****频率选择*****\r\n");

    BOARD_ConfigMPU();
#if defined(__MCUXPRESSO) && defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
#elif defined(__GNUC__) && defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
    Board_CopyToRam();
#endif
    BOARD_InitPins();
    BOARD_BootClockRUN();
//    Board_SdramInit(3, 3);

    /* Configure UART divider to default */
    CLOCK_SetMux(kCLOCK_UartMux, 1); /* Set UART source to OSC 24M */
    CLOCK_SetDiv(kCLOCK_UartDiv, 0); /* Set UART divider to 1 */

    /* Select OSC as PIT clock source */
    CLOCK_SetMux(kCLOCK_PerclkMux, 1); /* Set PIT clock source to OSC 24M */
    CLOCK_SetDiv(kCLOCK_PerclkDiv, 0); /* Set PIT clock divider to 1 */

#if !(defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1))
    /* In low power, better to use core pll for flexspi
       Use AXI clock for flexspi, AXI clock is from SEMC clock, 100MHz */
    CLOCK_SetMux(kCLOCK_FlexspiMux, 0x0);
    CLOCK_SetDiv(kCLOCK_FlexspiDiv, 0x0);

    /* In low power, better to use core pll for semc */
    CLOCK_SetMux(kCLOCK_SemcMux, 0x0); /* Use periph clock as semc clock source */
    CLOCK_SetDiv(kCLOCK_SemcDiv, 0x5); /* Semc 100MHz */
#endif

    BOARD_InitDebugConsole();



    PRINTF("\r\nCPU wakeup source 0x%x...\r\n", SRC->SRSR);
    PRINTF("\r\n***********************************************************\r\n");
    PRINTF("\tPower Mode Switch Demo for iMXRT1050\r\n");
    PRINTF("***********************************************************\r\n");

//    /* 初始化LED */
//    LED_GPIO_Config();

//    //RGB_LED_COLOR_BLUE;
//    RGB_LED_COLOR_RED;

    PrintfCoreClock();

    while(1)
    {

    }
}

/****************************END OF FILE**********************/
