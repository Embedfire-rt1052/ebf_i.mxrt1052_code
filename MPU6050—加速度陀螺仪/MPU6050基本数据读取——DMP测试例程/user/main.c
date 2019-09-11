/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   MPU6050-���ٶ�������
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

#include "fsl_elcdif.h"
#include "./delay/core_delay.h"  
#include "fsl_debug_console.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "./i2c/bsp_i2c.h"  

#include "./delay/core_delay.h"   

#include "inv_mpu.h"
#include "./mpu6050_DMP/mpu6050.h"  


/*******************************************************************************/
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{ 
		float Pitch,Roll,Yaw; 
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

		PRINTF("*****MPU6050-DMP��������*****\r\n");
		/*  ��ȷ��ʱ */
		CPU_TS_TmrInit();
		/* ���IIC��ʼ�� */
		I2C_Init();
		/* MPU6050��ʼ�� */		
		MPU_Init();
		/* mpu DMP������� */
		mpu_DMP_Config();	
		while(1)
		{
			if(mpu_dmp_get_data(&Pitch,&Roll,&Yaw)==0)
			{ 
				PRINTF("Pitch :  %.4f  ", Pitch);
				PRINTF("Roll  :  %.4f  ", Roll);			
				PRINTF("Yaw  :  %.4f  \r\n", Yaw);
				//CPU_TS_Tmr_Delay_US(1000*50);
			}
		}

}


///****************************END OF FILE**********************/
