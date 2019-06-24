/**
******************************************************************
* @file    main.c
* @author  fire
* @version V1.0
* @date    2018-xx-xx
* @brief   �����жϽ��ղ���
******************************************************************
* @attention
*
* ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
* ��̳    :http://www.firebbs.cn
* �Ա�    :http://firestm32.taobao.com
*CPU_MIMXRT1052DVL6B,PRINTF_FLOAT_ENABLE=1, SCANF_FLOAT_ENABLE=1, PRINTF_ADVANCED_ENABLE=1, SCANF_ADVANCED_ENABLE=1
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

   

flexcan_handle_t flexcanHandle;    //����flexcan���
volatile bool txComplete = false; //������ɱ�־
volatile bool rxComplete = false; //������ɱ�־
volatile bool wakenUp = false;    //���ѱ�־    

flexcan_mb_transfer_t txXfer;  //���巢��MB(Message Buffer)
flexcan_mb_transfer_t rxXfer;  //�������MB(Message Buffer)

flexcan_frame_t  rx_frame;//���������Ϣ��ܽṹ��
flexcan_frame_t  tx_frame;//���巢����Ϣ��ܽṹ��


uint32_t txIdentifier;//���巢�ͱ�ʶ��
uint32_t rxIdentifier;//������ձ�ʶ��




/*!
 * @brief FlexCAN Call Back function
 */
static void flexcan_callback(CAN_Type *base, flexcan_handle_t *handle, status_t status, uint32_t result, void *userData)
{
    switch (status)
    {
        case kStatus_FLEXCAN_RxIdle:
            if (RX_MESSAGE_BUFFER_NUM == result)
            {
                rxComplete = true;
            }
            break;

        case kStatus_FLEXCAN_TxIdle:
        case kStatus_FLEXCAN_TxSwitchToRx:
            if (TX_MESSAGE_BUFFER_NUM == result)
            {
                txComplete = true;
            }
            break;

        case kStatus_FLEXCAN_WakeUp:
            wakenUp = true;
            break;

        default:
            break;
    }
}



/*!
 * @brief Main function
 */
