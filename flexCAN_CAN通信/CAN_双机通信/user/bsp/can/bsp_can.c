/**
  ******************************************************************************
  * @file    bsp_debug_usart.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   can驱动（回环模式）
  ******************************************************************************
  * @attention
  *  
  * 实验平台:野火  STM32 F103-霸道 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

#include "fsl_debug_console.h"
#include "./can/bsp_can.h"
#include "./can/bsp_can.h"













































///*全局变量，保存CAN接收状态，定义在main.c*/
//extern volatile bool rxComplete  ;
//
///*全局变量，定义在main.c*/
//extern flexcan_frame_t txFrame;
//extern flexcan_frame_t rxFrame;
//
//    
//    
// 
/**
* @brief  CAN引脚初始化
*/  
static void CAN_GPIO_Config(void)   
{
  
}

/**
* @brief  CAN工作模式初始化
*参数：baudRate: CAN 通信波特率
       LoopBack: CAN 工作模式，bool变量，为0 工作在正常模式，为1 工作在回环模式
*/
void CAN_Mode_Config(uint32_t baudRate, bool LoopBack)
{
  /*CAN 配置结构体*/
  flexcan_config_t flexcanConfig;  //FlexCAN模块配置结构
  /*Clock setting for FLEXCAN*/
  CLOCK_SetMux(kCLOCK_CanMux, FLEXCAN_CLOCK_SOURCE_SELECT);
  CLOCK_SetDiv(kCLOCK_CanDiv, FLEXCAN_CLOCK_SOURCE_DIVIDER);
  
  /* 初始化 FlexCAN . */
  /*
  * flexcanConfig.clkSrc = kFLEXCAN_ClkSrcOsc;
  * flexcanConfig.baudRate = 1000000U;         //设置波特率为1 Mbps
  * flexcanConfig.baudRateFD = 2000000U;       //柔性版FD波特率在bps
  * flexcanConfig.maxMbNum = 16;               //用户使用的消息缓冲区的最大数量
  * flexcanConfig.enableLoopBack = false;      //禁用回环自检模式
  * flexcanConfig.enableSelfWakeup = false;    //启用或禁用自唤醒模式
  * flexcanConfig.enableIndividMask = false;   //启用或禁用Rx单个掩码
  * flexcanConfig.enableDoze = false;          //启用或禁用doze模式
  */
  FLEXCAN_GetDefaultConfig(&flexcanConfig);
  
  flexcanConfig.enableLoopBack = LoopBack;//设置为回环模式
  flexcanConfig.baudRate = baudRate;//设置为回环模式
  FLEXCAN_Init(EXAMPLE_CAN, &flexcanConfig, EXAMPLE_CAN_CLK_FREQ);
}

/**
* @brief  初始化CAN接收缓冲区
*/
void CAN_RX_Buffer_Config(uint32_t ID_STD,uint8_t RX_MB)
{
  flexcan_rx_mb_config_t mbConfig; //FlexCAN接收缓冲区配置结构
  
  
  mbConfig.format = kFLEXCAN_FrameFormatStandard;//CAN 帧格式，标准帧或扩展帧
  mbConfig.type = kFLEXCAN_FrameTypeData;        //CAN 帧类型，数据帧或远程帧
  mbConfig.id = FLEXCAN_ID_STD(ID_STD);           //消息缓冲区帧标识符，
  
  FLEXCAN_SetRxMbConfig(EXAMPLE_CAN, RX_MB, &mbConfig, true);//配置接受消息缓冲区
  
//   /* Enable Rx Message Buffer interrupt. */
//  FLEXCAN_EnableMbInterrupts(EXAMPLE_CAN, 1 << RX_MB);//启用Rx消息缓冲区中断
//  EnableIRQ(EXAMPLE_FLEXCAN_IRQn);    /* Enable Rx Message Buffer interrupt. */
}
//
//
//
/**
* @brief  初始化CAN发送缓冲区
*/
void CAN_TX_Buffer_Config(uint32_t ID_STD,flexcan_frame_t* _txFrame )
{
  flexcan_frame_t* txFrame = _txFrame;
  /* 准备要发送的数据Prepare Tx Frame for sending. */
  txFrame->format = kFLEXCAN_FrameFormatStandard;//设置为标准格式（分为扩展和标准格式）
  txFrame->type = kFLEXCAN_FrameTypeData;        //设置位数据帧（分为数据帧和遥控帧）
  txFrame->id = FLEXCAN_ID_STD(ID_STD);          //设置帧ID （ID决定数据包在CAN网络上的发送优先级，以及有谁接收该帧）
  txFrame->length = DLC;                 //设置数据长度，（可选范围0~8）
 
  /*设置要发送的数据*/
  txFrame->dataWord0 = CAN_WORD0_DATA_BYTE_0(0x11) | CAN_WORD0_DATA_BYTE_1(0x22) | CAN_WORD0_DATA_BYTE_2(0x33) |
  CAN_WORD0_DATA_BYTE_3(0x44);
  txFrame->dataWord1 = CAN_WORD1_DATA_BYTE_4(0x55) | CAN_WORD1_DATA_BYTE_5(0x66) | CAN_WORD1_DATA_BYTE_6(0x77) |
  CAN_WORD1_DATA_BYTE_7(0x88);
}


/**
* @brief  CAN初始化
*/ 
void CAN_Config(void)
{
  /*CAN引脚初始化*/
  CAN_GPIO_Config();
  /*CAN工作模式初始化*/
  CAN_Mode_Config(1000000U,0);
}
//    
//
//
//    
///**
//* @brief  CAN接收中断服务函数
//*/  
//void EXAMPLE_FLEXCAN_IRQHandler(void)
//{
//    /* If new data arrived. */
//    if (FLEXCAN_GetMbStatusFlags(EXAMPLE_CAN, 1 << RX_MESSAGE_BUFFER_NUM))
//    {
//        FLEXCAN_ClearMbStatusFlags(EXAMPLE_CAN, 1 << RX_MESSAGE_BUFFER_NUM);
//
//        FLEXCAN_ReadRxMb(EXAMPLE_CAN, RX_MESSAGE_BUFFER_NUM, &rxFrame);
//
//        rxComplete = true;
//    }
///* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
//  exception return operation might vector to incorrect interrupt */
//#if defined __CORTEX_M && (__CORTEX_M == 4U)
//    __DSB();
//#endif
//}


/**************************END OF FILE************************************/




//  if((data_length>8) || (sizeof(text)>8))
//  {
//    data_length = 8;
//  }
//
//  
//  PRINTF("data is: %c\r\n",*data);
//  PRINTF("data is: %c\r\n",*(data+1));
//  if(data_length <= 4)
//  {
//    PRINTF("data_length is: %d\r\n",data_length);
//   
//    for(i=24; i>=32-8*data_length; i = i-8)
//    {
// 
//      data_1 |= ((uint32_t)(*data))<<i;
//      data++;
//    }
//    txFrame.dataWord0 = data_1;
//  }
//  if(data_length > 4)
//  {
//    for(i=24; i>=0; i-=8)
//    {
//      data_1 |= (*data)<<i;
//      data++;
//    }
//    
//    for(i=24; i>64-8*data_length; i-=8)
//    {
//      data_1 |= (*data)<<i;
//      data++;
//    }
//    txFrame.dataWord0 = data_2;
//  }
// 






