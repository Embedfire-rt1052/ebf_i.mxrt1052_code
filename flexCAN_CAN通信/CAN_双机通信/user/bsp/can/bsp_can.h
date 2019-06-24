#ifndef __CAN_H
#define	__CAN_H

#include "fsl_iomuxc.h"
#include "pad_config.h"
#include "fsl_gpio.h" 
#include "fsl_common.h"
#include "fsl_flexcan.h"


/*********************************************************
 * FLEXCAN2 GPIO�˿ڡ����źż�IOMUXC���ú궨��
 
ʹ��i.MX RT1052-Pro�װ壺
 *  TX-----GPIO_AD_B0_14-----(CN5,17)
 *  RX-----GPIO_AD_B0_15-----(CN5, 19)
 *���Ź���         ���ű��     �����ڿ������϶�Ӧλ��
 *TX���Ŷ�Ӧ������CN5����ĵ�17��(CN5, 17)
 *RX���Ŷ�Ӧ������CN5����ĵ�19��(CN5, 19)
 
ʹ��i.MX RT1052-Mini�װ壺
 *  TX-----GPIO_AD_B0_14-----(CN4, 17)
 *  RX-----GPIO_AD_B0_15-----(CN4, 19)
 *���Ź���         ���ű��     �����ڿ������϶�Ӧλ��
 *TX���Ŷ�Ӧ������CN4����ĵ�17��(CN4, 17)
 *RX���Ŷ�Ӧ������CN4����ĵ�19��(CN4, 19)
 *********************************************************/
#define FLEXCAN2_TX_GPIO                GPIO1
#define FLEXCAN2_TX_GPIO_PIN            (14U)
#define FLEXCAN2_TX_IOMUXC              IOMUXC_GPIO_AD_B0_14_FLEXCAN2_TX

#define FLEXCAN2_RX_GPIO                GPIO1
#define FLEXCAN2_RX_GPIO_PIN            (15U)
#define FLEXCAN2_RX_IOMUXC              IOMUXC_GPIO_AD_B0_15_FLEXCAN2_RX




/*CAN��غ궨��*/
#define EXAMPLE_CAN CAN2                 //����ʹ�õ�CAN
#define EXAMPLE_FLEXCAN_IRQn CAN2_IRQn   //�����жϺ�
#define EXAMPLE_FLEXCAN_IRQHandler CAN2_IRQHandler//�����жϷ�����


/*����������ض���*/
#define RX_MESSAGE_BUFFER_NUM (9)    //����ʹ�õĽ�������
#define TX_MESSAGE_BUFFER_NUM (8)    //����ʹ�õķ�������

#define DLC (8)                      //�������ݳ���


/*ʱ����غ궨��*/
#define FLEXCAN_CLOCK_SOURCE_SELECT (2U) //ѡ��ʱ��Դ��PLL3(480 MHz)����6��Ƶ��(80MHz)��ΪCAN��ʱ�ӡ�
#define FLEXCAN_CLOCK_SOURCE_DIVIDER (3U)//����ʱ�ӷ�Ƶ��80MHz��CAN��ʱ�Ӿ�����Ƶ����ΪCANʱ��Դ��
/* ��ȡCAN�ǹ���Ƶ�� */
#define EXAMPLE_CAN_CLK_FREQ ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 6) / (FLEXCAN_CLOCK_SOURCE_DIVIDER + 1U))





/*******************************************************************************
 * FLEXCAN2��������
 ******************************************************************************/
#define FLEXCAN2_PAD_CONFIG_DATA        (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_1_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_1_PULL_SELECTED| \
                                        PUS_3_22K_OHM_PULL_UP| \
                                        HYS_0_HYSTERESIS_DISABLED) 



void CAN_Mode_Config(uint32_t baudRate, bool LoopBack);
void CAN_Config(void);
void CAN_RX_Buffer_Config(uint32_t ID_STD,uint8_t RX_MB);


#endif







