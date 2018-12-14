/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   SEMC����չ�ⲿSDRAM��Դ���ʼ����
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

#include "./led/bsp_led.h"
#include "./sdram/bsp_sdram.h" 


extern bool SEMC_SDRAMReadWriteTest(void);
extern void SEMC_SDRAMReadWriteSpeedTest(void);
extern bool SDRAM_FullChipTest(void);

/*******************************************************************
 * Prototypes
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
    
    /* ��ʼ��SDRAM */
    if (SDRAM_Init() != kStatus_Success)
    {
        RGB_LED_COLOR_RED;
        PRINTF("\r\n SEMC SDRAM Init Failed\r\n");
    }
  
    PRINTF("*****������ʹ����main������ʹ��SDRAM_Init��ʼ��SDRAM*****\r\n");
    PRINTF("*****���ֳ�ʼ����ʽ ���ʺ� ��ϵͳ���������ҪSDRAM��Ӧ��*****\r\n");
    PRINTF("*****��������Ҫ��Ϊ����ʾ���ʹ��Դ���ʼ��SDRAM*****\r\n");

    PRINTF("�����SDRAM���ж�д����\r\n");

    if(SEMC_SDRAMReadWriteTest() && SDRAM_FullChipTest())
    {
      RGB_LED_COLOR_GREEN;
      CORE_BOARD_LED_ON;
      PRINTF("\r\n SDRAM���Գɹ���\r\n");
    }
    else
    {
      RGB_LED_COLOR_RED;
      CORE_BOARD_LED_OFF;
      PRINTF("\r\n SDRAM����ʧ�ܣ�\r\n");
    }
    
    PRINTF("\r\n SDRAM��д���Խ���\r\n");   
    
    while(1)
    {
      
    }    
}
/****************************END OF FILE**********************/
