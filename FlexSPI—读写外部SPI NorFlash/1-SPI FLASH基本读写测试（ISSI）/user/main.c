/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   FlexSPI����д�ⲿSPI NorFlash
  ******************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************
  */
#include "fsl_debug_console.h"

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "./norflash/bsp_norflash.h"
#include "./led/bsp_led.h"   

/*******************************************************************
 * Prototypes
 *******************************************************************/

extern int NorFlash_AHBCommand_Test(void);
extern int NorFlash_IPCommand_Test(void);
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

    LED_GPIO_Config();
    /* ��ʼ��FlexSPI���� */
    FlexSPI_NorFlash_Init();

    /* ʹ��IP�������FLASH�Ĳ��� */
    NorFlash_IPCommand_Test();

    /* ʹ��AHB�������FLASH�Ĳ��� */
    NorFlash_AHBCommand_Test();
    while(1)
    {
    }     

}
/****************************END OF FILE**********************/
