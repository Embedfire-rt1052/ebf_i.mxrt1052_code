/**
******************************************************************
* @file    main.c
* @author  fire
* @version V1.0
* @date    2018-xx-xx
* @brief   串口中断接收测试
******************************************************************
* @attention
*
* 实验平台:野火  i.MXRT1052开发板 
* 论坛    :http://www.firebbs.cn
* 淘宝    :http://firestm32.taobao.com
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

   

flexcan_handle_t flexcanHandle;    //定义flexcan句柄
volatile bool txComplete = false; //发送完成标志
volatile bool rxComplete = false; //接收完成标志
volatile bool wakenUp = false;    //唤醒标志    

flexcan_mb_transfer_t txXfer;  //定义发送MB(Message Buffer)
flexcan_mb_transfer_t rxXfer;  //定义接收MB(Message Buffer)

flexcan_frame_t  rx_frame;//定义接收消息框架结构体
flexcan_frame_t  tx_frame;//定义发送消息框架结构体


uint32_t txIdentifier;//定义发送标识符
uint32_t rxIdentifier;//定义接收标识符




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
    flexcan_config_t flexcanConfig;//定义FlexCAN初始化变量，用于初始化FlexCAN
    uint8_t node_type;             //用于保存使用GETCHAR()得到的字符

    /* 系统初始化*/
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /*设置FlexCAN的时钟频率*/
    CLOCK_SetMux(kCLOCK_CanMux, FLEXCAN_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_CanDiv, FLEXCAN_CLOCK_SOURCE_DIVIDER);
    
    /*输出提示信息*/
    PRINTF("********* FLEXCAN Interrupt EXAMPLE *********\r\n");
    PRINTF("    Message format: Standard (11 bit id)\r\n");
    PRINTF("    Message buffer %d used for Rx.\r\n", RX_MESSAGE_BUFFER_NUM);
    PRINTF("    Message buffer %d used for Tx.\r\n", TX_MESSAGE_BUFFER_NUM);
    PRINTF("    Interrupt Mode: Enabled\r\n");
    PRINTF("    Operation Mode: TX and RX --> Normal\r\n");
    PRINTF("*********************************************\r\n\r\n");
    
    
   /*这是一个CAN双机通信实验
    *将接收标识符与发送标识符设置为相同值，则无需改动程序即可将
    *程序下载到两个开发板上即可观察到实验现象。
    */
    txIdentifier = 0x123;
    rxIdentifier = 0x123;


    
    /*获取默认配置*/
    FLEXCAN_GetDefaultConfig(&flexcanConfig);
    /* 初始化FlexCAN*/
    CAN_Config();
    
    
    /* 创建CAN控制句柄，并设置回调函数*/
    FLEXCAN_TransferCreateHandle(EXAMPLE_CAN, &flexcanHandle, flexcan_callback, NULL);

    /*设置接收过滤器*/
    FLEXCAN_SetRxMbGlobalMask(EXAMPLE_CAN, FLEXCAN_RX_MB_STD_MASK(rxIdentifier, 0, 0));

    
    /* 设置接收消息缓冲区（MB）Setup Rx Message Buffer. */
    CAN_RX_Buffer_Config(rxIdentifier,RX_MESSAGE_BUFFER_NUM);//初始化接收缓冲区
    /* 设置发送消息缓冲区. */
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
          
          
          /*设置全局的 MB */
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

          /*等待接收内容*/
          rxXfer.mbIdx = RX_MESSAGE_BUFFER_NUM;
          rxXfer.frame = &rx_frame;
          
          while(1)
          {
            /*如果当前接收空闲或者接收未完成*/
            if(!rxComplete)
            {
              /*请求接收*/
              FLEXCAN_TransferReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxXfer);
            }    
            
            /*接收完成，输出接收到的数据*/
            if(rxComplete)
            {
              PRINTF("resive success\r\n");
              /*输出*/
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
              rxComplete = 0;//设置接收完成标志为0（表示没有收到数据）
            }
          }

        }
    }
}

/****************************END OF FILE**********************/






