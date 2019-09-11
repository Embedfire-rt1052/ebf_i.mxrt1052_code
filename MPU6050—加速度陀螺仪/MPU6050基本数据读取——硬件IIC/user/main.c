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

//�����Ƿ�ʹ��LCD������ʾ������Ҫ�Ļ��������ע�͵�����
//#define USE_LCD_DISPLAY    1//��Ļ���� �� Ӳ��IIC���ų�ͻ

#ifdef USE_LCD_DISPLAY
 #include "./lcd/bsp_lcd.h"
#endif

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
		static short Acel[3];
		static short Gyro[3];
		static float Temp;

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
		

		/* ��ȷ��ʱ */
		CPU_TS_TmrInit();
		I2C_Init_Hard();
		/* MPU6050��ʼ�� */
		MPU6050_Init();
		
#ifdef USE_LCD_DISPLAY
		/* ��ʼ��systick����֡�� */
		SysTick_Init();
		/* ��ʼ��LCD */
		LCD_Init(LCD_INTERRUPT_ENABLE);
#endif
		
    if (MPU6050ReadID() == 1)	//���MPU6050
    {	
      PRINTF(" MPU6050ReadID OK ");
      while(1)
      {
				MPU6050ReadAcc(Acel);
				PRINTF("���ٶȣ�%8d%8d%8d",Acel[0],Acel[1],Acel[2]);
				MPU6050ReadGyro(Gyro);
				PRINTF("    ������%8d%8d%8d",Gyro[0],Gyro[1],Gyro[2]);
				MPU6050_ReturnTemp(&Temp);
				PRINTF("    �¶�%8.2f\r\n",Temp);				
				
#ifdef USE_LCD_DISPLAY	
        {
          char cStr [ 70 ];
          sprintf ( cStr, "Acce:%8d%8d%8d",Acel[0],Acel[1],Acel[2] );	//���ٶ�ԭʼ����
          LCD_DisplayStringLine(LINE(0),(uint8_t* )cStr);			
          sprintf ( cStr, "Gyro:%8d%8d%8d",Gyro[0],Gyro[1],Gyro[2] );	//��ԭʼ����
          LCD_DisplayStringLine(LINE(1),(uint8_t* )cStr);			
          sprintf ( cStr, "Temp:  %8.2f",Temp );	//�¶�ֵ
          LCD_DisplayStringLine(LINE(2),(uint8_t* )cStr);			
        }
#endif				
				CPU_TS_Tmr_Delay_MS(100);	//����һ�����ݣ��ɸ����Լ���������߲���Ƶ�ʣ���100ms����һ��
      }
    }
    else
    {
			printf("\r\nû�м�⵽MPU6050��������\r\n");
#ifdef USE_LCD_DISPLAY			
      /*����������ɫ������ı�����ɫ*/
      LCD_SetColors(CL_BLUE,CL_BLACK);	
      LCD_DisplayStringLine(LINE(4),(uint8_t* )"No MPU6050 detected! ");			//Ұ���Դ���16*24��ʾ
      LCD_DisplayStringLine(LINE(5),(uint8_t* )"Please check the hardware connection! ");			//Ұ���Դ���16*24��ʾ
#endif
      while(1);	
    }
}

/****************************END OF FILE**********************/
