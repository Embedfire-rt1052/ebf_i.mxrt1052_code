/**
******************************************************************
* @file    main.c
* @author  fire
* @version V1.0
* @date    2019-xx-xx
* @brief   CAN_�ػ�ģʽ
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
#include "fsl_flexcan.h"


#include "clock_config.h"
#include "./bsp/nvic/bsp_nvic.h"
#include "./bsp/led/bsp_led.h" 
#include "./can/bsp_can.h"



/*ȫ�ֱ���������CAN����״̬��false��û�н��յ���Ϣ��true:���յ���Ϣ���һ�δ������*/
volatile bool rxComplete = false;

/*ȫ�ֱ�����������պͷ�����Ϣ�ṹ��*/
flexcan_frame_t txFrame, rxFrame;


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
  
  PRINTF("\r\n==FlexCAN �ػ��������� -- Start.==\r\n\r\n");
  CAN_Config();
  
  while(1)
  {
    CAN_RX_Buffer_Config(0x123,RX_MESSAGE_BUFFER_NUM);//��ʼ�����ջ�����
    
    FLEXCAN_SetTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, true);//���÷�����Ϣ������
    CAN_TX_Buffer_Config(0x123);//��ʼ���������ݸ�ʽ�Լ�����
    
    PRINTF("�ӷ��ͻ����� MB%d ����ջ����� MB%d ������Ϣ\r\n", TX_MESSAGE_BUFFER_NUM, RX_MESSAGE_BUFFER_NUM);

    /*������ͻ�����������*/
    PRINTF("tx word0 = 0x%x\r\n", txFrame.dataWord0);
    PRINTF("tx word1 = 0x%x\r\n", txFrame.dataWord1);

    /*ִ�з���*/
    FLEXCAN_TransferSendBlocking(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, &txFrame);

    /* �ȴ�������� */
    while (!rxComplete)
    {
    }

    PRINTF("\r\n���ջ�����MB%d ������\r\n", RX_MESSAGE_BUFFER_NUM);
    
    /*������յ�������*/
    PRINTF("rx word0 = 0x%x\r\n", rxFrame.dataWord0);
    PRINTF("rx word1 = 0x%x\r\n", rxFrame.dataWord1);

    /* ֹͣ FlexCAN ���պͷ���. */
    FLEXCAN_DisableMbInterrupts(EXAMPLE_CAN, 1 << RX_MESSAGE_BUFFER_NUM);

    PRINTF("\r\n==FlexCAN �ػ�ģʽ���Խ���==\r\n");
    while (1)
    {
      ;
    }
  }
  
}



/****************************END OF FILE**********************/

