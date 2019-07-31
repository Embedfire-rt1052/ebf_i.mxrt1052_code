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

#include "fsl_elcdif.h"
#include "./delay/core_delay.h"  
#include "fsl_debug_console.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "./i2c/bsp_i2c.h"  

#include "./delay/core_delay.h"   

#include "inv_mpu.h"


/*******************************************************************************/
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{ 
	float pitch,roll,yaw; 		//欧拉角
	short aacx,aacy,aacz;		//加速度传感器原始数据
	short gyrox,gyroy,gyroz;	//陀螺仪原始数据
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
		/*  精确延时 */
		CPU_TS_TmrInit();

		I2C_Init();//软件
		/* MPU6050初始化 */		
		MPU_Init();
		mpu_DMP_Config();	
		while(1)
		{
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{ 

			MPU_Get_Temperature();	
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据

			
			PRINTF("Pitch :  %.4f  ", pitch);
			PRINTF("Roll  :  %.4f  ", roll);			
			PRINTF("Yaw  :  %.4f  \r\n", yaw);
			//CPU_TS_Tmr_Delay_US(1000*50);
		}
		}

}


///****************************END OF FILE**********************/
