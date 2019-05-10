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



/*******************************************************************************
 * Definitions
 ******************************************************************************/




#if (defined(FSL_FEATURE_FLEXCAN_HAS_ERRATA_5829) && FSL_FEATURE_FLEXCAN_HAS_ERRATA_5829)
/* To consider the First valid MB must be used as Reserved TX MB for ERR005829
   If RX FIFO enable(RFEN bit in MCE set as 1) and RFFN in CTRL2 is set default zero, the first valid TX MB Number is 8
   If RX FIFO enable(RFEN bit in MCE set as 1) and RFFN in CTRL2 is set by other value(0x1~0xF), User should consider
   detail first valid MB number
   If RX FIFO disable(RFEN bit in MCE set as 0) , the first valid MB number is zero */
//#ifdef RX_MESSAGE_BUFFER_NUM
//#undef RX_MESSAGE_BUFFER_NUM
//#define RX_MESSAGE_BUFFER_NUM (10)
//#endif
//#ifdef TX_MESSAGE_BUFFER_NUM
//#undef TX_MESSAGE_BUFFER_NUM
//#define TX_MESSAGE_BUFFER_NUM (9)
//#endif
#endif
//#ifndef DEMO_FORCE_CAN_SRC_OSC
//#define DEMO_FORCE_CAN_SRC_OSC 0
//#endif

   
   
   

flexcan_handle_t flexcanHandle;    //����flexcan���
volatile bool txComplete = false; //������ɱ�־
volatile bool rxComplete = false; //������ɱ�־
volatile bool wakenUp = false;    //���ѱ�־    

flexcan_mb_transfer_t txXfer;  //���巢��MB(Message Buffer)
flexcan_mb_transfer_t rxXfer;  //�������MB(Message Buffer)

//#if (defined(USE_CANFD) && USE_CANFD)
//flexcan_fd_frame_t frame;
//#else
flexcan_frame_t frame;
//#endif
uint32_t txIdentifier;
uint32_t rxIdentifier;

/*******************************************************************************
 * Code
 ******************************************************************************/
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
    flexcan_config_t flexcanConfig;
    flexcan_rx_mb_config_t mbConfig;
    uint8_t node_type;

    /* Initialize board hardware. */
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /*Clock setting for FLEXCAN*/
    CLOCK_SetMux(kCLOCK_CanMux, FLEXCAN_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_CanDiv, FLEXCAN_CLOCK_SOURCE_DIVIDER);

    PRINTF("********* FLEXCAN Interrupt EXAMPLE *********\r\n");
    PRINTF("    Message format: Standard (11 bit id)\r\n");
    PRINTF("    Message buffer %d used for Rx.\r\n", RX_MESSAGE_BUFFER_NUM);
    PRINTF("    Message buffer %d used for Tx.\r\n", TX_MESSAGE_BUFFER_NUM);
    PRINTF("    Interrupt Mode: Enabled\r\n");
    PRINTF("    Operation Mode: TX and RX --> Normal\r\n");
    PRINTF("*********************************************\r\n\r\n");
    
    /*����һ��CAN˫��ͨ��ʵ�飬�������к󲻶�ִ�з��ͣ��յ����ݺ��յ������ݷ���*/
    
    txIdentifier = 0x3;
    rxIdentifier = 0x123;

