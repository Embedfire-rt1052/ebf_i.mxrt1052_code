/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   485�շ�ʵ��
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
#include "./bsp/nvic/bsp_nvic.h"
#include "./bsp/led/bsp_led.h" 
#include "./bsp/485/bsp_485.h"



extern volatile uint8_t ucTemp;//���ڱ����յ����ַ�
extern volatile bool resived;//���ڱ������״̬

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
   
  /*��ʼ��uart5*/
  UART_Config();
  
  /*��ʼ��485оƬ�����뷢�Ϳ�������*/
  _485_Control_GPIO_init();  
  
  PRINTF("����һ��485ͨ��ʵ��\r\n");
  PRINTF("�����Լÿ1�뷢��һ���ַ���a�������յ���Ϣ����������յ����ַ�\r\n");
  while(1)
  {
    /* ����� */
    RGB_RED_LED_ON;
    RGB_GREEN_LED_OFF
    /*ִ��485����*/
    _485_SendByte(DEBUG_UARTx,'a');  
    PRINTF("send finish \r\n");
     
    delay(LED_DELAY_COUNT);
    
    if(resived)
    {
      PRINTF("resive is:%c\r\n",ucTemp);
      resived = false;
    }
    /*���̵�*/
    RGB_RED_LED_OFF
    RGB_GREEN_LED_ON;
    delay(LED_DELAY_COUNT);    
  }
}



/****************************END OF FILE**********************/
      
