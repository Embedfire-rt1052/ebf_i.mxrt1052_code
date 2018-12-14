#ifndef __USART_H
#define __USART_H

#include "fsl_common.h"

/*********************************************************
 * UART3 GPIO�˿ڡ����źż�IOMUXC���ú궨��
 
ʹ��i.MX RT1052-Pro�װ壺
 *  TX-----GPIO_AD_B1_06,J12-----(CN5, 26)
 *  RX-----GPIO_AD_B1_07,K10-----(CN5, 28)
 *���Ź���         ���ű��     �����ڿ������϶�Ӧλ��
 *TX���Ŷ�Ӧ������CN5����ĵ�26��(CN5, 26)
 *RX���Ŷ�Ӧ������CN5����ĵ�28��(CN5, 28)

ʹ��i.MX RT1052-Mini�װ�
 *  TX-----GPIO_AD_B1_06,J12-----(CN4, 26)
 *  RX-----GPIO_AD_B1_07,K10-----(CN4, 28)
 *���Ź���         ���ű��     �����ڿ������϶�Ӧλ��
 *TX���Ŷ�Ӧ������CN4����ĵ�26��(CN4, 26)
 *RX���Ŷ�Ӧ������CN4����ĵ�28��(CN4, 28) 
 *********************************************************/
#define UART_RX_GPIO                GPIO1
#define UART_RX_GPIO_PIN            (23U)
#define UART_RX_IOMUXC              IOMUXC_GPIO_AD_B1_07_LPUART3_RX

#define UART_TX_GPIO                GPIO1
#define UART_TX_GPIO_PIN            (22U)
#define UART_TX_IOMUXC              IOMUXC_GPIO_AD_B1_06_LPUART3_TX

/*******************************************************************************
 * UART3 ���ںš��жϷ��������жϺ��ض���
 ******************************************************************************/

#define DEBUG_USARTx LPUART3
#define DEBUG_USART_IRQ LPUART3_IRQn
#define DEBUG_USART_IRQHandler LPUART3_IRQHandler


/*******************************************************************************
* UART3 �������ò������壬Ĭ�ϲ�������:
 * .baudRate_Bps = 115200U;
 * .parityMode = kLPUART_ParityDisabled;
 * .stopBitCount = kLPUART_OneStopBit;
 * .txFifoWatermark = 0;
 * .rxFifoWatermark = 0;
 * .enableTx = false;
 * .enableRx = false;
 ******************************************************************************/
#define  DEBUG_USART_BAUDRATE           115200U
#define  DEBUG_USART_STOP_BIT           kLPUART_OneStopBit
#define  DEBUG_USART_ENABLE_SEND        true
#define  DEBUG_USART_ENABLE_RESIVE      true

/*******************************************************************************
 * uart3��������
 ******************************************************************************/
#define USART_RX_PAD_CONFIG_DATA            (SRE_0_SLOW_SLEW_RATE| \
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

#define USART_TX_PAD_CONFIG_DATA            (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_1_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_0_KEEPER_SELECTED| \
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

/*******************************************************************************
 * ��������
 ******************************************************************************/
 void USART_Config(void);
 void USART_ModeConfig(void);
 void USART_IOMUXC_MUX_Config(void);
 void USART_IOMUXC_PAD_Config(void);
 
 void Usart_SendByte(LPUART_Type *base, uint8_t data);
 void Usart_SendString( LPUART_Type *base, const char *str);
 void Usart_SendHalfWord(LPUART_Type *base, uint16_t ch);




#endif /* __USART_H */


