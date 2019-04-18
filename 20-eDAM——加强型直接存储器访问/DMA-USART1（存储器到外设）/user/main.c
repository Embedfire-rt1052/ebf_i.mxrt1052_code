/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   ʹ��DMAʵ�ִ��ڵķ��������
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
#include "./bsp/dma_uart/bsp_dma_uart.h"

   
/*����TX��RX���ݴ洢��*/
AT_NONCACHEABLE_SECTION_INIT(uint8_t g_txBuffer[ECHO_BUFFER_LENGTH]) = {0};
AT_NONCACHEABLE_SECTION_INIT(uint8_t g_rxBuffer[ECHO_BUFFER_LENGTH]) = {0};


/*����UART����״̬��־*/
volatile bool rxBufferEmpty = true;  //���ջ�������
volatile bool txBufferFull = false;  //���ͻ�������
volatile bool txOnGoing = false;     //����ִ�з���
volatile bool rxOnGoing = false;     //����ִ�н���


/*�������*/
lpuart_edma_handle_t g_lpuartEdmaHandle;  //����DMA������
edma_handle_t g_lpuartTxEdmaHandle;       //����DMA���;��
edma_handle_t g_lpuartRxEdmaHandle;       //����DMA���վ��


/*����ϵͳ������ʾ��Ϣ*/
AT_NONCACHEABLE_SECTION_INIT(uint8_t g_tipString[]) =
    "LPUART EDMA example\r\nSend back received data\r\nEcho every 8 characters\r\n";

/*****************************************************************
 * ������
 *****************************************************************/
int main(void)
{
  
  /*���崫��ṹ��*/
  lpuart_transfer_t xfer;          //������ʾ��Ϣ����ṹ��
  lpuart_transfer_t sendXfer;      //���巢�ʹ���ṹ��
  lpuart_transfer_t receiveXfer;   //������մ���ṹ��
  
  
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
  
  
  UART_Init();     //��ʼ������
  UART_DMA_Init(); //��ʼ������DMA����ʹ�õ�DMA

  /* ������ʾ��Ϣ */
  xfer.data = g_tipString;
  xfer.dataSize = sizeof(g_tipString) - 1;
  txOnGoing = true;
  LPUART_SendEDMA(DEMO_LPUART, &g_lpuartEdmaHandle, &xfer);
  /* �ȴ�������� */
  while (txOnGoing)
  {
    
  }
  
  /* ����UART���ͺͽ��մ���ṹ�� */
  sendXfer.data = g_txBuffer;
  sendXfer.dataSize = ECHO_BUFFER_LENGTH;
  receiveXfer.data = g_rxBuffer;
  receiveXfer.dataSize = ECHO_BUFFER_LENGTH;
  
  /*��ѯ��⴮�ڵ�ǰ״̬�����յ����ݺ��������ͳ�ȥ*/
  while (1)
  {
    /* ������տ��в��ҽ��ջ�����Ϊ�գ���ʾ��ǰ���ڿ��д�ʱ�ȴ���������*/
    if ((!rxOnGoing) && rxBufferEmpty)
    {
      rxOnGoing = true;
      LPUART_ReceiveEDMA(DEMO_LPUART, &g_lpuartEdmaHandle, &receiveXfer);
    }
    
    /* ������Ϳ��в��ҷ��ͻ�����������ʱӦ����ʼ��������*/
    if ((!txOnGoing) && txBufferFull)
    {
      txOnGoing = true;
      LPUART_SendEDMA(DEMO_LPUART, &g_lpuartEdmaHandle, &sendXfer);
    }
    
    /* ������ͻ������ղ��ҽ��ջ�����������ʱӦ�����ջ����������ݿ��������ͻ����� */
    if ((!rxBufferEmpty) && (!txBufferFull))
    {
      memcpy(g_txBuffer, g_rxBuffer, ECHO_BUFFER_LENGTH);
      rxBufferEmpty = true;
      txBufferFull = true;
    }
  }
}
