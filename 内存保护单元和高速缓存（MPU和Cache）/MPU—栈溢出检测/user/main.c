/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   MPU—栈溢出检测
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
	PRINTF("栈溢出！！！ \r\n");
}

void MPU_for_test(void);

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	  int i = 0;
	  /* 初始化内存保护单元 */
    BOARD_ConfigMPU();
		/* 初始化开发板引脚 */
    BOARD_InitPins();
		/* 初始化开发板时钟 */
    BOARD_BootClockRUN();
		/* 初始化调试串口 */
		BOARD_InitDebugConsole();
	
	  EnableIRQ(MemoryManagement_IRQn);
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

  	PRINTF("*****液晶显示英文*****\r\n");
		
		/*测试栈溢出*/
    MPU_for_test();
		
		
    /* 初始化LED */
    LED_GPIO_Config();
		/* 初始化systick计算帧率 */
		SysTick_Init();
    
    /* 初始化LCD */
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






