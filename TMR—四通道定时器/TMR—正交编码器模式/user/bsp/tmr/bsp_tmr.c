#include "fsl_gpio.h"
#include "pad_config.h"
#include "fsl_debug_console.h"
#include "bsp_tmr.h"


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

/**************************MR_Init end**************************/


void MR_Quadrature_Count_GPIO_Init(void)
{
    /*����GPIO�������ýṹ��*/
  gpio_pin_config_t gpt_config; 
  
  /*���ó�ʼ���ṹ��*/
  gpt_config.direction = kGPIO_DigitalInput; //����ģʽ
  gpt_config.interruptMode = kGPIO_NoIntmode; //��ʹ���ж�
  
  
   /*��ʼ��QTMR��ʱ��CH0 �ⲿ����*/
  IOMUXC_SetPinMux(QTMR_CH2_IOMUXC, 0U);
  IOMUXC_SetPinConfig(QTMR_CH2_IOMUXC, TMR_Quadrature_Count_PAD_CONFIG_DATA);
  GPIO_PinInit(QTMR_CH2_GPIO, QTMR_CH0_GPIO_PIN, &gpt_config);
  
  /*��ʼ��QTMR��ʱ��CH1 �ⲿ����*/
  IOMUXC_SetPinMux(QTMR_CH3_IOMUXC, 0U);
  IOMUXC_SetPinConfig(QTMR_CH3_IOMUXC, TMR_Quadrature_Count_PAD_CONFIG_DATA);
  GPIO_PinInit(QTMR_CH3_GPIO, QTMR_CH3_GPIO_PIN, &gpt_config);
  
     
}

void MR_Quadrature_Count(TMR_Type *base, qtmr_channel_selection_t channel)
{    
  

    MR_Quadrature_Count_GPIO_Init();
    
    base->CHANNEL[channel].CTRL &= ~(0x0f<<9);  //ѡ����ʱ��Դ
    base->CHANNEL[channel].CTRL |=  (0x02<<9);
    
    base->CHANNEL[channel].CTRL &= ~(0x03<<7);
    base->CHANNEL[channel].CTRL |= (0x03<<7);   //ѡ����ʱ��Դ 
    

//    base->CHANNEL[channel].CTRL |= (0x80);   //ѡ����ʱ��Դ 
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


