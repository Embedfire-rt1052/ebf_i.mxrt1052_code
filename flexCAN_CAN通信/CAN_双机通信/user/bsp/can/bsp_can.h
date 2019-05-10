#ifndef __CAN_H
#define	__CAN_H

#include "fsl_common.h"
#include "fsl_flexcan.h"







/*CAN相关宏定义*/
#define EXAMPLE_CAN CAN2                 //定义使用的CAN
#define EXAMPLE_FLEXCAN_IRQn CAN2_IRQn   //定义中断号
#define EXAMPLE_FLEXCAN_IRQHandler CAN2_IRQHandler//定义中断服务函数


/*发送邮箱相关定义*/
#define RX_MESSAGE_BUFFER_NUM (9)     //定义使用的接收邮箱
#define TX_MESSAGE_BUFFER_NUM (8)    //定义使用的发送邮箱

#define DLC (8)                       //定义数据长度

///*定义接收缓冲区ID与发送缓冲区ID,
//*进行CAN回环测试时需要将接收ID与发送ID相同
//*/
//#define CAN_RX_ID 0x123
//#define CAN_TX_ID 0x123
//
//
/*时钟相关宏定义*/
#define FLEXCAN_CLOCK_SOURCE_SELECT (2U)//选择时钟源，PLL3(480 MHz)经过6分频后(80MHz)作为CAN根时钟。
#define FLEXCAN_CLOCK_SOURCE_DIVIDER (3U)//设置时钟分频，80MHz的CAN根时钟经过分频后作为CAN时钟源。
/* 读取CAN是工作频率 */
#define EXAMPLE_CAN_CLK_FREQ ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 6) / (FLEXCAN_CLOCK_SOURCE_DIVIDER + 1U))




/* To get most precise baud rate under some circumstances, users need to set
   quantum which is composed of PSEG1/PSEG2/PROPSEG. Because CAN clock prescaler
   = source clock/(baud rate * quantum), for e.g. 84M clock and 1M baud rate, the
   quantum should be .e.g 14=(6+3+1)+4, so prescaler is 6. By default, quantum
   is set to 10=(3+2+1)+4, because for most platforms e.g. 120M source clock/(1M
   baud rate * 10) is an integer. Remember users must ensure the calculated
   prescaler an integer thus to get precise baud rate. */
/*
*波特率计算。
*PLL3时钟480MHz经固定的6分频之后最为CAN根时钟(80MHZ)
*在程序中设置时钟分频为3，最终CAN工作频率为80/(3+1)=20MHz
*
*/
#define SET_CAN_QUANTUM 0
#define PSEG1 3
#define PSEG2 2
#define PROPSEG 1



//
//
//
////static void CAN_GPIO_Config(void);
//////static void CAN_NVIC_Config(void);
////static void CAN_Mode_Config(void);
//
void CAN_Mode_Config(uint32_t baudRate, bool LoopBack);

void CAN_Config(void);
void CAN_RX_Buffer_Config(uint32_t ID_STD,uint8_t RX_MB);
void CAN_TX_Buffer_Config(uint32_t ID_STD,flexcan_frame_t* _txFrame );
//void CAN_TX_Buffer_Config(uint32_t ID_STD);

                                                                              


#endif







