#ifndef __BSP_SD_H
#define __BSP_SD_H


#include "pad_config.h"
#include "fsl_iomuxc.h"
#include "fsl_common.h"
#include "fsl_sd.h"

/*********************************************************
 * TMR3 ����˿ڡ����źż�IOMUXC���ú궨��
 
ʹ��i.MX RT1052-Pro�װ壺
 *  TMR3_CH0-----GPIO_AD_B1_00-----(CN5, 21)
 *  TMR3_CH1-----GPIO_AD_B1_01-----(CN5, 23)
 *  TMR3_CH2-----GPIO_AD_B1_02-----(CN5, 10)
 *  TMR3_CH3-----GPIO_AD_B1_03-----(CN5, 8)
 *���Ź���         ���ű��     �����ڿ������϶�Ӧλ��
 (CN5, 21) �������������ӵ�������CN5����ĵ�21��


ʹ��i.MX RT1052-Mini�װ�

 *  TMR3_CH0-----GPIO_AD_B1_00-----(CN4, 21)
 *  TMR3_CH1-----GPIO_AD_B1_01-----(CN4, 23)
 *  TMR3_CH2-----GPIO_AD_B1_02-----(CN4, 10)
 *  TMR3_CH3-----GPIO_AD_B1_03-----(CN4, 8)
 *���Ź���         ���ű��     �����ڿ������϶�Ӧλ��
 (CN4, 21) �������������ӵ�������CN4����ĵ�21�� 

ע�⣺TMR��ʱ����һ��ͨ�����Զ�Ӧ��һ�����߶���ⲿ���ţ���ϸ��Ϣ��ο�
      ��IMXRT1050RM����4��External Signals and Pin Multiplexing
 *********************************************************/


/*SD1_CMD*/
#define USDHC1_CMD_GPIO             GPIO3
#define USDHC1_CMD_GPIO_PIN         (12U)
#define USDHC1_CMD_IOMUXC           IOMUXC_GPIO_SD_B0_00_USDHC1_CMD

/*SD1_CLK*/
#define USDHC1_CLK_GPIO             GPIO2
#define USDHC1_CLK_GPIO_PIN         (30U)
#define USDHC1_CLK_IOMUXC           IOMUXC_GPIO_SD_B0_01_USDHC1_CLK

/*SD1_D0*/
#define USDHC1_DATA0_GPIO             GPIO3
#define USDHC1_DATA0_GPIO_PIN         (14U)
#define USDHC1_DATA0_IOMUXC           IOMUXC_GPIO_SD_B0_02_USDHC1_DATA0

/*SD1_D1*/
#define USDHC1_DATA1_GPIO             GPIO3
#define USDHC1_DATA1_GPIO_PIN         (15U)
#define USDHC1_DATA1_IOMUXC           IOMUXC_GPIO_SD_B0_03_USDHC1_DATA1

/*SD1_D2*/
#define USDHC1_DATA2_GPIO             GPIO3
#define USDHC1_DATA2_GPIO_PIN         (16U)
#define USDHC1_DATA2_IOMUXC           IOMUXC_GPIO_SD_B0_04_USDHC1_DATA2

/*SD1_D3*/
#define USDHC1_DATA3_GPIO             GPIO3
#define USDHC1_DATA3_GPIO_PIN         (17U)
#define USDHC1_DATA3_IOMUXC           IOMUXC_GPIO_SD_B0_05_USDHC1_DATA3

/**********************���⹦������********************************/
/**/
//#define GPT1_COMPARE3_GPIO             GPIO1
//#define GPT1_COMPARE3_GPIO_PIN         (5U)
//#define GPT1_COMPARE3_IOMUXC           IOMUXC_GPIO_B1_14_USDHC1_VSELECT
//
///**/
//#define GPT1_COMPARE2_GPIO             GPIO2
//#define GPT1_COMPARE2_GPIO_PIN         (28U)
//#define GPT1_COMPARE2_IOMUXC           IOMUXC_GPIO_B1_12_GPIO2_IO28
//
//
///**/
//#define GPT1_COMPARE3_GPIO             GPIO1
//#define GPT1_COMPARE3_GPIO_PIN         (5U)
//#define GPT1_COMPARE3_IOMUXC           IOMUXC_GPIO_AD_B0_05_GPIO1_IO05




/* USDHC1 DATA���� CMD���� PAD�������� IO28,*/
#define USDHC1_DATA_PAD_CONFIG_DATA     (SRE_1_FAST_SLEW_RATE| \
                                        DSE_1_R0_1| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_1_PULL_SELECTED| \
                                        PUS_1_47K_OHM_PULL_UP| \
                                        HYS_1_HYSTERESIS_ENABLED)

    /* ����˵�� : */
    /* ת������: ת�����ʿ�
        ����ǿ��: R0 
        �������� : medium(100MHz)
        ��©����: �ر� 
        ��/����������: ʹ��
        ��/������ѡ��: ������
        ����/����ѡ��: 4.7Kŷķ����(ѡ���˱�������������Ч)
        �ͻ�������: ��ֹ */ 

/* USDHC1 CLK����PAD�������� */
#define USDHC1_CLK_PAD_CONFIG_DATA       (SRE_1_FAST_SLEW_RATE| \
                                        DSE_1_R0_1| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_1_47K_OHM_PULL_UP| \
                                        HYS_1_HYSTERESIS_ENABLED)
    /* ����˵�� : */
    /* ת������: ת�����ʿ�
        ����ǿ��: R0 
        �������� : medium(100MHz)
        ��©����: �ر� 
        ��/����������: ʹ��
        ��/������ѡ��: ������
        ����/����ѡ��: 4.7Kŷķ����(ѡ���˱�������������Ч)
        �ͻ�������: ��ֹ */

///* USDHC1 VSELECT����PAD�������� */
//#define TMR_PWM_OUTPUT_PAD_CONFIG_DATA       (SRE_1_FAST_SLEW_RATE| \
//                                        DSE_4_R0_4| \
//                                        SPEED_2_MEDIUM_100MHz| \
//                                        ODE_0_OPEN_DRAIN_DISABLED| \
//                                        PKE_1_PULL_KEEPER_ENABLED| \
//                                        PUE_1_PULL_SELECTED| \
//                                        PUS_1_47K_OHM_PULL_UP| \
//                                        HYS_1_HYSTERESIS_ENABLED)
//
//    /* ����˵�� : */
//    /* ת������: ת�����ʿ�
//        ����ǿ��: R0/4
//        �������� : medium(100MHz)
//        ��©����: �ر� 
//        ��/����������: ʹ��
//        ��/������ѡ��: ������
//        ����/����ѡ��: 4.7Kŷķ����(ѡ���˱�������������Ч)
//        �ͻ�������: ��ֹ */ 



void USDHC1_gpio_init(void);
static status_t AccessCard(sd_card_t *card);
static void BOARD_USDHCClockConfiguration(void);

void SDCardTest(void);
int SDCard_Init(void);

#endif /* __BSP_SD_H */                             