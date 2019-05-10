/**
  ******************************************************************************
  * @file    bsp_debug_usart.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   can�������ػ�ģʽ��
  ******************************************************************************
  * @attention
  *  
  * ʵ��ƽ̨:Ұ��  STM32 F103-�Ե� ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

#include "fsl_debug_console.h"
#include "./can/bsp_can.h"
#include "./can/bsp_can.h"













































///*ȫ�ֱ���������CAN����״̬��������main.c*/
//extern volatile bool rxComplete  ;
//
///*ȫ�ֱ�����������main.c*/
//extern flexcan_frame_t txFrame;
//extern flexcan_frame_t rxFrame;
//
//    
//    
// 
/**
* @brief  CAN���ų�ʼ��
*/  
static void CAN_GPIO_Config(void)   
{
  
}

/**
* @brief  CAN����ģʽ��ʼ��
*������baudRate: CAN ͨ�Ų�����
       LoopBack: CAN ����ģʽ��bool������Ϊ0 ����������ģʽ��Ϊ1 �����ڻػ�ģʽ
*/
void CAN_Mode_Config(uint32_t baudRate, bool LoopBack)
{
  /*CAN ���ýṹ��*/
  flexcan_config_t flexcanConfig;  //FlexCANģ�����ýṹ
  /*Clock setting for FLEXCAN*/
  CLOCK_SetMux(kCLOCK_CanMux, FLEXCAN_CLOCK_SOURCE_SELECT);
  CLOCK_SetDiv(kCLOCK_CanDiv, FLEXCAN_CLOCK_SOURCE_DIVIDER);
  
  /* ��ʼ�� FlexCAN . */
  /*
  * flexcanConfig.clkSrc = kFLEXCAN_ClkSrcOsc;
  * flexcanConfig.baudRate = 1000000U;         //���ò�����Ϊ1 Mbps
  * flexcanConfig.baudRateFD = 2000000U;       //���԰�FD��������bps
  * flexcanConfig.maxMbNum = 16;               //�û�ʹ�õ���Ϣ���������������
  * flexcanConfig.enableLoopBack = false;      //���ûػ��Լ�ģʽ
  * flexcanConfig.enableSelfWakeup = false;    //���û�����Ի���ģʽ
  * flexcanConfig.enableIndividMask = false;   //���û����Rx��������
  * flexcanConfig.enableDoze = false;          //���û����dozeģʽ
  */
  FLEXCAN_GetDefaultConfig(&flexcanConfig);
  
  flexcanConfig.enableLoopBack = LoopBack;//����Ϊ�ػ�ģʽ
  flexcanConfig.baudRate = baudRate;//����Ϊ�ػ�ģʽ
  FLEXCAN_Init(EXAMPLE_CAN, &flexcanConfig, EXAMPLE_CAN_CLK_FREQ);
}

/**
* @brief  ��ʼ��CAN���ջ�����
*/
void CAN_RX_Buffer_Config(uint32_t ID_STD,uint8_t RX_MB)
{
  flexcan_rx_mb_config_t mbConfig; //FlexCAN���ջ��������ýṹ
  
  
  mbConfig.format = kFLEXCAN_FrameFormatStandard;//CAN ֡��ʽ����׼֡����չ֡
  mbConfig.type = kFLEXCAN_FrameTypeData;        //CAN ֡���ͣ�����֡��Զ��֡
  mbConfig.id = FLEXCAN_ID_STD(ID_STD);           //��Ϣ������֡��ʶ����
  
  FLEXCAN_SetRxMbConfig(EXAMPLE_CAN, RX_MB, &mbConfig, true);//���ý�����Ϣ������
  
//   /* Enable Rx Message Buffer interrupt. */
//  FLEXCAN_EnableMbInterrupts(EXAMPLE_CAN, 1 << RX_MB);//����Rx��Ϣ�������ж�
//  EnableIRQ(EXAMPLE_FLEXCAN_IRQn);    /* Enable Rx Message Buffer interrupt. */
}
//
//
//
/**
* @brief  ��ʼ��CAN���ͻ�����
*/
void CAN_TX_Buffer_Config(uint32_t ID_STD,flexcan_frame_t* _txFrame )
{
  flexcan_frame_t* txFrame = _txFrame;
  /* ׼��Ҫ���͵�����Prepare Tx Frame for sending. */
  txFrame->format = kFLEXCAN_FrameFormatStandard;//����Ϊ��׼��ʽ����Ϊ��չ�ͱ�׼��ʽ��
  txFrame->type = kFLEXCAN_FrameTypeData;        //����λ����֡����Ϊ����֡��ң��֡��
  txFrame->id = FLEXCAN_ID_STD(ID_STD);          //����֡ID ��ID�������ݰ���CAN�����ϵķ������ȼ����Լ���˭���ո�֡��
  txFrame->length = DLC;                 //�������ݳ��ȣ�����ѡ��Χ0~8��
 
  /*����Ҫ���͵�����*/
  txFrame->dataWord0 = CAN_WORD0_DATA_BYTE_0(0x11) | CAN_WORD0_DATA_BYTE_1(0x22) | CAN_WORD0_DATA_BYTE_2(0x33) |
  CAN_WORD0_DATA_BYTE_3(0x44);
  txFrame->dataWord1 = CAN_WORD1_DATA_BYTE_4(0x55) | CAN_WORD1_DATA_BYTE_5(0x66) | CAN_WORD1_DATA_BYTE_6(0x77) |
  CAN_WORD1_DATA_BYTE_7(0x88);
}


/**
* @brief  CAN��ʼ��
*/ 
void CAN_Config(void)
{
  /*CAN���ų�ʼ��*/
  CAN_GPIO_Config();
  /*CAN����ģʽ��ʼ��*/
  CAN_Mode_Config(1000000U,0);
}
//    
//
//
//    
///**
//* @brief  CAN�����жϷ�����
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






