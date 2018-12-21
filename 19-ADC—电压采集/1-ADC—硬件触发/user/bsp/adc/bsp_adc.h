#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#include "fsl_common.h"
#include "fsl_adc.h"
#include "fsl_adc_etc.h"

/*********************************************************
 * ADC GPIO�˿ڡ����źż�IOMUXC���ú궨��
 *********************************************************/
 //GPIO_AD_B1_11
#define CORE_BOARD_ADC_GPIO_CH0             GPIO1
#define CORE_BOARD_ADC_GPIO_PIN_CH0         (27U)
#define CORE_BOARD_ADC_IOMUXC_CH0          IOMUXC_GPIO_AD_B1_11_GPIO1_IO27

 //GPIO_AD_B1_10
#define CORE_BOARD_ADC_GPIO_CH15             GPIO1
#define CORE_BOARD_ADC_GPIO_PIN_CH15             (26U)
#define CORE_BOARD_ADC_IOMUXC_CH15           IOMUXC_GPIO_AD_B1_10_GPIO1_IO26
 

// ADC ���ѡ��
// ������ ADC1/2���ж�Ҫ��ADC��Ӧ
#define    ADCx                          ADC1



// ADC �ж���غ궨��
#define    ADC_IRQ                       ADC1_IRQn
#define    ADC_IRQHandler                ADC1_IRQHandler
#define EXAMPLE_ADC_ETC_DONE0_Handler ADC_ETC_IRQ0_IRQHandler
#define EXAMPLE_ADC_ETC_DONE1_Handler ADC_ETC_IRQ1_IRQHandler

/*ADC ͨ���飬���ֻ��ʹ��8��ͨ���飨0��7��*/
#define DEMO_ADC_CHANNEL_GROUP 0U

/*ADC ͨ���궨��*/
#define DEMO_ADC_USER_CHANNEL0 15U
#define DEMO_ADC_USER_CHANNEL1 0U


/*�ⲿת��ͨ���궨�壬ȡֵ��Χ 0~31 Ϊʲô����ȡ16 �������*/
#define DEMO_ADC_USER_CHANNEL 16U

/*����ת��ͨ���飬ȡֵ��ΧΪ0~8��һ��ת��ͨ������԰������ת��ͨ������������ֻ����һ��*/
#define DEMO_ADC_CHANNEL_GROUP0 0U
#define DEMO_ADC_CHANNEL_GROUP1 1U


#define DEMO_ADC_ETC_BASE ADC_ETC
#define DEMO_ADC_ETC_CHAIN_LENGTH 1U /* Chain length is 2. */

/*����ת��ͨ����*/
#define DEMO_ADC_ETC_CHANNEL0 15U
#define DEMO_ADC_ETC_CHANNEL1 0U
/*******************************************************************************
 * ADC����PAD����
 ******************************************************************************/
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
