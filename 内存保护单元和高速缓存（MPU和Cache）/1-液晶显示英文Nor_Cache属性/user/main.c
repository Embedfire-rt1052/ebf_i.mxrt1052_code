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

#include "./led/bsp_led.h" 
#include "./lcd/bsp_lcd.h" 
#include "./systick/bsp_systick.h"
/*******************************************************************
 * Prototypes
 *******************************************************************/
__attribute__((section("MPU_TEST_SECTION"))) char mpu_test[6] = {'a','b','c','d','e','f'};
extern void LCD_Test(void);


/*******************************************************************
 * Variables
 *******************************************************************/


/*******************************************************************
 * Code
 *******************************************************************/
void MemManage_Handler(void)
{
	PRINTF("I am is MemManage_Handler \r\n");
}

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
		
		
		
		PRINTF("mpu_test is %s \r\n", mpu_test);
		PRINTF("read success\r\n");
		for(i=0; i<6; i++)
		{
			mpu_test[i] = mpu_test[i] - 32;
		}
		PRINTF("mpu_test is %s \r\n", mpu_test);
		PRINTF("write and read success\r\n");




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

/****************************END OF FILE**********************/

