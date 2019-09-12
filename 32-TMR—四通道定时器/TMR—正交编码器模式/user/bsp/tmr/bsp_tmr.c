#include "fsl_gpio.h"
#include "pad_config.h"
#include "fsl_debug_console.h"

#include "./bsp/nvic/bsp_nvic.h"
#include "./bsp/tmr/bsp_tmr.h"



void TMR_Quadrature_Count_GPIO_Init(void)
{
    /*����GPIO�������ýṹ��*/
  gpio_pin_config_t gpt_config; 
  
  /*���ó�ʼ���ṹ��*/
  gpt_config.direction = kGPIO_DigitalInput; //����ģʽ
  gpt_config.interruptMode = kGPIO_NoIntmode; //��ʹ���ж�
  
  
   /*��ʼ��QTMR��ʱ��CH2 �ⲿ����*/
  IOMUXC_SetPinMux(QTMR_CH2_IOMUXC, 0U);
  IOMUXC_SetPinConfig(QTMR_CH2_IOMUXC, TMR_Quadrature_Count_PAD_CONFIG_DATA);
  GPIO_PinInit(QTMR_CH2_GPIO, QTMR_CH0_GPIO_PIN, &gpt_config);
  
  /*��ʼ��QTMR��ʱ��CH3 �ⲿ����*/
  IOMUXC_SetPinMux(QTMR_CH3_IOMUXC, 0U);
  IOMUXC_SetPinConfig(QTMR_CH3_IOMUXC, TMR_Quadrature_Count_PAD_CONFIG_DATA);
  GPIO_PinInit(QTMR_CH3_GPIO, QTMR_CH3_GPIO_PIN, &gpt_config);   
}



void TMR_Quadrature_Count_init(void)
{    
  
  qtmr_config_t qtmrConfig; /*����TMR ��ʱ����ʼ���ṹ��*/
 
  /*��ʼ���ⲿ��������*/
  TMR_Quadrature_Count_GPIO_Init();
  
   /*��ʼ��TMR ��ʱ��*/
  QTMR_GetDefaultConfig(&qtmrConfig);
  qtmrConfig.primarySource = kQTMR_ClockCounter2InputPin;  //������Դ ѡ��ʱ����������2   
  qtmrConfig.secondarySource = kQTMR_Counter3InputPin;     //��������Դ ѡ��ʱ����������3
  qtmrConfig.debugMode = kQTMR_RunNormalInDebug;           //��debugģʽ�¶�ʱ����������
  QTMR_Init(QTMR_BASEADDR, QTMR_PWM_CHANNEL_0, &qtmrConfig);
  
  /*������ʱ��*/
  QTMR_StartTimer(QTMR_BASEADDR, QTMR_PWM_CHANNEL_0, kQTMR_QuadCountMode);


}


/**************************MR_Init end**************************/
