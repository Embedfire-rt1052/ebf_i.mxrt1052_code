/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   Һ����ʾӢ��
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

#include "./delay/core_delay.h"   

#include "./led/bsp_led.h" 
#include "./lcd/bsp_lcd.h" 
#include "./touch/gt9xx.h"
#include "./touch/bsp_i2c_touch.h"
#include "./touch/palette.h"
#include "./eeprom/bsp_i2c_eeprom.h"
#include "./norflash/bsp_norflash.h"
#include "./logictest/logictest.h" 
extern bool SEMC_SDRAMReadWriteTest(void);
extern void SEMC_SDRAMReadWriteSpeedTest(void);
extern bool SDRAM_FullChipTest(void);
extern uint8_t EEPROM_Test(void);
extern int NorFlash_AHBCommand_Test(void);
extern int NorFlash_IPCommand_Test(void);
extern int sdcard_test(uint8_t cardnum);
extern int USBtest(void);
extern int lwip_ping_test(void);

/*******************************************************************
 * Prototypes
 *******************************************************************/
#define SDRAM_TEST_FAIL  0
#define SDRAM_TEST_OK    1

#define ENET_TEST_FAIL   0
#define ENET_TEST_OK     1

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
		uint16_t display_y_axis=0;
		uint8_t result1,result2 =0;	
		static uint8_t runonce=1;
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

  	PRINTF("*****���ݴ���������������*****\r\n");

    if(SEMC_SDRAMReadWriteTest() && SDRAM_FullChipTest())
    {
      result1 = SDRAM_TEST_OK;
    }
    else
    {
      result1 = SDRAM_TEST_FAIL;
    }
		PRINTF("*****������̫��*****\r\n");	
		/* ������̫������ */
    if(lwip_ping_test())
    {
      result2 = ENET_TEST_FAIL;
      PRINTF("*****��̫������ʧ��*****\r\n");	

    }
    else
    {      
      result2 = ENET_TEST_OK;
      PRINTF("*****��̫�����Գɹ�*****\r\n");	

    }		
    /* ��ʼ��LED */
    LED_GPIO_Config();
    
    while(1);
    
//    /* ��ʼ��LCD */
//    LCD_Init(LCD_INTERRUPT_DISABLE);
//    
//    GTP_Init_Panel();   

//    /*���û��庯��*/
//    Palette_Init();
//		
//		LCD_BackLight_ON();		
//		LCD_SetColors(CL_RED,CL_WHITE);
//		LCD_DispString( 320,display_y_axis,(uint8_t*)"EBF1052 test program ");
//		LCD_SetColors(CL_BLUE,CL_WHITE);
//		display_y_axis +=30;
//		LCD_DispString( 320,display_y_axis,(uint8_t*)"ETHNET test... ");
//		display_y_axis +=30;
//		if(result2==0)
//		{
//			LCD_SetColors(CL_BLUE,CL_WHITE);
//			LCD_DispString( 320,display_y_axis,(uint8_t*)"ETHNET test success ");				
//		}
//		else
//		{
//			LCD_SetColors(CL_RED,CL_WHITE);
//			LCD_DispString( 320,display_y_axis,(uint8_t*)"ETHNET test fail ");			
//		}
//		display_y_axis +=30;
//		if(result1)
//		{
//			LCD_SetColors(CL_BLUE,CL_WHITE);
//			LCD_DispString( 320,display_y_axis,(uint8_t*)"SDRAM test success ");			
//		}
//		else
//		{
//			LCD_SetColors(CL_RED,CL_WHITE);
//			LCD_DispString( 320,display_y_axis,(uint8_t*)"SDRAM test fail ");			
//		}	
//    /* ��ʼ��EEPROM */
//    I2C_EEPROM_Init();
//    display_y_axis +=30;
//    /* ����EEPROM��д���� */
//    if(EEPROM_Test())
//		{
//			LCD_SetColors(CL_RED,CL_WHITE);
//			LCD_DispString( 320,display_y_axis,(uint8_t*)"EEPROM test fail ");
//		}
//		else
//		{
//			LCD_SetColors(CL_BLUE,CL_WHITE);
//			LCD_DispString( 320,display_y_axis,(uint8_t*)"EEPROM test success ");			
//		}	
//		display_y_axis +=30;
//		FlexSPI_NorFlash_Init();
//    /* ����flash��д���� */
//    if(NorFlash_IPCommand_Test())
//		{
//			LCD_SetColors(CL_RED,CL_WHITE);
//			LCD_DispString( 320,display_y_axis,(uint8_t*)"Flash test fail ");
//		}
//		else
//		{
//			LCD_SetColors(CL_BLUE,CL_WHITE);
//			LCD_DispString( 320,display_y_axis,(uint8_t*)"Flash test success ");			
//		}	
//		display_y_axis +=30;
//    /* ����IO���߼����� */
//    if(sdcard_test(1))
//		{
//			LCD_SetColors(CL_RED,CL_WHITE);
//			LCD_DispString( 320,display_y_axis,(uint8_t*)"SD card 1 test fail ");
//		}
//		else
//		{
//			LCD_SetColors(CL_BLUE,CL_WHITE);
//			LCD_DispString( 320,display_y_axis,(uint8_t*)"SD card 1 test success ");			
//		}
//		display_y_axis +=30;
//    /* ����IO���߼����� */
//    if(sdcard_test(2))
//		{
//			LCD_SetColors(CL_RED,CL_WHITE);
//			LCD_DispString( 320,display_y_axis,(uint8_t*)"SD card 2 test fail ");
//		}
//		else
//		{
//			LCD_SetColors(CL_BLUE,CL_WHITE);
//			LCD_DispString( 320,display_y_axis,(uint8_t*)"SD card 2 test success ");			
//		}
//		display_y_axis +=30;
//    /* ����USB���� */
//    if(USBtest())
//		{
//			LCD_SetColors(CL_RED,CL_WHITE);
//			LCD_DispString( 320,display_y_axis,(uint8_t*)"USB test fail ");
//		}
//		else
//		{
//			LCD_SetColors(CL_BLUE,CL_WHITE);
//			LCD_DispString( 320,display_y_axis,(uint8_t*)"USB test success ");			
//		}
//		display_y_axis +=30;
//		/* ��ʼ���߼��������� */
//    LogicTest_GPIO_Config();
//    /* ����IO���߼����� */
//    if(logicTestInOut())
//		{
//			LCD_SetColors(CL_RED,CL_WHITE);
//			LCD_DispString( 320,display_y_axis,(uint8_t*)"GPIO logic test fail ");
//		}
//		else
//		{
//			LCD_SetColors(CL_BLUE,CL_WHITE);
//			LCD_DispString( 320,display_y_axis,(uint8_t*)"GPIO logic test success ");			
//		}
//		display_y_axis +=30;
//		LCD_SetColors(CL_BLUE,CL_WHITE);
//		LCD_DispString( 320,display_y_axis,(uint8_t*)"touch the pannel...");		
//    while(1)
//    {	
//      /* ���ִ����ź�ʱ���д��� */
//			if(g_TouchPadInputSignal)
//			{
//				GTP_TouchProcess();    
//				g_TouchPadInputSignal = false;
//				if(runonce)
//				{
//					runonce=0;
//					display_y_axis +=30;
//					LCD_SetColors(CL_BLUE,CL_WHITE);
//					LCD_DispString( 320,display_y_axis+30,(uint8_t*)"Test OK");					
//				}

//			}
//    }
}

/****************************END OF FILE**********************/
