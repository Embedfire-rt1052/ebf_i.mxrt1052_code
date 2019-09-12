/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   MPU��ջ������
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

#include "./led/bsp_led.h" 
#include "./lcd/bsp_lcd.h" 
#include "./systick/bsp_systick.h"
/*******************************************************************
 * Prototypes
 *******************************************************************/


/*******************************************************************
 * Code
 *******************************************************************/
void MemManage_Handler(void)
{
	PRINTF("ջ��������� \r\n");
}

void MPU_for_test(void);

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	  int i = 0;
	  /* ��ʼ���ڴ汣����Ԫ */
    BOARD_ConfigMPU();
		/* ��ʼ������������ */
    BOARD_InitPins();
		/* ��ʼ��������ʱ�� */
    BOARD_BootClockRUN();
		/* ��ʼ�����Դ��� */
		BOARD_InitDebugConsole();
	
	  EnableIRQ(MemoryManagement_IRQn);
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
		
		/*����ջ���*/
    MPU_for_test();
		
		
    /* ��ʼ��LED */
    LED_GPIO_Config();
		/* ��ʼ��systick����֡�� */
		SysTick_Init();
    
    /* ��ʼ��LCD */
    LCD_Init(LCD_INTERRUPT_ENABLE);
    
    RGB_LED_COLOR_BLUE;
    
    while(1)
    {
      LCD_Test();
    }
}


void MPU_for_test(void)
{
	char data1[100] = "hello the world!";
	char data2[100] = "hello the world!";
	char data3[100] = "hello the world!";
	char data4[100] = "hello the world!";
	char data5[100] = "hello the world!";
	char data6[100] = "hello the world!";
	
	PRINTF("data1 = %s\r\n",data1);
	PRINTF("data2 = %s\r\n",data2);
	PRINTF("data3 = %s\r\n",data3);
	PRINTF("data4 = %s\r\n",data4);
	PRINTF("data5 = %s\r\n",data5);
	PRINTF("data6 = %s\r\n",data6);
}

/****************************END OF FILE**********************/






