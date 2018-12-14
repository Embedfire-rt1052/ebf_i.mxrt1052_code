/**
  ******************************************************************************
  * @file    rx_data_queue.c
  * @author  fire
  * @version V1.0
  * @date    2015-01-xx
  * @brief   使用数组模拟环形缓冲区。
  ******************************************************************************
  * @attention
  *
  * 实验平台:
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

#include "bsp_rx_data_queue.h"
#include "bsp_usart.h"


//队列缓冲区的内存池
//__align(4) char resive_buff[QUEUE_RESIVE_FIFO_LEN] ;
//__align(4) char send_buff[QUEUE_SEND_FIFO_LEN] ;


 char resive_buff[QUEUE_RESIVE_FIFO_LEN] ;
 char send_buff[QUEUE_SEND_FIFO_LEN] ;
/*实例化接收和发送FIFO结构体*/
 volatile ESP_USART_FRAME resive_fifo_struct;
 volatile ESP_USART_FRAME send_fifo_struct;


 
 
 

/**
  * @brief  初始化resive_fifo_struct结构体
  */
void rx_queue_init(void)
{
  resive_fifo_struct.head = &(resive_buff[0]);
  resive_fifo_struct.write = 0;
  resive_fifo_struct.read = 0;
  resive_fifo_struct.len = 0;
}


/**
  * @brief  初始化send_fifo_struct结构体
  */
void tx_queue_init(void)
{
  send_fifo_struct.head = &(resive_buff[0]);
  send_fifo_struct.write = 0;
  send_fifo_struct.read = 0;
  send_fifo_struct.len = 0;
}


///**
//  * @brief  判断接收缓存是(0)否(1)可以继续接收数据
//  */
//int rxFifoIsFull(void)
//{
//  ;
//}


///**
//  * @brief  判断发送缓存是(0)否(1)可以继续存放待发送数据
//  */
//int txFifoIfFull(void)
//{
//  ;
//}

/**
  * @brief  将接收到的数据存放到接收缓冲区
  * @param  fifo_struct：数据缓冲区地址，a：要存放的数据
  */
void push_data_to_queue(volatile ESP_USART_FRAME* fifo_struct,char a)
{
  fifo_struct->head[fifo_struct->write] = a;
  fifo_struct->len += 1;
  fifo_struct->write += 1;
  if(fifo_struct->write >= QUEUE_RESIVE_FIFO_LEN)
  {
    fifo_struct->write = 0;
  }
  
}

/**
  * @brief  输出接收缓冲区数据
  * @param  
  */
void pull_data_from_reisve_queue(void)
{
  char a;
  if(resive_fifo_struct.len > 0)
  {
    a = resive_fifo_struct.head[resive_fifo_struct.read];
    Usart_SendByte(DEBUG_USARTx, a);
    resive_fifo_struct.read += 1;
    resive_fifo_struct.len -= 1;
    
    if(resive_fifo_struct.read >= QUEUE_RESIVE_FIFO_LEN)
    {
      resive_fifo_struct.read = 0;
    }
  }
}


