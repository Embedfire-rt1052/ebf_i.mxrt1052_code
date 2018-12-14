/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   ��V2.3.1�汾�⽨�Ĺ���ģ��
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
#include "fsl_gpio.h"
#include "fsl_gpt.h"

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "./led/bsp_led.h"   
#include "./SysTick/bsp_SysTick.h"



/*******************************************************************
 * Prototypes
 *******************************************************************/
/*!
 * @brief delay a while.
 */
void delay(uint32_t count);

/*******************************************************************
 * Code
 *******************************************************************/
void delay(uint32_t count)
{
    volatile uint32_t i = 0;
    for (i = 0; i < count; ++i)
    {
        __asm("NOP"); /* delay */
    }
}

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
    /* ��ʼ���ڴ����Ԫ */
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
  
    PRINTF("SysTick��ϵͳ�δ�ʱ��\r\n");
  
    /* ��ʼ��LED���� */
    LED_GPIO_Config();  
    
    /* ����SysTick */
    SysTick_Init();
    
    while(1)
    {
      /* LED�� */
      CORE_BOARD_LED_ON;
      RGB_LED_COLOR_GREEN;
      /* ��ʱ */

      //SysTick_Delay_Ms(500);
      Delay_ms(500);
      
      /* LED�� */
      CORE_BOARD_LED_OFF;
      RGB_LED_COLOR_OFF;
      /* ��ʱ */

      //SysTick_Delay_Ms(500);
      Delay_ms(500);
      
    }     

}
/****************************END OF FILE**********************/
