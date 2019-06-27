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

#include "./bsp/nvic/bsp_nvic.h"
#include "./bsp/485/bsp_485.h"


volatile uint8_t ucTemp = ' '; //���ڱ����յ����ַ�
volatile bool resived = false;//���ڱ������״̬


/**
* @brief  ��ʼ��uart���ò���
* @param  ��
* @retval ��
*/
void UART_ModeConfig(void)
{
  /*���崮�����ò����ṹ����������ڱ��洮�ڵ�������Ϣ*/
  lpuart_config_t config;
  
  /*���ù̼��⺯���õ�Ĭ�ϵĴ������ò�������Ĭ�ϵ����ò����������޸�*/
  LPUART_GetDefaultConfig(&config);
  config.baudRate_Bps = DEBUG_UART_BAUDRATE;  //������
  config.enableRx = DEBUG_UART_ENABLE_RESIVE; //�Ƿ������������
  config.enableTx = DEBUG_UART_ENABLE_SEND;   //�Ƿ�����������

  /*���ù̼��⺯�������޸ĺõ�������Ϣд�뵽���ڵ����üĴ�����*/
  LPUART_Init(DEBUG_UARTx, &config, BOARD_DEBUG_UART_CLK_FREQ);
  
  /*��������ж�*/
  LPUART_EnableInterrupts(DEBUG_UARTx, kLPUART_RxDataRegFullInterruptEnable);
  /*�����ж����ȼ�,*/
  set_IRQn_Priority(DEBUG_UART_IRQ,Group4_PreemptPriority_6, Group4_SubPriority_0);
  /*ʹ���ж�*/
  EnableIRQ(DEBUG_UART_IRQ);
  

}

 /**
* @brief  ��ʼ��uart���Ź���
* @param  ��
* @retval ��
*/
void UART_IOMUXC_MUX_Config(void)
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
void UART_IOMUXC_PAD_Config(void)
{
  IOMUXC_SetPinConfig(UART_RX_IOMUXC, UART_RX_PAD_CONFIG_DATA);
  IOMUXC_SetPinConfig(UART_TX_IOMUXC, UART_TX_PAD_CONFIG_DATA);
}
  /**
* @brief  ��ʼ��uart,���������շ�����
* @param  ��
* @retval ��
*/
void UART_Config(void)
{
  UART_IOMUXC_MUX_Config();
  UART_IOMUXC_PAD_Config();
  UART_ModeConfig();
}

  /**
* @brief  ����һ���ַ� 
* @param  base:ѡ��˿�
* @param  data:��Ҫ���͵�����
* @retval ��
*/
void Uart_SendByte(LPUART_Type *base, uint8_t data)
{
  LPUART_WriteByte( base, data);
  while (!(base->STAT & LPUART_STAT_TDRE_MASK));
}

  /**
* @brief  ����һ���ַ��� 
* @param  base:ѡ��˿�
* @param  data:��Ҫ���͵�����
* @retval ��
*/
void Uart_SendString( LPUART_Type *base,  const char *str)
{
  LPUART_WriteBlocking( base, (const uint8_t *)str, strlen(str));
}
 

  /**
* @brief  ����һ��16λ��  
* @param  base:ѡ��˿�
* @param  data:��Ҫ���͵�����
* @retval ��
*/
void Uart_SendHalfWord(LPUART_Type *base, uint16_t ch)
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
 




/***************һ��������485����ڴ��������ӻ��޸ĵ�����******/

void DEBUG_UART_IRQHandler(void)
{
  
  
  /*���ڽ��յ�����*/
  if ((kLPUART_RxDataRegFullFlag)&LPUART_GetStatusFlags(DEBUG_UARTx))
  {
    /*��ȡ����*/
    ucTemp = LPUART_ReadByte(DEBUG_UARTx);
    resived = true;
  }
}


/*
*�������ܣ���ʼ��485ͨ�ſ������ţ�Ĭ�ϵ�ƽΪ�͵�ƽ����Ĭ�������Ϊ
*����״̬��
*/
void _485_Control_GPIO_init(void)
{
  /* ����gpio��ʼ�����ýṹ�� */
  gpio_pin_config_t led_config;     
  
  IOMUXC_SetPinMux(_485_RE_IOMUXC, 0U);
  IOMUXC_SetPinConfig(_485_RE_IOMUXC, RE_485_PAD_CONFIG_DATA);
  
  /*GPIO����*/       
  led_config.direction = kGPIO_DigitalOutput; //���ģʽ
  led_config.outputLogic =  0;                //Ĭ�ϸߵ�ƽ
  led_config.interruptMode = kGPIO_NoIntmode; //��ʹ���ж�
  
  /* ��ʼ��  GPIO. */
  GPIO_PinInit(_485_RE_GPIO, _485_RE_GPIO_PIN, &led_config);
}


/*
*�������ܣ�ִ��485����һ���ֽ�
*/
void _485_SendByte(LPUART_Type *base, uint8_t data)
{
  _485_Send                                     //ʹ�ܷ���
  _485_delay(90000);
  LPUART_WriteByte( base, data);                //ִ�з���
  while (!(base->STAT & LPUART_STAT_TDRE_MASK));//�ȴ��������
  _485_delay(90000);
  _485_Resive
}

/*���ӳٺ���*/
static void _485_delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
} 




