//    do
//    {
//        PRINTF("Please select local node as A or B:\r\n");
//        PRINTF("Note: Node B should start first.\r\n");
//        PRINTF("Node:");
//        node_type = GETCHAR();
//        PRINTF("%c", node_type);
//        PRINTF("\r\n");
//    } while ((node_type != 'A') && (node_type != 'B') && (node_type != 'a') && (node_type != 'b'));

    /* Select mailbox ID. */
    if ((node_type == 'A') || (node_type == 'a'))
    {
        txIdentifier = 0x321;
        rxIdentifier = 0x123;
    }
    else
    {
        txIdentifier = 0x123;
        rxIdentifier = 0x321;
    }


     */
    FLEXCAN_GetDefaultConfig(&flexcanConfig);
    /* Init FlexCAN module. */

    CAN_Config();
    
    
    
    /* ����CAN���ƾ���������ûص�����Create FlexCAN handle structure and set call back function. */
    FLEXCAN_TransferCreateHandle(EXAMPLE_CAN, &flexcanHandle, flexcan_callback, NULL);

    /* ���ý��չ�����   Set Rx Masking mechanism. */
    FLEXCAN_SetRxMbGlobalMask(EXAMPLE_CAN, FLEXCAN_RX_MB_STD_MASK(rxIdentifier, 0, 0));

    /* ���ý�����Ϣ��������MB��Setup Rx Message Buffer. */
    CAN_RX_Buffer_Config(rxIdentifier,RX_MESSAGE_BUFFER_NUM);//��ʼ�����ջ�����


    /* Setup Tx Message Buffer. */
    FLEXCAN_SetTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, true);


    if ((node_type == 'A') || (node_type == 'a'))
    {
        PRINTF("Press any key to trigger one-shot transmission\r\n\r\n");
        frame.dataByte0 = 0;
    }
    else
    {
        PRINTF("Start to Wait data from Node A\r\n\r\n");
    }

    while (1)
    {
        if ((node_type == 'A') || (node_type == 'a'))
        {
            GETCHAR();

            /*���÷���*/
            frame.id = FLEXCAN_ID_STD(txIdentifier);
            frame.format = kFLEXCAN_FrameFormatStandard;
            frame.type = kFLEXCAN_FrameTypeData;
            frame.length = DLC;

            /*����ȫ�ֵ� MB */
            txXfer.mbIdx = TX_MESSAGE_BUFFER_NUM;
            txXfer.frame = &frame;
            FLEXCAN_TransferSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);


            while (!txComplete)
            {
            };
            txComplete = false;

            /* Start receive data through Rx Message Buffer. */
            
            rxXfer.mbIdx = RX_MESSAGE_BUFFER_NUM;
            rxXfer.frame = &frame;
            FLEXCAN_TransferReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxXfer);


            /* Wait until Rx MB full. */
            while (!rxComplete)
            {
            };
            rxComplete = false;

            PRINTF("Rx MB ID: 0x%3x, Rx MB data: 0x%x, Time stamp: %d\r\n", frame.id >> CAN_ID_STD_SHIFT,
                   frame.dataByte0, frame.timestamp);
            PRINTF("Press any key to trigger the next transmission!\r\n\r\n");
            frame.dataByte0++;
            frame.dataByte1 = 0x55;
        }
        else
        {
            /* Before this , should first make node B enter STOP mode after FlexCAN
             * initialized with enableSelfWakeup=true and Rx MB configured, then A
             * sends frame N which wakes up node B. A will continue to send frame N
             * since no acknowledgement, then B received the second frame N(In the
             * application it seems that B received the frame that woke it up which
             * is not expected as stated in the reference manual, but actually the
             * output in the terminal B received is the same second frame N). */
            if (wakenUp)
            {
                PRINTF("B has been waken up!\r\n\r\n");
            }

            /* Start receive data through Rx Message Buffer. */
            rxXfer.mbIdx = RX_MESSAGE_BUFFER_NUM;
#if (defined(USE_CANFD) && USE_CANFD)
            rxXfer.framefd = &frame;
            FLEXCAN_TransferFDReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxXfer);
#else
            rxXfer.frame = &frame;
            FLEXCAN_TransferReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxXfer);
#endif

            /* Wait until Rx receive full. */
            while (!rxComplete)
            {
            };
            rxComplete = false;

            PRINTF("Rx MB ID: 0x%3x, Rx MB data: 0x%x, Time stamp: %d\r\n", frame.id >> CAN_ID_STD_SHIFT,
                   frame.dataByte0, frame.timestamp);

            frame.id = FLEXCAN_ID_STD(txIdentifier);
            txXfer.mbIdx = TX_MESSAGE_BUFFER_NUM;
#if (defined(USE_CANFD) && USE_CANFD)
            frame.brs = 1;
            txXfer.framefd = &frame;
            FLEXCAN_TransferFDSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
#else
            txXfer.frame = &frame;
            FLEXCAN_TransferSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
#endif

            while (!txComplete)
            {
            };
            txComplete = false;
            PRINTF("Wait Node A to trigger the next transmission!\r\n\r\n");
        }
    }
}



























