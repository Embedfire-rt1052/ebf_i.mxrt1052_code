#include "fsl_gpio.h"
#include "pad_config.h"
#include "fsl_debug_console.h"

#include "./bsp/tmr/bsp_tmr.h"


/**
* @brief  ����TMR������Ź���
* @param  ��
* @retval ��
*/
void TMR_GPIO_Config(void)
{
  /*����GPIO�������ýṹ��*/
  gpio_pin_config_t gpt_config; 
  
  /*���ó�ʼ���ṹ��*/
  gpt_config.direction = kGPIO_DigitalOutput; //���ģʽ
  gpt_config.outputLogic =  1;                //Ĭ�ϸߵ�ƽ
  gpt_config.interruptMode = kGPIO_NoIntmode; //��ʹ���ж�
  
  /*��ʼ��QTMR��ʱ��CH0 �ⲿ����*/
  IOMUXC_SetPinMux(QTMR_CH0_IOMUXC, 0U);
  IOMUXC_SetPinConfig(QTMR_CH0_IOMUXC, TMR_PWM_OUTPUT_PAD_CONFIG_DATA);
  GPIO_PinInit(QTMR_CH0_GPIO, QTMR_CH0_GPIO_PIN, &gpt_config);
  
  /*��ʼ��QTMR��ʱ��CH1 �ⲿ����*/
  IOMUXC_SetPinMux(QTMR_CH1_IOMUXC, 0U);
  IOMUXC_SetPinConfig(QTMR_CH1_IOMUXC, TMR_PWM_OUTPUT_PAD_CONFIG_DATA);
  GPIO_PinInit(QTMR_CH1_GPIO, QTMR_CH1_GPIO_PIN, &gpt_config);
  
  /*��ʼ��QTMR��ʱ��CH2 �ⲿ����*/
  IOMUXC_SetPinMux(QTMR_CH2_IOMUXC, 0U);
  IOMUXC_SetPinConfig(QTMR_CH2_IOMUXC, TMR_PWM_OUTPUT_PAD_CONFIG_DATA);
  GPIO_PinInit(QTMR_CH2_GPIO, QTMR_CH2_GPIO_PIN, &gpt_config);
  
  /*��ʼ��QTMR��ʱ��CH3 �ⲿ����*/
  IOMUXC_SetPinMux(QTMR_CH3_IOMUXC, 0U);
  IOMUXC_SetPinConfig(QTMR_CH3_IOMUXC, TMR_PWM_OUTPUT_PAD_CONFIG_DATA);
  GPIO_PinInit(QTMR_CH3_GPIO, QTMR_CH3_GPIO_PIN, &gpt_config);
}


/**
* @brief  ����TMR����ģʽ
* @param  ��
* @retval ��
*/  
void TMR_Init(void)
{
  qtmr_config_t qtmrConfig; /*����TMR ��ʱ����ʼ���ṹ��*/
  
  /*��ʼ����������*/
  TMR_GPIO_Config();
  
  /*��ʼ��TMR ��ʱ��*/
  QTMR_GetDefaultConfig(&qtmrConfig);

  qtmrConfig.primarySource = kQTMR_ClockDivide_8;
  QTMR_Init(QTMR_BASEADDR, QTMR_PWM_CHANNEL_0,  &qtmrConfig);
  /* ����CH0��PWM���������ָ��PWM��Ƶ����ռ�ձ�*/
  QTMR_SetupPwm(QTMR_BASEADDR, QTMR_PWM_CHANNEL_0, TMR3_CH0_PWM_FREQUENCY, TMR3_CH0_PWM_DUTYCYCLE, false, QTMR_SOURCE_CLOCK / 8);

  qtmrConfig.primarySource = kQTMR_ClockDivide_8;
  QTMR_Init(QTMR_BASEADDR, QTMR_PWM_CHANNEL_1,  &qtmrConfig); 
  /* ����CH1��PWM���������ָ��PWM��Ƶ����ռ�ձ�*/
  QTMR_SetupPwm(QTMR_BASEADDR, QTMR_PWM_CHANNEL_1, TMR3_CH1_PWM_FREQUENCY, TMR3_CH1_PWM_DUTYCYCLE, false, QTMR_SOURCE_CLOCK / 8);

  qtmrConfig.primarySource = kQTMR_ClockDivide_8;
  QTMR_Init(QTMR_BASEADDR, QTMR_PWM_CHANNEL_2,  &qtmrConfig);
  /* ����CH2��PWM���������ָ��PWM��Ƶ����ռ�ձ�*/
  QTMR_SetupPwm(QTMR_BASEADDR, QTMR_PWM_CHANNEL_2, TMR3_CH2_PWM_FREQUENCY, TMR3_CH2_PWM_DUTYCYCLE, false, QTMR_SOURCE_CLOCK / 8);

  qtmrConfig.primarySource = kQTMR_ClockDivide_128;
  QTMR_Init(QTMR_BASEADDR, QTMR_PWM_CHANNEL_3,  &qtmrConfig);
  /* ����CH3��PWM���������ָ��PWM��Ƶ����ռ�ձ�*/
  QTMR_SetupPwm(QTMR_BASEADDR, QTMR_PWM_CHANNEL_3, TMR3_CH3_PWM_FREQUENCY, TMR3_CH3_PWM_DUTYCYCLE, false, QTMR_SOURCE_CLOCK / 128);

  
  /* ������ʱ��������������ʱ�ӵ������ؼ��� */
  QTMR_StartTimer(QTMR_BASEADDR, QTMR_PWM_CHANNEL_0, kQTMR_PriSrcRiseEdge);
  QTMR_StartTimer(QTMR_BASEADDR, QTMR_PWM_CHANNEL_1, kQTMR_PriSrcRiseEdge);
  QTMR_StartTimer(QTMR_BASEADDR, QTMR_PWM_CHANNEL_2, kQTMR_PriSrcRiseEdge);
  QTMR_StartTimer(QTMR_BASEADDR, QTMR_PWM_CHANNEL_3, kQTMR_PriSrcRiseEdge);
  
}







