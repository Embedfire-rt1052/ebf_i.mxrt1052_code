/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   液晶显示英文
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

/*******************************************************************
 * Prototypes
 *******************************************************************/
/**
 * @brief 延时一段时间
 */
void delay(uint32_t count);

void LCD_Test(void);


/*******************************************************************
 * Variables
 *******************************************************************/


/*******************************************************************
 * Code
 *******************************************************************/
/**
 * @note 本函数在不同的优化模式下延时时间不同，
 *       如flexspi_nor_debug和flexspi_nor_release版本的程序中，
 *       flexspi_nor_release版本的延时要短得多  
 */ 
void delay(uint32_t count)
{
    volatile uint32_t i = 0;
    for (i = 0; i < count; ++i)
    {
        __asm("NOP"); /* 调用nop空指令 */
    }
}


/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
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

    /* 初始化LED */
    LED_GPIO_Config();
    
    /* 初始化LCD */
    LCD_Init(LCD_INTERRUPT_DISABLE);
    
    RGB_LED_COLOR_BLUE;

    LCD_PXP_Config();
    
    LCD_Test();

    APP_Scale();

    while(1)
    {
      
    }
}



/*用于测试各种液晶的函数*/
void LCD_Test(void)
{
	/*演示显示变量*/
	static uint8_t testCNT = 0;	
	char dispBuff[100];
	
	testCNT++;	
	
  LCD_Clear(LCD_COLOR_BLACK);	/* 清屏，显示全黑 */

	/*设置字体颜色及字体的背景颜色*/
  LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
	
	/*选择字体*/
  LCD_SetFont(&Font24x48);

  LCD_DisplayStringLine(LINE(0),(uint8_t* )"YH 5.0/7.0 inch LCD demo");
  LCD_DisplayStringLine(LINE(1),(uint8_t* )"Image resolution:800x480 px");
  LCD_DisplayStringLine(LINE(2),(uint8_t* )"Use RT1052-ELCDIF driver");

    
  /*选择字体*/
  LCD_SetFont(&Font16x32);
  LCD_SetTextColor(LCD_COLOR_RED);


	/*使用c标准库把变量转化成字符串*/
	sprintf(dispBuff,"Display value demo: testCount = %d ",testCNT);
	LCD_ClearLine(LINE(5));
	
	/*然后显示该字符串即可，其它变量也是这样处理*/
	LCD_DisplayStringLine(LINE(5),(uint8_t* )dispBuff);


  /* 画直线 */
  LCD_SetTextColor(LCD_COLOR_BLUE);

	LCD_ClearLine(LINE(6));
  LCD_DisplayStringLine(LINE(6),(uint8_t* )"Draw line:");
  
	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
  LCD_DrawUniLine(50,250,750,250);  
  LCD_DrawUniLine(50,300,750,300);
  
	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
  LCD_DrawUniLine(300,250,400,400);  
  LCD_DrawUniLine(600,250,600,400);
  
  delay(0xFFFFFFF);
  
	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
  LCD_DrawFullRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);
  
  
  /*画矩形*/
  LCD_SetTextColor(LCD_COLOR_BLUE);
	LCD_ClearLine(LINE(6));
  LCD_DisplayStringLine(LINE(6),(uint8_t* )"Draw Rect:");
	
	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
  LCD_DrawRect(200,250,200,100);
	
	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
  LCD_DrawRect(350,250,200,50);
	
	LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BLACK);
  LCD_DrawRect(200,350,50,200);
  
  delay(0xFFFFFFF);
  
  
	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
  LCD_DrawFullRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);
  

  /*填充矩形*/
  LCD_SetTextColor(LCD_COLOR_BLUE);
	LCD_ClearLine(LINE(6));
	LCD_DisplayStringLine(LINE(6),(uint8_t* )"Draw Full Rect:");

 	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
  LCD_DrawFullRect(200,250,200,100);
	
	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
  LCD_DrawFullRect(350,250,200,50);
	
	LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BLACK);
  LCD_DrawFullRect(200,350,50,200);
  
  delay(0xFFFFFFF);
  
	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
  LCD_DrawFullRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);
	
  /* 画圆 */
	LCD_SetTextColor(LCD_COLOR_BLUE);
	LCD_ClearLine(LINE(6));
	LCD_DisplayStringLine(LINE(6),(uint8_t* )"Draw circle:");
  
	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_RED);
  LCD_DrawCircle(200,350,50);
	
	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_GREEN);
  LCD_DrawCircle(350,350,75);
  
  delay(0xFFFFFFF);
  
	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
  LCD_DrawFullRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);


  /*填充圆*/
	LCD_SetTextColor(LCD_COLOR_BLUE);
	LCD_ClearLine(LINE(6));
	LCD_DisplayStringLine(LINE(6),(uint8_t* )"Draw full circle:");
  
	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
  LCD_DrawFullCircle(300,350,50);
	
	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
  LCD_DrawFullCircle(450,350,75);
  
  delay(0xFFFFFFF);
  
	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
  LCD_DrawFullRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);
	
	LCD_ClearLine(LINE(6));	
 
  delay(0xFFFFFF);
	
	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
  LCD_DrawFullRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);


}
/****************************END OF FILE**********************/
