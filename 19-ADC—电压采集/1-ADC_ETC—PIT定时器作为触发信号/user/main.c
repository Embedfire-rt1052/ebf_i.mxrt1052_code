/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   ADCӲ�������ɼ�����
  ******************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  ******************************************************************
  */


#include "fsl_debug_console.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
    
#include "./bsp/nvic/bsp_nvic.h"
#include "./bsp/led/bsp_led.h" 
#include "./bsp/adc/bsp_adc.h"
#include "./bsp/pit/bsp_pit.h"
#include "./bsp/xbara/bsp_xbara.h"




volatile uint32_t g_AdcConversionValue0;
volatile uint32_t g_AdcConversionValue1;
volatile  bool b_Value0_Conversion_complete_flag;
volatile  bool b_Value1_Conversion_complete_flag;

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
  /*�����ж����ȼ�����*/
  Set_NVIC_PriorityGroup(Group_4); 
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

  /* ��ʼ��LED���� */
  LED_GPIO_Config(); 
  
  /*��ʼ�� ADC */
  ADC_Config();
  
  /*��ʼ��xborta*/
  XBARA_Configuration();
  
  /*��ʼ��������PIT��ʱ��*/
  PIT_Configuration();
  PIT_start_timer();
  
  /*��ʼ��ת����ɱ�־*/
  b_Value0_Conversion_complete_flag = false;
  b_Value1_Conversion_complete_flag = false;
  
  while(1)
  {
    /*����Ƿ�ת�����*/
    while(!(b_Value0_Conversion_complete_flag&&b_Value1_Conversion_complete_flag))
    {

    }
   /*��ӡת�����*/
   PRINTF("CH0:%d  CH15:%d\r\n", g_AdcConversionValue0, g_AdcConversionValue1);
   b_Value0_Conversion_complete_flag = false;
   b_Value1_Conversion_complete_flag = false;
  }

}



/****************************END OF FILE**********************/

