#ifndef __BSP_DMA_UART_H
#define __BSP_DMA_UART_H

#include "fsl_dmamux.h"
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "pad_config.h"
#include "fsl_lpuart.h"
#include "board.h"
#include "fsl_lpuart_edma.h"



/*********************************************************
 * UART1 GPIO�˿ڡ����źż�IOMUXC���ú궨��
 
ʹ��i.MX RT1052-Pro�װ壺
 *  TX-----GPIO_AD_B0_12,K14-----(CN5, 9)
 *  RX-----GPIO_AD_B0_13,L14-----(CN5, 11)
 *���Ź���         ���ű��     �����ڿ������϶�Ӧλ��
 *TX���Ŷ�Ӧ������CN5����ĵ�9��(CN5, 9)
 *RX���Ŷ�Ӧ������CN5����ĵ�11��(CN5, 11)
 
ʹ��i.MX RT1052-Mini�װ壺
 *  TX-----GPIO_AD_B0_12,K14-----(CN4, 9)
 *  RX-----GPIO_AD_B0_13,L14-----(CN4, 11)
 *���Ź���         ���ű��     �����ڿ������϶�Ӧλ��
 *TX���Ŷ�Ӧ������CN4����ĵ�9��(CN4, 9)
 *RX���Ŷ�Ӧ������CN4����ĵ�11��(CN4, 11)
 *********************************************************/
#define UART_RX_GPIO                GPIO1
#define UART_RX_GPIO_PIN            (13U)
#define UART_RX_IOMUXC              IOMUXC_GPIO_AD_B0_13_LPUART1_RX

#define UART_TX_GPIO                GPIO1
#define UART_TX_GPIO_PIN            (12U)
#define UART_TX_IOMUXC              IOMUXC_GPIO_AD_B0_12_LPUART1_TX


/*******************************************************************************
 * uart��������
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
   
   
   

/*�궨��*/
/*���屾����ʹ�õĴ��ڣ�
*ע��:LPUART1��ϵͳ���Դ��ڣ���ϵͳ��ʼ��ʱ�Ѿ���������GPIO�ĳ�ʼ�������ʹ������
*     ���ڣ�ʹ��֮ǰһ��Ҫ��ʼ����Ӧ���š�
*/
#define DEMO_LPUART LPUART1               
#define DEMO_LPUART_CLK_FREQ BOARD_DebugConsoleSrcFreq() //UARTʱ��Ƶ��
#define LPUART_TX_DMA_CHANNEL 0U                         //UART����ʹ�õ�DMAͨ����
#define LPUART_RX_DMA_CHANNEL 1U                         //UART����ʹ�õ�DMAͨ����
#define LPUART_TX_DMA_REQUEST kDmaRequestMuxLPUART1Tx    //���崮��DMA��������Դ
#define LPUART_RX_DMA_REQUEST kDmaRequestMuxLPUART1Rx    //���崮��DMA��������Դ
#define EXAMPLE_LPUART_DMAMUX_BASEADDR DMAMUX            //������ʹ�õ�DMA��·����ģ��(DMAMUX)
#define EXAMPLE_LPUART_DMA_BASEADDR DMA0                 //����ʹ�õ�DAM
#define ECHO_BUFFER_LENGTH 8                             //UART���պͷ������ݻ���������


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void UART_GPIO_Init(void);
void UART_Init(void);
void UART_DMA_Init(void);
/* LPUART �ص����� */
void LPUART_UserCallback(LPUART_Type *base, lpuart_edma_handle_t *handle, status_t status, void *userData);

#endif /* __BSP_DMA_UART_H */
