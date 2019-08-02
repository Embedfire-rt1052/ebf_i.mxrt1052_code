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
#include "./lcd/bsp_camera_lcd.h"
#include "./camera/bsp_ov5640.h"
#include "./lcd/bsp_lcd.h"
#include "./systick/bsp_systick.h"
#include "./key/bsp_key.h"
/*******************************************************************
 * Code
 *******************************************************************/

//��ʾ֡�����ݣ�Ĭ�ϲ���ʾ����Ҫ��ʾʱ�����������Ϊ1����
#define FRAME_RATE_DISPLAY 1
/*���������*/
uint32_t Task_Delay[NumOfTask];
extern void LCD_Test(void);
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
uint8_t fps = 0;
int main(void)
{
	/* ��ʼ���ڴ汣����Ԫ */
	BOARD_ConfigMPU();
	/* ��ʼ������������ */
	BOARD_InitPins();
	/* ��ʼ��������ʱ�� */
	BOARD_BootClockRUN();
	/* ��ʼ�����Դ��� */
	BOARD_InitDebugConsole();
	/* ��ʼ��Һ���ӿ�*/
	BOARD_InitLcd();

	Key_GPIO_Config();
	/* ��ӡϵͳʱ�� */
	PRINTF("\r\n");
	PRINTF("*****��ӭʹ�� Ұ��i.MX RT1052 ������*****\r\n");
	PRINTF("CPU:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
	PRINTF("AHB:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AhbClk));
	PRINTF("SEMC:            %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
	PRINTF("SYSPLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
	PRINTF("SYSPLLPFD0:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk));
	PRINTF("SYSPLLPFD1:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd1Clk));
	PRINTF("SYSPLLPFD2:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
	PRINTF("SYSPLLPFD3:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd3Clk));
	PRINTF("CSI RGB565 example start...\r\n");
	SysTick_Init();
	Camera_Init();
	while (1)
	{
		ELCDIF_ClearInterruptStatus(APP_ELCDIF, kELCDIF_CurFrameDone);
		/* �ȴ��ǻ���������ڻ״̬ */
		while (!(kELCDIF_CurFrameDone & ELCDIF_GetInterruptStatus(APP_ELCDIF)))
		{
		}

		CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, activeFrameAddr);
		activeFrameAddr = inactiveFrameAddr;

		/* �ȴ���ȡ����֡����������ʾ */
		while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &inactiveFrameAddr))
		{
		}
		/*����֡��ַ*/
		ELCDIF_SetNextBufferAddr(APP_ELCDIF, LCD_SetOpenWindows_Pos(Set_Cam_mode(index_num), inactiveFrameAddr));
		/*���ݰ����仯���ı�����ͷ�ֱ���*/
		Cam_Config_Switch();
		//��ʾ֡�ʣ�Ĭ�ϲ���ʾ
#if FRAME_RATE_DISPLAY
		if (Task_Delay[0] == 0)
		{
			/*���֡��*/
			CAMERA_DEBUG("\r\n֡��:%.1f/s \r\n", (double)fps / 5.0);
			//����
			fps = 0;

			Task_Delay[0] = 5000; //��ֵÿ1ms���1������0�ſ������½�������
		}

#endif
	}
}

/****************************END OF FILE**********************/
