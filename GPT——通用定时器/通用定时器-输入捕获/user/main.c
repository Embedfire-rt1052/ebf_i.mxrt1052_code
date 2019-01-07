/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   ͨ�ö�ʱ������ʱ
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
#include "./gpt/bsp_gpt.h"


extern volatile GPT_ICUserValueTypeDef GPT_ICUserValueStructure;

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
  uint64_t timer = 0;
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

  PRINTF("GPT��ʱ�����벶��ʵ��\r\n");

  /* ��ʼ��LED���� */
  LED_GPIO_Config();
  
  /*��ʼ��������GPT��ʱ��*/
  GPT_Config();
  while(1)
  {  
     if(GPT_ICUserValueStructure.Capture_FinishFlag)
     {

       /*�õ�����ֵ��timer Ϊ64λ���ݣ�32λ�ܿ��ܻ����*/
       timer = GPT_ICUserValueStructure.Capture_Period * 0xffffffff; 
       timer += GPT_ICUserValueStructure.Capture_CcrValue_2;         
       timer -= GPT_ICUserValueStructure.Capture_CcrValue_1;
       
       /*������ֵת��Ϊʱ�䣬��λ��ms��*/
       timer = timer / ((EXAMPLE_GPT_CLK_FREQ)/1000);
       
       PRINTF("the result is: %lld ms \r\n",timer);
       GPT_ICUserValueStructure.Capture_FinishFlag = 0;
     }
  }     

}
/****************************END OF FILE**********************/
