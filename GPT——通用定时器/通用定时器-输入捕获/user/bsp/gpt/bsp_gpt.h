#ifndef __BSP_GPT_H
#define __BSP_GPT_H

#include "fsl_iomuxc.h"
#include "fsl_common.h"
#include "fsl_gpt.h"




/* ѡ�� IPG Clock as PERCLK_CLK clock source */
#define EXAMPLE_GPT_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for PERCLK_CLK clock source */
#define EXAMPLE_GPT_CLOCK_DIVIDER_SELECT (0U)

/*GPT ʱ�ӷ�Ƶ(1-4096)*/
#define GPT_DIVIDER 1

/* �õ�GPT��ʱ���ļ���Ƶ��*/
#define EXAMPLE_GPT_CLK_FREQ ( (CLOCK_GetFreq(kCLOCK_IpgClk) / (EXAMPLE_GPT_CLOCK_DIVIDER_SELECT + 1U))/GPT_DIVIDER )

/*����ʹ�õ�GPT*/
#define EXAMPLE_GPT GPT2

/*�жϺź��жϷ������궨��*/
#define GPT_IRQ_ID GPT2_IRQn
#define EXAMPLE_GPT_IRQHandler GPT2_IRQHandler





/*********************************************************
 * GPT GPIO�˿ڡ����źż�IOMUXC���ú궨��
 *********************************************************/
  /*��W9825G6KH-6��CAS�ų�ͻ*/
#define GPT1_CAPTURE1_GPIO             GPIO4 
#define GPT1_CAPTURE1_GPIO_PIN         (24U)
#define GPT1_CAPTURE1_IOMUXC           IOMUXC_GPIO_EMC_24_GPT1_CAPTURE1
 /*��W9825G6KH-6��A10��ͻ*/
#define GPT1_CAPTURE2_GPIO             GPIO4
#define GPT1_CAPTURE2_GPIO_PIN         (23U)
#define GPT1_CAPTURE2_IOMUXC           IOMUXC_GPIO_EMC_23_GPT1_CAPTURE2
 
 
 
 
 /*NAND Flash ��CE�ų�ͻ��CN4,27��*/
#define GPT2_CAPTURE1_GPIO             GPIO3 
#define GPT2_CAPTURE1_GPIO_PIN         (27U)
#define GPT2_CAPTURE1_IOMUXC           IOMUXC_GPIO_EMC_41_GPT2_CAPTURE1



/* sw3, (CN4,48)*/
 /*NAND Flash ��R/B�Ź��ã������10K�������裨CN4,25��*/
#define GPT2_CAPTURE2_GPIO             GPIO3
#define GPT2_CAPTURE2_GPIO_PIN         (26U)
#define GPT2_CAPTURE2_IOMUXC           IOMUXC_GPIO_EMC_40_GPT2_CAPTURE2




// ��ʱ�����벶���û��Զ�������ṹ������
typedef struct
{   
	uint8_t   Capture_FinishFlag;   // ���������־λ
	uint8_t   Capture_StartFlag;    // ����ʼ��־λ
	uint32_t  Capture_CcrValue_1;     // ����Ĵ�����ֵ
  uint32_t   Capture_CcrValue_2;     // ����Ĵ�����ֵ
	uint16_t  Capture_Period;       // ��ʱ��������� 
}GPT_ICUserValueTypeDef;




/********************************************************************************/


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


