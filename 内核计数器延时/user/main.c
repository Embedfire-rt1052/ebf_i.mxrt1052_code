/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   ʹ���ں�ʱ�����������ʱ
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
#include "./delay/core_delay.h"   



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

//ʹ�ò���ȷ����ʱ
//#define LED_DELAY   delay(LED_DELAY_COUNT)

//ʹ�þ�ȷ����ʱ����ʱ���ܴ����Ż�Ӱ��
#define LED_DELAY   CPU_TS_Tmr_Delay_S(1)


/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
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
  
    PRINTF("ʹ���ں�ʱ�����ȷ��ʱ\r\n");
  
    /* ��ʼ��LED���� */
    LED_GPIO_Config();  
    
    while(1)
    {         
      /* LED�� */
      CORE_BOARD_LED_ON;
      /* ��ʱ */
      LED_DELAY;
      
      /* ����������� */
      RGB_RED_LED_ON;
      LED_DELAY;
      
      RGB_RED_LED_OFF;
      LED_DELAY;
      
      /* ���������̵� */
      RGB_GREEN_LED_ON;
      LED_DELAY;
      
      RGB_GREEN_LED_OFF;
      LED_DELAY;
      
      /* ������������ */
      RGB_BLUE_LED_ON;
      LED_DELAY;
      
      RGB_BLUE_LED_OFF;
      LED_DELAY;    

      /* ���������ɫ */
      RGB_LED_COLOR_RED;
      LED_DELAY;    
      
      /* ���������ɫ */
      RGB_LED_COLOR_GREEN;
      LED_DELAY;    
      
      /* ���������ɫ */
      RGB_LED_COLOR_BLUE;
      LED_DELAY;    
      
      /* ���������ɫ */
      RGB_LED_COLOR_YELLOW;
      LED_DELAY;    
      
      /* ���������ɫ */
      RGB_LED_COLOR_PURPLE;
      LED_DELAY;    
      
      /* ���������ɫ */
      RGB_LED_COLOR_CYAN;
      LED_DELAY;    
      
      /* ���������ɫ */
      RGB_LED_COLOR_WHITE;
      LED_DELAY;    
      
      /* ���������ɫ��ȫ�رգ� */
      RGB_LED_COLOR_OFF;
      LED_DELAY;    
    }     

}
/****************************END OF FILE**********************/
