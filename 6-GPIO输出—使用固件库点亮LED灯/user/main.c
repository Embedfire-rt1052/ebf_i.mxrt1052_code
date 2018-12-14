/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   GPIO�����ʹ�ù̼������LED��
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



/*******************************************************************
 * Prototypes
 *******************************************************************/
/**
 * @brief ��ʱһ��ʱ��
 */
void delay(uint32_t count);

/*******************************************************************
 * Code
 *******************************************************************/
/**
 * @note �������ڲ�ͬ���Ż�ģʽ����ʱʱ�䲻ͬ��
 *       ��flexspi_nor_debug��flexspi_nor_release�汾�ĳ����У�
 *       flexspi_nor_release�汾����ʱҪ�̵ö�  
 */ 
void delay(uint32_t count)
{
    volatile uint32_t i = 0;
    for (i = 0; i < count; ++i)
    {
        __asm("NOP"); /* ����nop��ָ�� */
    }
}

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
  
    PRINTF("GPIO���-ʹ�ù̼������LED\r\n");
  
    /* ��ʼ��LED���� */
    LED_GPIO_Config();  
    
    while(1)
    {         
      /* LED�� */
      CORE_BOARD_LED_ON;
      /* ��ʱ */
      delay(LED_DELAY_COUNT);
      
      /* ����������� */
      RGB_RED_LED_ON;
      delay(LED_DELAY_COUNT);
      
      RGB_RED_LED_OFF;
      delay(LED_DELAY_COUNT);
      
      /* ���������̵� */
      RGB_GREEN_LED_ON;
      delay(LED_DELAY_COUNT);
      
      RGB_GREEN_LED_OFF;
      delay(LED_DELAY_COUNT);
      
      /* ������������ */
      RGB_BLUE_LED_ON;
      delay(LED_DELAY_COUNT);
      
      RGB_BLUE_LED_OFF;
      delay(LED_DELAY_COUNT);   

      /* ���������ɫ */
      RGB_LED_COLOR_RED;
      delay(LED_DELAY_COUNT);   
      
      /* ���������ɫ */
      RGB_LED_COLOR_GREEN;
      delay(LED_DELAY_COUNT);   
      
      /* ���������ɫ */
      RGB_LED_COLOR_BLUE;
      delay(LED_DELAY_COUNT);   
      
      /* ���������ɫ */
      RGB_LED_COLOR_YELLOW;
      delay(LED_DELAY_COUNT);   
      
      /* ���������ɫ */
      RGB_LED_COLOR_PURPLE;
      delay(LED_DELAY_COUNT);   
      
      /* ���������ɫ */
      RGB_LED_COLOR_CYAN;
      delay(LED_DELAY_COUNT);   
      
      /* ���������ɫ */
      RGB_LED_COLOR_WHITE;
      delay(LED_DELAY_COUNT);   
      
      /* ���������ɫ��ȫ�رգ� */
      RGB_LED_COLOR_OFF;
      delay(LED_DELAY_COUNT);   
    }     

}
/****************************END OF FILE**********************/
