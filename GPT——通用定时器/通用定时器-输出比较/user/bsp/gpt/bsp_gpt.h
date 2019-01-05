#ifndef __BSP_GPT_H
#define __BSP_GPT_H

#include "fsl_iomuxc.h"
#include "fsl_common.h"
#include "fsl_gpt.h"


/* ѡ�� IPG Clock as PERCLK_CLK clock source */
#define EXAMPLE_GPT_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for PERCLK_CLK clock source */
#define EXAMPLE_GPT_CLOCK_DIVIDER_SELECT (0U)


/* Get source clock for GPT driver (GPT prescaler = 0) */
#define EXAMPLE_GPT_CLK_FREQ (CLOCK_GetFreq(kCLOCK_IpgClk) / (EXAMPLE_GPT_CLOCK_DIVIDER_SELECT + 1U))

#define EXAMPLE_GPT GPT2

#define GPT_IRQ_ID GPT2_IRQn
#define EXAMPLE_GPT_IRQHandler GPT2_IRQHandler



/*********************************************************
 * GPT GPIO�˿ڡ����źż�IOMUXC���ú궨��
 *********************************************************/
 /*��SEMC_D13����*/
#define GPT1_COMPARE1_GPIO             GPIO3 
#define GPT1_COMPARE1_GPIO_PIN         (21U)
#define GPT1_COMPARE1_IOMUXC           IOMUXC_GPIO_EMC_35_GPT1_COMPARE1
 /*��SEMC_D14����*/
#define GPT1_COMPARE2_GPIO             GPIO3
#define GPT1_COMPARE2_GPIO_PIN         (22U)
#define GPT1_COMPARE2_IOMUXC           IOMUXC_GPIO_EMC_36_GPT1_COMPARE2
 /*��SEMC_D15����*/
#define GPT1_COMPARE3_GPIO             GPIO3
#define GPT1_COMPARE3_GPIO_PIN         (23U)
#define GPT1_COMPARE3_IOMUXC           IOMUXC_GPIO_EMC_37_GPT1_COMPARE3

/********************************************************************************/

/*��JTAG_TMS ����*/
#define GPT2_COMPARE1_GPIO             GPIO1
#define GPT2_COMPARE1_GPIO_PIN         (6U)
#define GPT2_COMPARE1_IOMUXC           IOMUXC_GPIO_AD_B0_06_GPT2_COMPARE1

/*��JTAG_TCK ����*/
#define GPT2_COMPARE2_GPIO             GPIO1
#define GPT2_COMPARE2_GPIO_PIN         (7U)
#define GPT2_COMPARE2_IOMUXC           IOMUXC_GPIO_AD_B0_07_GPT2_COMPARE2

/*��JTAG_MOD ���� �������10K��������*/
#define GPT2_COMPARE3_GPIO             GPIO1
#define GPT2_COMPARE3_GPIO_PIN         (8U)
#define GPT2_COMPARE3_IOMUXC           IOMUXC_GPIO_AD_B0_08_GPT2_COMPARE3
/*********************************************************************************/


/*******************************************************************************
 * ��
 ******************************************************************************/
/* GPT_COMPARE����ʹ�õ�PAD���� */
#define GPT_COMPARE_PAD_CONFIG_DATA    (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_0_PULL_KEEPER_DISABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_0_100K_OHM_PULL_DOWN| \
                                        HYS_0_HYSTERESIS_DISABLED)   
    /* ����˵�� : */
    /* ת������: ת��������
      ����ǿ��: R0/6 
      �������� : medium(100MHz)
      ��©����: �ر� 
      ��/����������: �ر�
      ��/������ѡ��: �������������ѹرգ�������Ч��
      ����/����ѡ��: 100Kŷķ�����������ѹرգ�������Ч��
      �ͻ�������: �ر� */ 


void GPT_GPIO_Config(void);
void GPT_Config(void);


#endif /* __BSP_GPT_H */


