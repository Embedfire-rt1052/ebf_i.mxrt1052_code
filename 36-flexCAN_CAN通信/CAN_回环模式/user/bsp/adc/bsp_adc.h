#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#include "fsl_common.h"
#include "fsl_adc.h"

/*********************************************************
 * ADC GPIO�˿ڡ����źż�IOMUXC���ú궨��
ʹ��i.MX RT1052-Pro�װ壺
 *  ADC1_IN0-----GPIO_AD_B1_11-----(CN5,29)
 *  ���Ź���       ���ű��        �����ڿ������϶�Ӧλ��
 (CN5, 68) �������������ӵ�������CN5����ĵ�68��

ʹ��i.MX RT1052-Mini�װ�    
 *  ADC1_IN0-----GPIO_AD_B1_11-----(CN4,29)
 *���Ź���       ���ű��        �����ڿ������϶�Ӧλ��
 (CN4,29) �������������ӵ�������CN4����ĵ�29�� 
 *********************************************************/
#define CORE_BOARD_ADC_GPIO             GPIO1
#define CORE_BOARD_ADC_GPIO_PIN         (27U)
#define CORE_BOARD_ADC_IOMUXC           IOMUXC_GPIO_AD_B1_11_GPIO1_IO27

/*����ʹ�õ�ADC*/ 
#define    ADCx              ADC1
#define DEMO_ADC_CHANNEL_GROUP 0U //ADC ͨ���飬���ֻ��ʹ��8��ͨ���飨0��7��
#define DEMO_ADC_USER_CHANNEL 0U  //ADC ͨ���궨��

/*�ж���غ궨��*/ 
#define    ADC_IRQ                       ADC1_IRQn
#define    ADC_IRQHandler                ADC1_IRQHandler

/*********************************************************
 * ADC����PAD����
 ****************************************************************/
#define ADC_PAD_CONFIG_DATA            (SRE_0_SLOW_SLEW_RATE| \
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

static void ADC_IOMUXC_MUX_Config(void);
static void ADC_IOMUXC_PAD_Config(void);
static void ADC_IO_Mode_Config(void);
static void ADC_Mode_Config(void);

void ADC_Config(void);

#endif /* __BSP_ADC_H */



















