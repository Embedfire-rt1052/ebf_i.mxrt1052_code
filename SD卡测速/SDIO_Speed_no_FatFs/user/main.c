/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   ��дSD����������
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

#include "./bsp/sd/bsp_sd.h"  

#include "./bsp/nvic/bsp_nvic.h"
#include "./bsp/led/bsp_led.h"   
#include "./bsp/gpt/bsp_gpt.h"    
    
    
/*Card�ṹ������*/
sd_card_t g_sd;
volatile unsigned int i = 0; //���ڿ���RGB�Ƶķ�ת

/**
 * @brief ��ʱһ��ʱ��
 */
void delay(uint32_t count);

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
    
    /*��ʼ��������GPT��ʱ��*/
    GPT_Config();
    PRINTF("��ʱ������Ƶ�� %d \r\n",CLOCK_GetFreq(kCLOCK_IpgClk));
    
    /*��ʼ��SD���ӿڵ�GPIO����*/
    USDHC1_gpio_init();
    USDHC_Host_Init(&g_sd);
    /*SD������д���Ժ������ڲ�������SD���ĳ�ʼ��*/
    SD_Card_Init(&g_sd);
    
    /*ȷ����ʼֵΪ0*/
    PRINTF("ȷ����ʼֵΪ0��%d \r\n",GPT2->CNT);
    while(1)
    {
      HD_write_text(&g_sd);//��ʼд��
      
      PRINTF("�룺 %d \r\n",i);
      PRINTF("Timer Counter %d \r\n",GPT2->CNT);
      
      /*���㶨ʱ��*/
//      GPT2->CNT = 0;
      /*�������ֵ*/
      i = 0;
      
      /*ȷ����ʼֵΪ0*/
      PRINTF("ȷ����ʼֵΪ0��%d \r\n",GPT2->CNT);
      HD_read_text(&g_sd);
        
      PRINTF("�룺 %d \r\n",i);
      PRINTF("Timer Counter %d \r\n",GPT2->CNT);
      
      
      
      
      while(1);
      

      //SD_Card_Test(&g_sd);
      delay(0x3ffffff);
    }			
}
/****************************END OF FILE**********************/


/************************��һ����*************************/
