#ifndef __USART_H
#define __USART_H

#include "fsl_common.h"

/*********************************************************
 * UART7 GPIO�˿ڡ����źż�IOMUXC���ú궨��
 
ʹ��i.MX RT1052-Pro�װ�
 *  TX-----GPIO_SD_B1_08,P3-----(CN5, 55)
 *  RX-----GGPIO_SD_B1_09,N4-----(CN5, 57)
 *���Ź���         ���ű��     �����ڿ������϶�Ӧλ��
 *TX���Ŷ�Ӧ������CN5����ĵ�55��(CN5, 55)
 *RX���Ŷ�Ӧ������CN5����ĵ�57��(CN5, 57)

ʹ��i.MX RT1052-Mini�װ�
 *  TX-----GPIO_SD_B1_08,P3-----(CN4, 55)
 *  RX-----GGPIO_SD_B1_09,N4-----(CN4, 57)
 *���Ź���         ���ű��     �����ڿ������϶�Ӧλ��
 *TX���Ŷ�Ӧ������CN4����ĵ�55��(CN4, 55)
 *RX���Ŷ�Ӧ������CN4����ĵ�57��(CN4, 57)
 *********************************************************/
#define UART_RX_GPIO                GPIO3
#define UART_RX_GPIO_PIN            (9U)
#define UART_RX_IOMUXC              IOMUXC_GPIO_SD_B1_09_LPUART7_RX

#define UART_TX_GPIO                GPIO3
#define UART_TX_GPIO_PIN            (8U)
#define UART_TX_IOMUXC              IOMUXC_GPIO_SD_B1_08_LPUART7_TX

/*******************************************************************************
 * UART7 ���ںš��жϷ��������жϺ��ض���
 ******************************************************************************/

#define DEBUG_USARTx LPUART7
#define DEBUG_USART_IRQ LPUART7_IRQn
#define DEBUG_USART_IRQHandler LPUART7_IRQHandler


/*******************************************************************************
* UART7 �������ò������壬Ĭ�ϲ�������:
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
 * uart7��������
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
 void USART_Config(void);
 void USART_ModeConfig(void);
 void USART_IOMUXC_MUX_Config(void);
 void USART_IOMUXC_PAD_Config(void);
 
 void Usart_SendByte(LPUART_Type *base, uint8_t data);
 void Usart_SendString( LPUART_Type *base, const char *str);
 void Usart_SendHalfWord(LPUART_Type *base, uint16_t ch);




#endif /* __USART_H */