///*ȫ�ֱ���������CAN����״̬��false��û�н��յ���Ϣ��true:���յ���Ϣ���һ�δ������*/
//volatile bool rxComplete = false;
//
///*ȫ�ֱ�����������պͷ�����Ϣ�ṹ��*/
//flexcan_frame_t txFrame, rxFrame;
//
//
///*******************************************************************
//* Prototypes
//*******************************************************************/
///**
//* @brief ��ʱһ��ʱ��
//*/
//void delay(uint32_t count);
//
///*******************************************************************
//* Code
//*******************************************************************/
///**
//* @note �������ڲ�ͬ���Ż�ģʽ����ʱʱ�䲻ͬ��
//*       ��flexspi_nor_debug��flexspi_nor_release�汾�ĳ����У�
//*       flexspi_nor_release�汾����ʱҪ�̵ö�  
//*/ 
//void delay(uint32_t count)
//{
//  volatile uint32_t i = 0;
//  for (i = 0; i < count; ++i)
//  {
//    __asm("NOP"); /* ����nop��ָ�� */
//  }
//}
//
///**
//* @brief  ������
//* @param  ��
//* @retval ��
//*/
//int main(void)
//{
//
//  /* ��ʼ���ڴ汣����Ԫ */
//  BOARD_ConfigMPU();
//  /* ��ʼ������������ */
//  BOARD_InitPins();
//  /* ��ʼ��������ʱ�� */
//  BOARD_BootClockRUN();
//  /* ��ʼ�����Դ��� */
//  BOARD_InitDebugConsole();
//  
//  /*�����ж����ȼ�����*/
//  Set_NVIC_PriorityGroup(Group_4); 
//  
//  /* ��ӡϵͳʱ�� */
//  PRINTF("\r\n");
//  PRINTF("*****��ӭʹ�� Ұ��i.MX RT1052 ������*****\r\n");
//  PRINTF("CPU:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
//  PRINTF("AHB:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AhbClk));
//  PRINTF("SEMC:            %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
//  PRINTF("SYSPLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
//  PRINTF("SYSPLLPFD0:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk));
//  PRINTF("SYSPLLPFD1:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd1Clk));
//  PRINTF("SYSPLLPFD2:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
//  PRINTF("SYSPLLPFD3:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd3Clk));  
//  
//  /* ��ʼ��LED���� */
//  LED_GPIO_Config(); 
//  
//  PRINTF("\r\n==FlexCAN �ػ��������� -- Start.==\r\n\r\n");
//  CAN_Config();
//  
//  while(1)
//  {
//    CAN_RX_Buffer_Config(0x123,RX_MESSAGE_BUFFER_NUM);//��ʼ�����ջ�����
//    
//    FLEXCAN_SetTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, true);//���÷�����Ϣ������
//    CAN_TX_Buffer_Config(0x123);//��ʼ���������ݸ�ʽ�Լ�����
//    
//    PRINTF("�ӷ��ͻ����� MB%d ����ջ����� MB%d ������Ϣ\r\n", TX_MESSAGE_BUFFER_NUM, RX_MESSAGE_BUFFER_NUM);
//
//    /*������ͻ�����������*/
//    PRINTF("tx word0 = 0x%x\r\n", txFrame.dataWord0);
//    PRINTF("tx word1 = 0x%x\r\n", txFrame.dataWord1);
//
//    /*ִ�з���*/
//    FLEXCAN_TransferSendBlocking(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, &txFrame);
//
//    /* �ȴ�������� */
//    while (!rxComplete)
//    {
//    }
//
//    PRINTF("\r\n���ջ�����MB%d ������\r\n", RX_MESSAGE_BUFFER_NUM);
//    
//    /*������յ�������*/
//    PRINTF("rx word0 = 0x%x\r\n", rxFrame.dataWord0);
//    PRINTF("rx word1 = 0x%x\r\n", rxFrame.dataWord1);
//
//    /* ֹͣ FlexCAN ���պͷ���. */
//    FLEXCAN_DisableMbInterrupts(EXAMPLE_CAN, 1 << RX_MESSAGE_BUFFER_NUM);
//
//    PRINTF("\r\n==FlexCAN �ػ�ģʽ���Խ���==\r\n");
//    while (1)
//    {
//      ;
//    }
//  }
//  
//}



/****************************END OF FILE**********************/

