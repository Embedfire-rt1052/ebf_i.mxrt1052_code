/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   MPU6050-加速度陀螺仪
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
#include "./systick/bsp_systick.h"
#include "./lpi2c/bsp_lpi2c.h"
#include "./mpu6050/mpu6050.h"

#include "./led/bsp_led.h" 
#include "./lcd/bsp_lcd.h" 
#include "./delay/core_delay.h"   
/*******************************************************************
 * Prototypes
 *******************************************************************/

//设置是否使用LCD进行显示，不需要的话把这个宏注释掉即可
//#define USE_LCD_DISPLAY    1//屏幕引脚 与 硬件IIC引脚冲突

#ifdef USE_LCD_DISPLAY
 #include "./lcd/bsp_lcd.h"
#endif

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
		static short Acel[3];
		static short Gyro[3];
		static float Temp;

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
		

		/* 精确延时 */
		CPU_TS_TmrInit();
		I2C_Init_Hard();
		/* MPU6050初始化 */
		MPU6050_Init();
		
#ifdef USE_LCD_DISPLAY
		/* 初始化systick计算帧率 */
		SysTick_Init();
		/* 初始化LCD */
		LCD_Init(LCD_INTERRUPT_ENABLE);
#endif
		
    if (MPU6050ReadID() == 1)	//检测MPU6050
    {	
      PRINTF(" MPU6050ReadID OK ");
      while(1)
      {
				MPU6050ReadAcc(Acel);
				PRINTF("加速度：%8d%8d%8d",Acel[0],Acel[1],Acel[2]);
				MPU6050ReadGyro(Gyro);
				PRINTF("    陀螺仪%8d%8d%8d",Gyro[0],Gyro[1],Gyro[2]);
				MPU6050_ReturnTemp(&Temp);
				PRINTF("    温度%8.2f\r\n",Temp);				
				
#ifdef USE_LCD_DISPLAY	
        {
          char cStr [ 70 ];
          sprintf ( cStr, "Acce:%8d%8d%8d",Acel[0],Acel[1],Acel[2] );	//加速度原始数据
          LCD_DisplayStringLine(LINE(0),(uint8_t* )cStr);			
          sprintf ( cStr, "Gyro:%8d%8d%8d",Gyro[0],Gyro[1],Gyro[2] );	//角原始数据
          LCD_DisplayStringLine(LINE(1),(uint8_t* )cStr);			
          sprintf ( cStr, "Temp:  %8.2f",Temp );	//温度值
          LCD_DisplayStringLine(LINE(2),(uint8_t* )cStr);			
        }
#endif				
				CPU_TS_Tmr_Delay_MS(100);	//更新一次数据，可根据自己的需求，提高采样频率，如100ms采样一次
      }
    }
    else
    {
			printf("\r\n没有检测到MPU6050传感器！\r\n");
#ifdef USE_LCD_DISPLAY			
      /*设置字体颜色及字体的背景颜色*/
      LCD_SetColors(CL_BLUE,CL_BLACK);	
      LCD_DisplayStringLine(LINE(4),(uint8_t* )"No MPU6050 detected! ");			//野火自带的16*24显示
      LCD_DisplayStringLine(LINE(5),(uint8_t* )"Please check the hardware connection! ");			//野火自带的16*24显示
#endif
      while(1);	
    }
}

/****************************END OF FILE**********************/
