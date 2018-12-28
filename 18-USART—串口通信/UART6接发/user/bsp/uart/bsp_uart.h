#ifndef __USART_H
#define __USART_H

#include "fsl_common.h"

/*********************************************************
 * UART6 GPIO�˿ڡ����źż�IOMUXC���ú궨��
 
ʹ��i.MX RT1052-Pro�װ壺
 *  TX-----GPIO_AD_B0_02,M11-----(CN5, 14)
 *  RX-----GPIO_AD_B0_03,G11-----(CN5, 16)
 *���Ź���         ���ű��     �����ڿ������϶�Ӧλ��
 *TX���Ŷ�Ӧ������CN5����ĵ�14��(CN5, 14)
 *RX���Ŷ�Ӧ������CN5����ĵ�16��(CN5, 16)
 *ע�⣺����6�����������ͬʱʹ�ã�ʹ�ô���6ʱҪ����J37����ñ
 
ʹ��i.MX RT1052-Mini�װ壺
 * UART6 GPIO�˿ڡ����źż�IOMUXC���ú궨��
 *  TX-----GPIO_AD_B0_02,M11-----(CN4, 14)
 *  RX-----GPIO_AD_B0_03,G11-----(CN4, 16)
 *���Ź���         ���ű��     �����ڿ������϶�Ӧλ��
 *TX���Ŷ�Ӧ������CN4����ĵ�14��(CN4, 14)
 *RX���Ŷ�Ӧ������CN4����ĵ�16��(CN4, 16)
*********************************************************/
#define UART_RX_GPIO                GPIO1
#define UART_RX_GPIO_PIN            (3U)
#define UART_RX_IOMUXC              IOMUXC_GPIO_AD_B0_03_LPUART6_RX

#define UART_TX_GPIO                GPIO1
#define UART_TX_GPIO_PIN            (2U)
#define UART_TX_IOMUXC              IOMUXC_GPIO_AD_B0_02_LPUART6_TX

/*******************************************************************************
 * UART6 ���ںš��жϷ��������жϺ��ض���
 ******************************************************************************/

#define DEBUG_UARTx LPUART6
#define DEBUG_UART_IRQ LPUART6_IRQn
#define DEBUG_UART_IRQHandler LPUART6_IRQHandler


/*******************************************************************************
* UART6 �������ò������壬Ĭ�ϲ�������:
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
 * uart6��������
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




#endif /* __USART_H */


