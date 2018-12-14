/**
  ******************************************************************
  * @file    bsp_usart.c
  * @author  fire
  * @version V1.1
  * @date    2018-xx-xx
  * @brief   uartӦ�ú����ӿ�
  ******************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************
  */

#include "board.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"  
#include "fsl_lpuart.h"
  
#include "pad_config.h"  
#include "fsl_debug_console.h"

#include "./uart/bsp_usart.h"

/**
* @brief  ��ʼ��uart���ò���
* @param  ��
* @retval ��
*/
void USART_ModeConfig(void)
{
  /*���崮�����ò����ṹ����������ڱ��洮�ڵ�������Ϣ*/
  lpuart_config_t config;
  
  /*���ù̼��⺯���õ�Ĭ�ϵĴ������ò�������Ĭ�ϵ����ò����������޸�*/
  LPUART_GetDefaultConfig(&config);
  config.baudRate_Bps = DEBUG_USART_BAUDRATE;  //������
  config.enableRx = DEBUG_USART_ENABLE_RESIVE; //�Ƿ������������
  config.enableTx = DEBUG_USART_ENABLE_SEND;   //�Ƿ�����������
  
  /*���ù̼��⺯�������޸ĺõ�������Ϣд�뵽���ڵ����üĴ�����*/
  LPUART_Init(DEBUG_USARTx, &config, BOARD_DEBUG_UART_CLK_FREQ);
  
//  /*��������ж�*/
//  LPUART_EnableInterrupts(DEBUG_USARTx, kLPUART_RxDataRegFullInterruptEnable);
//  EnableIRQ(DEBUG_USART_IRQ);
  
  //LPUART_EnableRx(DEBUG_USARTx, true);
  //LPUART_EnableTx(DEBUG_USARTx, false);
}

 /**
* @brief  ��ʼ��uart���Ź���
* @param  ��
* @retval ��
*/
void USART_IOMUXC_MUX_Config(void)
{
    /* RX��TX���� */
    IOMUXC_SetPinMux(UART_RX_IOMUXC, 0U);                                   
    IOMUXC_SetPinMux(UART_TX_IOMUXC, 0U); 
 }
 
 /**
* @brief  ��ʼ��uart���IOMUXC��PAD��������
* @param  ��
* @retval ��
*/
void USART_IOMUXC_PAD_Config(void)
{
  IOMUXC_SetPinConfig(UART_RX_IOMUXC, USART_RX_PAD_CONFIG_DATA);
  IOMUXC_SetPinConfig(UART_TX_IOMUXC, USART_TX_PAD_CONFIG_DATA);

}
  /**
* @brief  ��ʼ��uart,���������շ�����
* @param  ��
* @retval ��
*/
void USART_Config(void)
{
  USART_IOMUXC_MUX_Config();
  USART_IOMUXC_PAD_Config();
  USART_ModeConfig();
}
 
  /**
* @brief  ����һ���ַ� 
* @param  base:ʹ�õĴ���
* @param  data:��Ҫ���͵�����
* @retval ��
*/
void Usart_SendByte(LPUART_Type *base, uint8_t data)
{
  LPUART_WriteByte( base, data);
  while (!(base->STAT & LPUART_STAT_TDRE_MASK));
}
 
   /**
* @brief  �����ַ��� 
* @param  base:ʹ�õĴ���
* @param  str:��Ҫ���͵�����
* @retval ��
*/
/*****************  �����ַ��� **********************/
void Usart_SendString( LPUART_Type *base,  const char *str)
{
  LPUART_WriteBlocking( base, (const uint8_t*)str, strlen(str));
}
 
   /**
* @brief  ����һ��16λ�� 
* @param  base:ʹ�õĴ���
* @param  ch:��Ҫ���͵�����
* @retval ��
*/
void Usart_SendHalfWord(LPUART_Type *base, uint16_t ch)
{
  uint8_t temp_h, temp_l;
  
  /* ȡ���߰�λ */
  temp_h = (ch&0XFF00)>>8;
  /* ȡ���Ͱ�λ */
  temp_l = ch&0XFF; 

  /* ���͸߰�λ */
  LPUART_WriteByte( base, temp_h);
  while (!(base->STAT & LPUART_STAT_TDRE_MASK));
  
  /* ���͵Ͱ�λ */ 
  LPUART_WriteByte( base, temp_l);
  while (!(base->STAT & LPUART_STAT_TDRE_MASK));  
}
 

/******************���ڽ����жϷ�����********************/
 void DEBUG_USART_IRQHandler(void)
{
  uint8_t ucTemp;
  /*���ڽ��յ�����*/
  if ((kLPUART_RxDataRegFullFlag)&LPUART_GetStatusFlags(DEBUG_USARTx))
  {
     /*��ȡ����*/
     ucTemp = LPUART_ReadByte(DEBUG_USARTx);

     /*����ȡ��������д�뵽������*/
     Usart_SendByte(DEBUG_USARTx,ucTemp);
   }
}

