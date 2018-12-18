/**
  ******************************************************************
  * @file    bsp_usart.c
  * @author  fire
  * @version V1.1
  * @date    2018-xx-xx
  * @brief   uart应用函数接口
  ******************************************************************
  * @attention
  *
  * 实验平台:野火  i.MXRT1052开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
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
* @brief  初始化uart配置参数
* @param  无
* @retval 无
*/
void USART_ModeConfig(void)
{
  /*定义串口配置参数结构体变量，用于保存串口的配置信息*/
  lpuart_config_t config;
  
  /*调用固件库函数得到默认的串口配置参数，在默认的配置参数基础上修改*/
  LPUART_GetDefaultConfig(&config);
  config.baudRate_Bps = DEBUG_USART_BAUDRATE;  //波特率
  config.enableRx = DEBUG_USART_ENABLE_RESIVE; //是否允许接收数据
  config.enableTx = DEBUG_USART_ENABLE_SEND;   //是否允许发送数据
  
  /*调用固件库函数，将修改好的配置信息写入到串口的配置寄存器中*/
  LPUART_Init(DEBUG_USARTx, &config, BOARD_DEBUG_UART_CLK_FREQ);
  
//  /*允许接收中断*/
//  LPUART_EnableInterrupts(DEBUG_USARTx, kLPUART_RxDataRegFullInterruptEnable);
//  EnableIRQ(DEBUG_USART_IRQ);
  
  //LPUART_EnableRx(DEBUG_USARTx, true);
  //LPUART_EnableTx(DEBUG_USARTx, false);
}

 /**
* @brief  初始化uart引脚功能
* @param  无
* @retval 无
*/
void USART_IOMUXC_MUX_Config(void)
{
    /* RX和TX引脚 */
    IOMUXC_SetPinMux(UART_RX_IOMUXC, 0U);                                   
    IOMUXC_SetPinMux(UART_TX_IOMUXC, 0U); 
 }
 
 /**
* @brief  初始化uart相关IOMUXC的PAD属性配置
* @param  无
* @retval 无
*/
void USART_IOMUXC_PAD_Config(void)
{
  IOMUXC_SetPinConfig(UART_RX_IOMUXC, USART_RX_PAD_CONFIG_DATA);
  IOMUXC_SetPinConfig(UART_TX_IOMUXC, USART_TX_PAD_CONFIG_DATA);

}
  /**
* @brief  初始化uart,并开启了收发功能
* @param  无
* @retval 无
*/
void USART_Config(void)
{
  USART_IOMUXC_MUX_Config();
  USART_IOMUXC_PAD_Config();
  USART_ModeConfig();
}
 
  /**
* @brief  发送一个字符 
* @param  base:使用的串口
* @param  data:将要发送的数据
* @retval 无
*/
void Usart_SendByte(LPUART_Type *base, uint8_t data)
{
  LPUART_WriteByte( base, data);
  while (!(base->STAT & LPUART_STAT_TDRE_MASK));
}
 
   /**
* @brief  发送字符串 
* @param  base:使用的串口
* @param  str:将要发送的数据
* @retval 无
*/
/*****************  发送字符串 **********************/
void Usart_SendString( LPUART_Type *base,  const char *str)
{
  LPUART_WriteBlocking( base, (const uint8_t*)str, strlen(str));
}
 
   /**
* @brief  发送一个16位数 
* @param  base:使用的串口
* @param  ch:将要发送的数据
* @retval 无
*/
void Usart_SendHalfWord(LPUART_Type *base, uint16_t ch)
{
  uint8_t temp_h, temp_l;
  
  /* 取出高八位 */
  temp_h = (ch&0XFF00)>>8;
  /* 取出低八位 */
  temp_l = ch&0XFF; 

  /* 发送高八位 */
  LPUART_WriteByte( base, temp_h);
  while (!(base->STAT & LPUART_STAT_TDRE_MASK));
  
  /* 发送低八位 */ 
  LPUART_WriteByte( base, temp_l);
  while (!(base->STAT & LPUART_STAT_TDRE_MASK));  
}
 

/******************串口接收中断服务函数********************/
 void DEBUG_USART_IRQHandler(void)
{
  uint8_t ucTemp;
  /*串口接收到数据*/
  if ((kLPUART_RxDataRegFullFlag)&LPUART_GetStatusFlags(DEBUG_USARTx))
  {
     /*读取数据*/
     ucTemp = LPUART_ReadByte(DEBUG_USARTx);

     /*将读取到的数据写入到缓冲区*/
     Usart_SendByte(DEBUG_USARTx,ucTemp);
   }
}

