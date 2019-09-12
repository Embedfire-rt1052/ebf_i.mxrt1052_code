/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   �����жϽ��ղ���
  ******************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *CPU_MIMXRT1052DVL6B,PRINTF_FLOAT_ENABLE=1, SCANF_FLOAT_ENABLE=1, PRINTF_ADVANCED_ENABLE=1, SCANF_ADVANCED_ENABLE=1
  ******************************************************************
  */


#include "fsl_debug_console.h"

#include "board.h"
#include "pin_mux.h"

#include "clock_config.h"
#include "./bsp/nvic/bsp_nvic.h"
#include "./bsp/led/bsp_led.h" 
#include "./bsp/adc/bsp_adc.h"



volatile bool ADC_ConversionDoneFlag; //��־λ
volatile uint32_t ADC_ConvertedValue;


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
  adc_channel_config_t adcChannelConfigStruct;//����ADC ͨ�����ýṹ��
  float ADC_ConvertedValueLocal = 0;  //����ת���õ��ĵ�ѹֵ
  
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
  
  adcChannelConfigStruct.channelNumber = DEMO_ADC_USER_CHANNEL;
  adcChannelConfigStruct.enableInterruptOnConversionCompleted = true;
  while(1)
  {
    ADC_ConversionDoneFlag = false;
    /*δ��������ת��ʱ��Ӳ������ת��ʱ��ÿ����һ�θú�������һ��ת��*/
    ADC_SetChannelConfig(ADCx, DEMO_ADC_CHANNEL_GROUP, &adcChannelConfigStruct);
    while (ADC_ConversionDoneFlag == false)
    {
      
    }
    
    /*���ԭʼת�����*/
    PRINTF("The Conversion Value: %d\r\n", ADC_ConvertedValue);
    
    /*�����ת��Ϊ��ѹֵ��ͨ���������*/
    ADC_ConvertedValueLocal =((float)ADC_ConvertedValue)/4095.0f*3.3f; 
    PRINTF("The current AD value = %f V \r\n",ADC_ConvertedValueLocal);
    PRINTF("\r\n");
    
    /*��ֹת���ٶȹ��쵼�´�����������쳣*/
    delay(LED_DELAY_COUNT/3);
    
  }

}



/****************************END OF FILE**********************/