int main(void)
{
    flexcan_config_t flexcanConfig;//����FlexCAN��ʼ�����������ڳ�ʼ��FlexCAN
    uint8_t node_type;             //���ڱ���ʹ��GETCHAR()�õ����ַ�

    /* ϵͳ��ʼ��*/
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /*����FlexCAN��ʱ��Ƶ��*/
    CLOCK_SetMux(kCLOCK_CanMux, FLEXCAN_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_CanDiv, FLEXCAN_CLOCK_SOURCE_DIVIDER);
    
    /*�����ʾ��Ϣ*/
    PRINTF("********* FLEXCAN Interrupt EXAMPLE *********\r\n");
    PRINTF("    Message format: Standard (11 bit id)\r\n");
    PRINTF("    Message buffer %d used for Rx.\r\n", RX_MESSAGE_BUFFER_NUM);
    PRINTF("    Message buffer %d used for Tx.\r\n", TX_MESSAGE_BUFFER_NUM);
    PRINTF("    Interrupt Mode: Enabled\r\n");
    PRINTF("    Operation Mode: TX and RX --> Normal\r\n");
    PRINTF("*********************************************\r\n\r\n");
    
    
   /*����һ��CAN˫��ͨ��ʵ��
    *�����ձ�ʶ���뷢�ͱ�ʶ������Ϊ��ֵͬ��������Ķ����򼴿ɽ�
    *�������ص������������ϼ��ɹ۲쵽ʵ������
    */
    txIdentifier = 0x123;
    rxIdentifier = 0x123;


    
    /*��ȡĬ������*/
    FLEXCAN_GetDefaultConfig(&flexcanConfig);
    /* ��ʼ��FlexCAN*/
    CAN_Config();
    
    
    /* ����CAN���ƾ���������ûص�����*/
    FLEXCAN_TransferCreateHandle(EXAMPLE_CAN, &flexcanHandle, flexcan_callback, NULL);

    /*���ý��չ�����*/
    FLEXCAN_SetRxMbGlobalMask(EXAMPLE_CAN, FLEXCAN_RX_MB_STD_MASK(rxIdentifier, 0, 0));

    
    /* ���ý�����Ϣ��������MB��Setup Rx Message Buffer. */
    CAN_RX_Buffer_Config(rxIdentifier,RX_MESSAGE_BUFFER_NUM);//��ʼ�����ջ�����
    /* ���÷�����Ϣ������. */
    FLEXCAN_SetTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, true);

    
    do
    {
        PRINTF("Please select local node as A or B:\r\n");
        PRINTF("Note: Node B should start first.\r\n");
        PRINTF("Node:");
        node_type = GETCHAR();
        PRINTF("%c", node_type);
        PRINTF("\r\n");
    } while ((node_type != 'A') && (node_type != 'B') && (node_type != 'a') && (node_type != 'b'));
    
    
    if ((node_type == 'A') || (node_type == 'a'))
    {
        PRINTF("Press any key to trigger one-shot transmission\r\n\r\n");
    }
   

    while (1)
    {
        if ((node_type == 'A') || (node_type == 'a'))
        {
          GETCHAR();
          tx_frame.id = FLEXCAN_ID_STD(txIdentifier);
          tx_frame.format = kFLEXCAN_FrameFormatStandard;
          tx_frame.type = kFLEXCAN_FrameTypeData;
          tx_frame.length = DLC;
          
          tx_frame.dataByte0 = 'a';
          tx_frame.dataByte1 = 'b';
          tx_frame.dataByte2 = 'c';
          tx_frame.dataByte3 = 'd';
          tx_frame.dataByte4 = 'e';
          tx_frame.dataByte5 = 'f';
          tx_frame.dataByte6 = 'g';
          tx_frame.dataByte7 = 'h';
          
          
          /*����ȫ�ֵ� MB */
          txXfer.mbIdx = TX_MESSAGE_BUFFER_NUM;
          txXfer.frame = &tx_frame;
          FLEXCAN_TransferSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
          
          while (!txComplete)
          {
          };
          txComplete = false;
          PRINTF("send success");
        }
        else
        {
          
          rx_frame.id = FLEXCAN_ID_STD(txIdentifier);
          rx_frame.format = kFLEXCAN_FrameFormatStandard;
          rx_frame.type = kFLEXCAN_FrameTypeData;
          rx_frame.length = DLC;

          /*�ȴ���������*/
          rxXfer.mbIdx = RX_MESSAGE_BUFFER_NUM;
          rxXfer.frame = &rx_frame;
          
          while(1)
          {
            /*�����ǰ���տ��л��߽���δ���*/
            if(!rxComplete)
            {
              /*�������*/
              FLEXCAN_TransferReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxXfer);
            }    
            
            /*������ɣ�������յ�������*/
            if(rxComplete)
            {
              PRINTF("resive success\r\n");
              /*���*/
              PRINTF(" Rx MB ID: 0x%3x, \r\n Rx MB data0:%c, data1:%c, data2:%c, data3:%c, data4:%c, data5:%c, data6:%c, data7:%c,\r\n Time stamp: %d\r\n",\
                rx_frame.id >> CAN_ID_STD_SHIFT,\
                rx_frame.dataByte0,\
                rx_frame.dataByte1,\
                rx_frame.dataByte2,\
                rx_frame.dataByte3,\
                rx_frame.dataByte4,\
                rx_frame.dataByte5,\
                rx_frame.dataByte6,\
                rx_frame.dataByte7,\
                rx_frame.timestamp);
              rxComplete = 0;//���ý�����ɱ�־Ϊ0����ʾû���յ����ݣ�
            }
          }

        }
    }
}

/****************************END OF FILE**********************/






