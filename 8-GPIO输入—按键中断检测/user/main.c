/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   GPIO���롪�����жϼ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "fsl_gpt.h"

#include "pin_mux.h"
#include "clock_config.h"

#include "./led/bsp_led.h"
#include "./key/bsp_key_it.h"


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
  
    PRINTF("GPIO���롪�����ж�ʵ��\r\n");
  
    /* ��ʼ��LED���� */
    LED_GPIO_Config();
    
    /* ��ʼ��KEY���� */
    Key_IT_GPIO_Config();
    
    /* �ȴ������ж����٣����������¾�ִ�а����жϷ�������
       �����жϷ�������bsp_key_it.cʵ�֡�
       �жϷ��������ȫ�ֱ���g_KeyDown����Ϊtrue��
    */ 

    while(1)
    {   
      /* WAUP�����ı�־ */
      /* ��g_KeyDownΪtrue�������������� */
      if(g_KeyDown[CORE_BOARD_WAUP_KEY_ID])
      {
          /* ��΢��ʱ */
          delay(100);
          /* �ȴ����������ͷ� ���ߵ�ƽ��*/
          if(1 == GPIO_PinRead(CORE_BOARD_WAUP_KEY_GPIO, CORE_BOARD_WAUP_KEY_GPIO_PIN))
          {
              /* ��תLED�ƣ����������Ϣ */
              CORE_BOARD_LED_TOGGLE;
              PRINTF("��⵽ %s ��������\r\n", CORE_BOARD_WAUP_KEY_NAME);
          }
          /* �������ñ�־λ */
          g_KeyDown[CORE_BOARD_WAUP_KEY_ID] = false; 
      }
      
      /* MODE�����ı�־ */
      /* ��g_KeyDownΪtrue�������������� */
      if(g_KeyDown[CORE_BOARD_MODE_KEY_ID])
      {
          /* ��΢��ʱ */
          delay(100);
          /* �ȴ����������ͷ� ���ߵ�ƽ��*/
          if(1 == GPIO_PinRead(CORE_BOARD_MODE_KEY_GPIO, CORE_BOARD_MODE_KEY_GPIO_PIN))
          {
              /* ��תLED�ƣ����������Ϣ */
              CORE_BOARD_LED_TOGGLE;
              PRINTF("��⵽ %s ��������\r\n", CORE_BOARD_MODE_KEY_NAME);
          }
          /* �������ñ�־λ */
          g_KeyDown[CORE_BOARD_MODE_KEY_ID] = false; 
      }
    }     

}
/*********************************************END OF FILE**********************/
