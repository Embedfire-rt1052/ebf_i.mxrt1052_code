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
#include "fsl_elcdif.h"


#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "./systick/bsp_systick.h"

#include "./delay/core_delay.h"   
#include "./led/bsp_led.h" 
#include "./lcd/bsp_lcd.h" 
#include "./touch/bsp_touch_gtxx.h"
#include "./touch/bsp_i2c_touch.h"
#include "./touch/palette.h"
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
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
#if (!CPU_TS_INIT_IN_DELAY_FUNCTION)      
      //ʹ��ʱ�����ʱ����ǰ������ʹ�ܼ�����
      CPU_TS_TmrInit();
#endif
	  /* ��ʼ���ڴ汣����Ԫ */
    BOARD_ConfigMPU();
		/* ��ʼ������������ */
    BOARD_InitPins();
		/* ��ʼ��������ʱ�� */
    BOARD_BootClockRUN();
		/* ��ʼ�����Դ��� */
		BOARD_InitDebugConsole();
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

  	PRINTF("*****Һ����ʾӢ��*****\r\n");

    /* ��ʼ��LED */
    LED_GPIO_Config();
		/* ��ʼ��systick����֡�� */
		SysTick_Init();
    
    /* ������ʼ�� */
    GTP_Init_Panel();

    /* ��ʼ��LCD */
    LCD_Init(LCD_INTERRUPT_ENABLE);    
    
    RGB_LED_COLOR_BLUE;    
    CORE_BOARD_LED_ON;
    
    /*���û��庯��*/
    Palette_Init();

    while(1)
    {
      /* ���ִ����ź�ʱ���д��� */
			if(g_TouchPadInputSignal)
			{
				GTP_TouchProcess();    
				g_TouchPadInputSignal = false;

			}
    }
}

/****************************END OF FILE**********************/
