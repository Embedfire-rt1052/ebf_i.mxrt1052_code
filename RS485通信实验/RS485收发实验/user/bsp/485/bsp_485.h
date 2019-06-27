#ifndef __UART_H
#define __UART_H

#include "fsl_common.h"

/*********************************************************
 * UART5 GPIO�˿��Լ�485���ա����Ϳ��ƶ˿ڡ����źż�IOMUXC���ú궨��
 
ʹ��i.MX RT1052-Pro�װ壺
 *  TX-----GPIO_B1_12,D13-----(CN5, 13)
 *  RX-----GPIO_B1_13,D14-----(CN5, 15)
 *  RE-----GPIO_AD_B1_10,L13--(CN5, 31)
 *���Ź���         ���ű��     �����ڿ������϶�Ӧλ��
 *TX���Ŷ�Ӧ������CN5����ĵ�13��(CN5, 13)
 *RX���Ŷ�Ӧ������CN5����ĵ�15��(CN5, 15)
 
ʹ��i.MX RT1052-Mini�װ壺
 *  TX-----GPIO_B1_12,D13-----(CN4, 13)
 *  RX-----GPIO_B1_13,D14-----(CN4, 15)
 *  RE-----GPIO_AD_B1_10,L13--(CN4, 31)
 *���Ź���         ���ű��     �����ڿ������϶�Ӧλ��
 *TX���Ŷ�Ӧ������CN4����ĵ�13��(CN4, 13)
 *RX���Ŷ�Ӧ������CN4����ĵ�15��(CN4, 15)
 *********************************************************/

/*485 ���͡����տ������Ŷ���
*Ϊ�ߵ�ƽ������ʹ��
*Ϊ�͵�ƽ������ʹ��
*/
#define _485_RE_GPIO             GPIO1
#define _485_RE_GPIO_PIN         (26U)
#define _485_RE_IOMUXC           IOMUXC_GPIO_AD_B1_10_GPIO1_IO26

#define digital_H(p,i)      {p->DR |= (1U << i);}   //���Ϊ�ߵ�ƽ    
#define digital_L(p,i)      {p->DR &= ~(1U << i);}  //����͵�ƽ

/*����485Ϊ���ջ���*/
#define _485_Resive           digital_L(_485_RE_GPIO,_485_RE_GPIO_PIN)
#define _485_Send             digital_H(_485_RE_GPIO,_485_RE_GPIO_PIN) 


/*TX����*/
#define UART_RX_GPIO                GPIO2
#define UART_RX_GPIO_PIN            (29U)
#define UART_RX_IOMUXC              IOMUXC_GPIO_B1_13_LPUART5_RX

/*RX ����*/
#define UART_TX_GPIO                GPIO2
#define UART_TX_GPIO_PIN            (28U)
#define UART_TX_IOMUXC              IOMUXC_GPIO_B1_12_LPUART5_TX





/*******************************************************************************
 * UART5 ���ںš��жϷ��������жϺ��ض���
 ******************************************************************************/

#define DEBUG_UARTx LPUART5
#define DEBUG_UART_IRQ LPUART5_IRQn
#define DEBUG_UART_IRQHandler LPUART5_IRQHandler


/*******************************************************************************
* UART5 �������ò������壬Ĭ�ϲ�������:
 * .baudRate_Bps = 115200U;
 * .parityMode = kLPUART_ParityDisabled;
 * .stopBitCount = kLPUART_OneStopBit;
 * .txFifoWatermark = 0;
 * .rxFifoWatermark = 0;
 * .enableTx = false;
 * .enableRx = false;
 ******************************************************************************/
#define  DEBUG_UART_BAUDRATE           115200U
#define  DEBUG_UART_STOP_BIT           kLPUART_OneStopBit
#define  DEBUG_UART_ENABLE_SEND        true
#define  DEBUG_UART_ENABLE_RESIVE      true

/*******************************************************************************
 * uart5��������
 ******************************************************************************/
#define UART_RX_PAD_CONFIG_DATA            (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_1_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_1_PULL_SELECTED| \
                                        PUS_3_22K_OHM_PULL_UP| \
                                        HYS_0_HYSTERESIS_DISABLED) 
    /* ����˵�� : */
    /* ת������: ת��������
        ����ǿ��: R0/6 
        �������� : medium(100MHz)
        ��©����: �ر� 
        ��/����������: ʹ��
        ��/������ѡ��: ������
        ����/����ѡ��: 22Kŷķ����(ѡ���˱�������������Ч)
        �ͻ�������: ��ֹ */ 

#define UART_TX_PAD_CONFIG_DATA            (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_1_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_1_PULL_SELECTED| \
                                        PUS_3_22K_OHM_PULL_UP| \
                                        HYS_0_HYSTERESIS_DISABLED)
    /* ����˵�� : */
    /* ת������: ת��������
        ����ǿ��: R0/6 
        �������� : medium(100MHz)
        ��©����: �ر� 
        ��/����������: ʹ��
        ��/������ѡ��: ������
        ����/����ѡ��: 22Kŷķ����(ѡ���˱�������������Ч)
        �ͻ�������: ��ֹ */ 


/* 485оƬ���ա����Ϳ�������*/
#define RE_485_PAD_CONFIG_DATA          (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_0_PULL_KEEPER_DISABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_0_100K_OHM_PULL_DOWN| \
                                        HYS_0_HYSTERESIS_DISABLED)  

/*******************************************************************************
 * ��������
 ******************************************************************************/
 void UART_Config(void);
 void UART_ModeConfig(void);
 void UART_IOMUXC_MUX_Config(void);
 void UART_IOMUXC_PAD_Config(void);
 
 void Uart_SendByte(LPUART_Type *base, uint8_t data);
 void Uart_SendString( LPUART_Type *base, const char *str);
 void Uart_SendHalfWord(LPUART_Type *base, uint16_t ch);

 /*485ͨ����������*/
 static void _485_delay(__IO uint32_t nCount);
 void _485_SendByte(LPUART_Type *base, uint8_t data);
 void _485_Control_GPIO_init(void);


#endif /* __UART_H */


 
 