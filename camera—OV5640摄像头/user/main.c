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
#include "./lcd/bsp_camera_lcd.h"
#include "./camera/bsp_ov5640.h"
#include "./lcd/bsp_lcd.h"
#include "./systick/bsp_systick.h"
#include "./key/bsp_key.h"
/*******************************************************************
 * Code
 *******************************************************************/

extern void LCD_Test(void);
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
	__IO int num_test=0;
int main(void)
{
	  /* 初始化内存保护单元 */
    BOARD_ConfigMPU();
		/* 初始化开发板引脚 */
    BOARD_InitPins();
		/* 初始化开发板时钟 */
    BOARD_BootClockRUN();
		/* 初始化调试串口 */
		BOARD_InitDebugConsole();
		/* 初始化液晶接口*/
    BOARD_InitLcd();
	
		Key_GPIO_Config();
		/* 打印系统时钟 */
		PRINTF("\r\n");
		PRINTF("*****欢迎使用 野火i.MX RT1052 开发板*****\r\n");
		PRINTF("CPU:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
		PRINTF("AHB:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AhbClk));
		PRINTF("SEMC:            %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
		PRINTF("SYSPLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
		PRINTF("SYSPLLPFD0:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk));
		PRINTF("SYSPLLPFD1:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd1Clk));
		PRINTF("SYSPLLPFD2:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
		PRINTF("SYSPLLPFD3:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd3Clk));			
		PRINTF("CSI RGB565 example start...\r\n");		
		
		Camera_Init();

    while(1)
		{
		
				LCD_SetCursor(50, 50);
				ELCDIF_ClearInterruptStatus(APP_ELCDIF, kELCDIF_CurFrameDone);
        /* 等待非活动缓冲区处于活动状态 */
        while (!(kELCDIF_CurFrameDone & ELCDIF_GetInterruptStatus(APP_ELCDIF)))
        {
        }

        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, activeFrameAddr);
        activeFrameAddr = inactiveFrameAddr;

        /* 等待获取完整帧缓冲区以显示 */
        while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &inactiveFrameAddr))
        {
        }

				ELCDIF_SetNextBufferAddr(APP_ELCDIF, LCD_SetOpenWindows_Pos(-100, -80,inactiveFrameAddr));	//+ LCD_BPP*(-50 + (LCD_PIXEL_WIDTH*(0)))	
				Cam_Config_Switch();


		}			

}


/****************************END OF FILE**********************/
