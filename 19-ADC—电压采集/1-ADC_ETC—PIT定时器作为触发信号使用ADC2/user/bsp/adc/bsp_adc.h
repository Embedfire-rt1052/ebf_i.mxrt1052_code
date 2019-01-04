#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#include "fsl_common.h"
#include "fsl_adc.h"
#include "fsl_adc_etc.h"

/*********************************************************
 * ADC GPIO�˿ڡ����źż�IOMUXC���ú궨��
 
 ʹ��i.MX RT1052-Pro�װ壺
 *  ADC1 CH0-----GPIO_AD_B1_11,J13-----(CN5, 29)
 *  CH15 CH15----GPIO_AD_B1_10,l13-----(CN5, 31)
 *���Ź���         ���ű��        �����ڿ������϶�Ӧλ��
 * CH0 ���Ŷ�Ӧ������CN5����ĵ�29��(CN5, 29)
 * CH15���Ŷ�Ӧ������CN5����ĵ�31��(CN5, 31)

ʹ��i.MX RT1052-Mini�װ�
 *  ADC1 CH0-----GPIO_AD_B1_11,J13-----(CN4, 29)
 *  CH15 CH15----GPIO_AD_B1_10,l13-----(CN4, 31)
 *���Ź���         ���ű��        �����ڿ������϶�Ӧλ��
 * CH0 ���Ŷ�Ӧ������CN4����ĵ�29��(CN4, 29)
 * CH15���Ŷ�Ӧ������CN4����ĵ�31��(CN4, 31) 

 *********************************************************/
 //GPIO_AD_B1_11
#define CORE_BOARD_ADC_GPIO_CH0             GPIO1
#define CORE_BOARD_ADC_GPIO_PIN_CH0         (27U)
#define CORE_BOARD_ADC_IOMUXC_CH0          IOMUXC_GPIO_AD_B1_11_GPIO1_IO27

 //GPIO_AD_B1_10
#define CORE_BOARD_ADC_GPIO_CH15             GPIO1
#define CORE_BOARD_ADC_GPIO_PIN_CH15         (26U)
#define CORE_BOARD_ADC_IOMUXC_CH15           IOMUXC_GPIO_AD_B1_10_GPIO1_IO26
 


/*����ʹ�õ�ADCģ��*/ 
#define    ADCx                          ADC2




/**
*����ADC_ETCת����
*ÿ��ADC��4��ת��ͨ���飬ADC1��Ӧ0��3��ADC2��Ӧ4��7.
*ÿ��ADC_ETCת��ͨ�����������7��ADCת��ͨ����ת�������ɺ�DEMO_ADC_ETC_CHAIN_LENGTH�趨
*��һ��ADC_ETCת��ͨ�����յ������ź�ʱ��ʼת����ͨ����ָ����ADCת��ͨ��.
*/
#define ADC_ETC_CHANNEL_GROUPx 7


/**
*����ADCת��ͨ����
*ÿ��ADC��8��������ת��ͨ����0��7����ÿ��ת��ͨ��ֻ��ָ��һ���ⲿ����ͨ����һ��ADC��16���ⲿ����ͨ����
*/
#define DEMO_ADC_CHANNEL_GROUP0 4U
#define DEMO_ADC_CHANNEL_GROUP1 5U

/**
*����ADC����ͨ����ÿ��ADC��16���ⲿ����ͨ����0��15����Ӧ��оƬ�Ĳ�ͬ���š�
*16�����ⲿ����ͨ����ADC_ETC��Ӧ�Ĵ���������
*/
#define DEMO_ADC_ETC_CHANNLE_NUMBER_x 16
#define DEMO_ADC_ETC_CHANNLE_NUMBER_0 0
#define DEMO_ADC_ETC_CHANNLE_NUMBER_15 15

/**
*����ADC_ETC��ַ������adcת�����ĳ��� 
*ת��������,��һ���ⲿ����ת�����ٸ�ADCת��ͨ�������� =  DEMO_ADC_ETC_CHAIN_LENGTH + 1
*/
#define DEMO_ADC_ETC_BASE ADC_ETC
#define DEMO_ADC_ETC_CHAIN_LENGTH 1U 


/*�����жϷ�����*/
#define EXAMPLE_ADC_ETC_DONE0_Handler ADC_ETC_IRQ0_IRQHandler
#define EXAMPLE_ADC_ETC_DONE1_Handler ADC_ETC_IRQ1_IRQHandler
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

