#include "fsl_gpio.h"
#include "pad_config.h"
#include "fsl_debug_console.h"

#include "./tmr/bsp_tmr.h"



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

void TMR_Quadrature_Count_init(TMR_Type *base, qtmr_channel_selection_t channel)
{    
  /*��ʼ���ⲿ��������*/
  MR_Quadrature_Count_GPIO_Init();
  
  base->CHANNEL[channel].CTRL &= ~(0x0f<<9);  //ѡ����ʱ��Դ
  base->CHANNEL[channel].CTRL |=  (0x02<<9);
  
  base->CHANNEL[channel].CTRL &= ~(0x03<<7);  //ѡ����ʱ��Դ
  base->CHANNEL[channel].CTRL |= (0x03<<7);    
  
  
  /*����SCTRL�Ĵ�������λֵ*/
  base->CHANNEL[channel].SCTRL &= 0X00;
  base->CHANNEL[channel].SCTRL |= 0x01;   
  
  /*���ö�ʱ�������Ĵ����ͼ����Ĵ������Զ���װ��ֵΪ0����λֵ��*/
  base->CHANNEL[channel].CNTR = 0X00;
  base->CHANNEL[channel].LOAD = 0X00;
  
  /*���������ȽϼĴ����ĳ�ʼֵ*/
  base->CHANNEL[channel].COMP1 = 0XFFFF;
  base->CHANNEL[channel].COMP2 = 0X00;
  
  base->CHANNEL[channel].CSCTRL = 0X00;
  
  
  base->CHANNEL[channel].CTRL &= ~(0x07<<13);//�������ģʽѡ��λCTRL[CM]
  base->CHANNEL[channel].CTRL |= (0X04<<13);  //ѡ��ʹ����������ģʽ
  
  //QTMR_StartTimer(base, channel, kQTMR_PriSrcRiseEdge);

  /*ʹ���ж�*/
  EnableIRQ(QTMR3_IRQ_ID);   
  
  /*ʹ�ܱȽ��ж�*/
  QTMR_EnableInterrupts(base, channel, kQTMR_CompareInterruptEnable);
}



/*TMR��ʱ���жϷ�����*/
void QTMR3_IRQ_HANDLER(void)
{

  /* ����жϱ�־λ*/
  QTMR_ClearStatusFlags(QTMR_BASEADDR,QTMR_PWM_CHANNEL_0, kQTMR_CompareFlag);
  
}

/**************************MR_Init end**************************/
