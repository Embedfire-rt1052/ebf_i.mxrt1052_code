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


#include "./bsp/sd/bsp_sd.h"  
#include "ff.h"
#include "diskio.h"
#include "fsl_sd_disk.h"
#include "bsp_sd_fatfs_test.h"



#define BUFFER_SIZE (100U)

/*�ļ�ϵͳ�����ṹ��*/
FATFS g_fileSystem; /* File system object */


/* ���ڴ洢��ǰ������ɫ�����屳����ɫ�ı���*/
extern volatile  pixel_t CurrentTextColor;//Ĭ��Ϊ��ɫ
extern volatile  pixel_t CurrentBackColor;//Ĭ��Ϊ��ɫ


/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
  volatile FIL file_object;   //�����ļ���������
  volatile DIR dir_object;    //Ŀ¼����ṹ��
  volatile FILINFO file_info; //�ļ���Ϣ�����ṹ��
  
  
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
  
//  /*�����ļ�ϵͳ*/
//  f_mount_test(&g_fileSystem);
//  /*��ȡ·���µ��ļ����ļ���*/
//  PRINTF("/Font Ŀ¼�µ��ֿ��ļ�");
//  f_readdir_test("/Font",&dir_object,&file_info);
  
  
  /* ��ʼ��LCD */
  LCD_Init(LCD_INTERRUPT_ENABLE);
  
  RGB_LED_COLOR_BLUE;
  
  CurrentTextColor = CL_RED;  
  /*��ʾ��Ӣ�Ĳ���*/
  LCD_DisplayStringLine_EN_CH(1,(uint8_t* )"AABBCCDD���СС");

  while(1)
  {
    
  }
}

/****************************END OF FILE**********************/
