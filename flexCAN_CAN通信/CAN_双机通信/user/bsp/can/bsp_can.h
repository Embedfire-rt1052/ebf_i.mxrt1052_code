#ifndef __CAN_H
#define	__CAN_H

#include "fsl_common.h"
#include "fsl_flexcan.h"







/*CAN��غ궨��*/
#define EXAMPLE_CAN CAN2                 //����ʹ�õ�CAN
#define EXAMPLE_FLEXCAN_IRQn CAN2_IRQn   //�����жϺ�
#define EXAMPLE_FLEXCAN_IRQHandler CAN2_IRQHandler//�����жϷ�����


/*����������ض���*/
#define RX_MESSAGE_BUFFER_NUM (9)     //����ʹ�õĽ�������
#define TX_MESSAGE_BUFFER_NUM (8)    //����ʹ�õķ�������

#define DLC (8)                       //�������ݳ���

///*������ջ�����ID�뷢�ͻ�����ID,
//*����CAN�ػ�����ʱ��Ҫ������ID�뷢��ID��ͬ
//*/
//#define CAN_RX_ID 0x123
//#define CAN_TX_ID 0x123
//
//
/*ʱ����غ궨��*/
#define FLEXCAN_CLOCK_SOURCE_SELECT (2U)//ѡ��ʱ��Դ��PLL3(480 MHz)����6��Ƶ��(80MHz)��ΪCAN��ʱ�ӡ�
#define FLEXCAN_CLOCK_SOURCE_DIVIDER (3U)//����ʱ�ӷ�Ƶ��80MHz��CAN��ʱ�Ӿ�����Ƶ����ΪCANʱ��Դ��
/* ��ȡCAN�ǹ���Ƶ�� */
#define EXAMPLE_CAN_CLK_FREQ ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 6) / (FLEXCAN_CLOCK_SOURCE_DIVIDER + 1U))




/* To get most precise baud rate under some circumstances, users need to set
   quantum which is composed of PSEG1/PSEG2/PROPSEG. Because CAN clock prescaler
   = source clock/(baud rate * quantum), for e.g. 84M clock and 1M baud rate, the
   quantum should be .e.g 14=(6+3+1)+4, so prescaler is 6. By default, quantum
   is set to 10=(3+2+1)+4, because for most platforms e.g. 120M source clock/(1M
   baud rate * 10) is an integer. Remember users must ensure the calculated
   prescaler an integer thus to get precise baud rate. */
/*
*�����ʼ��㡣
*PLL3ʱ��480MHz���̶���6��Ƶ֮����ΪCAN��ʱ��(80MHZ)
*�ڳ���������ʱ�ӷ�ƵΪ3������CAN����Ƶ��Ϊ80/(3+1)=20MHz
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







