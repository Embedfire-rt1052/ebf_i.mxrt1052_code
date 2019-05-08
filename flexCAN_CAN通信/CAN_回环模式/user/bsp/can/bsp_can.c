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

#include "./can/bsp_can.h"
#include "./can/bsp_can.h"

/*ȫ�ֱ���������CAN����״̬��������main.c*/
extern volatile bool rxComplete  ;

/*ȫ�ֱ�����������main.c*/
extern flexcan_frame_t txFrame;
extern flexcan_frame_t rxFrame;

    
    
 
/**
* @brief  CAN���ų�ʼ��
*/  
static void CAN_GPIO_Config(void)   
{
  
}

/**
* @brief  CAN����ģʽ��ʼ��
*/
static void CAN_Mode_Config(void)
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
  flexcanConfig.enableLoopBack = true;//����Ϊ�ػ�ģʽ
  FLEXCAN_Init(EXAMPLE_CAN, &flexcanConfig, EXAMPLE_CAN_CLK_FREQ);
  
}

/**
* @brief  ��ʼ��CAN���ջ�����
*/
void CAN_RX_Buffer_Config(void)
{
  flexcan_rx_mb_config_t mbConfig; //FlexCAN���ջ��������ýṹ
  
  
  mbConfig.format = kFLEXCAN_FrameFormatStandard;//CAN ֡��ʽ����׼֡����չ֡
  mbConfig.type = kFLEXCAN_FrameTypeData;        //CAN ֡���ͣ�����֡��Զ��֡
  mbConfig.id = FLEXCAN_ID_STD(0x123);           //��Ϣ������֡��ʶ����
  
  FLEXCAN_SetRxMbConfig(EXAMPLE_CAN, RX_MESSAGE_BUFFER_NUM, &mbConfig, true);//���ý�����Ϣ������
  
}

/**
* @brief  ��ʼ��CAN���ջ�����
*/

//void CAN_RX_Buffer_Config2(flexcan_rx_mb_config_t* rx_mb, )
//{
//  flexcan_rx_mb_config_t mbConfig; //FlexCAN���ջ��������ýṹ
//  
//  
//  mbConfig.format = kFLEXCAN_FrameFormatStandard;//CAN ֡��ʽ����׼֡����չ֡
//  mbConfig.type = kFLEXCAN_FrameTypeData;        //CAN ֡���ͣ�����֡��Զ��֡
//  mbConfig.id = FLEXCAN_ID_STD(0x123);           //��Ϣ������֡��ʶ����
//  
//  FLEXCAN_SetRxMbConfig(EXAMPLE_CAN, RX_MESSAGE_BUFFER_NUM, &mbConfig, true);//���ý�����Ϣ������
//  
//}

/**
* @brief  ��ʼ��CAN���ͻ�����
*/
void CAN_TX_Buffer_Config(void)
{
  FLEXCAN_SetTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, true);//����Tx��Ϣ������
  
  /* Enable Rx Message Buffer interrupt. */
  FLEXCAN_EnableMbInterrupts(EXAMPLE_CAN, 1 << RX_MESSAGE_BUFFER_NUM);//����Rx��Ϣ�������ж�
  EnableIRQ(EXAMPLE_FLEXCAN_IRQn);    /* Enable Rx Message Buffer interrupt. */

  
  /* ׼��Ҫ���͵�����Prepare Tx Frame for sending. */
  txFrame.format = kFLEXCAN_FrameFormatStandard;//����Ϊ��׼��ʽ����Ϊ��չ�ͱ�׼��ʽ��
  txFrame.type = kFLEXCAN_FrameTypeData;        //����λ����֡����Ϊ����֡��ң��֡��
  txFrame.id = FLEXCAN_ID_STD(0x123);           //����֡ID ��ID�������ݰ���CAN�����ϵķ������ȼ����Լ���˭���ո�֡��
  txFrame.length = DLC;                         //�������ݳ��ȣ�����ѡ��Χ0~8��
  
  txFrame.dataWord0 = CAN_WORD0_DATA_BYTE_0(0x88) | CAN_WORD0_DATA_BYTE_1(0x22) | CAN_WORD0_DATA_BYTE_2(0x33) |
    CAN_WORD0_DATA_BYTE_3(0x44);
  txFrame.dataWord1 = CAN_WORD1_DATA_BYTE_4(0x55) | CAN_WORD1_DATA_BYTE_5(0x66) | CAN_WORD1_DATA_BYTE_6(0x77) |
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
  CAN_Mode_Config();
}
    


    
/**
* @brief  CAN�����жϷ�����
*/  
void EXAMPLE_FLEXCAN_IRQHandler(void)
{
    /* If new data arrived. */
    if (FLEXCAN_GetMbStatusFlags(EXAMPLE_CAN, 1 << RX_MESSAGE_BUFFER_NUM))
    {
        FLEXCAN_ClearMbStatusFlags(EXAMPLE_CAN, 1 << RX_MESSAGE_BUFFER_NUM);

        FLEXCAN_ReadRxMb(EXAMPLE_CAN, RX_MESSAGE_BUFFER_NUM, &rxFrame);

        rxComplete = true;
    }
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}


/**************************END OF FILE************************************/











