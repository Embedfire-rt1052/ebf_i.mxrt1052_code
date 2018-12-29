
#include "fsl_debug_console.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"  
#include "fsl_adc.h"
  
#include "pad_config.h"  
#include "./adc/bsp_adc.h" 


extern volatile uint32_t g_AdcConversionValue0;
extern volatile  bool b_Value0_Conversion_complete_flag;

/**
* @brief  ��ʼ��ADC���IOMUXC��MUX��������
* @param  ��
* @retval ��
*/
static void ADC_IOMUXC_MUX_Config(void)
{
  IOMUXC_SetPinMux(CORE_BOARD_ADC_IOMUXC_CH0, 0U);  
}

/**
* @brief  ��ʼ��ADC���IOMUXC��MUX��������
* @param  ��
* @retval ��
*/
static void ADC_IOMUXC_PAD_Config(void)
{
  IOMUXC_SetPinConfig(CORE_BOARD_ADC_IOMUXC_CH0, ADC_PAD_CONFIG_DATA);    
}

 /**
  * @brief  ��ʼ��ADC��ص�GPIOģʽ
  * @param  ��
  * @retval ��
  */
static void ADC_IO_Mode_Config(void)
{
  /* ����gpio��ʼ�����ýṹ�� */
  gpio_pin_config_t adc_config; 

  
   /*ADC��GPIO���� */   
  adc_config.direction = kGPIO_DigitalInput; //����ģʽ
  //adc_config.outputLogic =  1;                //Ĭ�ϸߵ�ƽ�������ģʽ�����ø�ѡ����Ч
  adc_config.interruptMode = kGPIO_NoIntmode; //��ʹ���ж�
  GPIO_PinInit(CORE_BOARD_ADC_GPIO_CH0, CORE_BOARD_ADC_GPIO_PIN_CH0, &adc_config);
}


 /**
  * @brief  ��ʼ��ADC����ģʽ
  * @param  ��
  * @retval ��
  */
static void ADC_Mode_Config(void)
{
  adc_config_t adcConfigStrcut;                //����ADC ģʽ���ýṹ��
  adc_channel_config_t adcChannelConfigStruct;    //ADC ͨ�����ýṹ��
  
  /*���ù���ģʽ*/
  ADC_GetDefaultConfig(&adcConfigStrcut);         //��ȡADC Ĭ�Ϲ���ģʽ
  adcConfigStrcut.resolution = kADC_Resolution12Bit;
  ADC_Init(ADCx, &adcConfigStrcut);               //����ADC����ģʽ
  

  ADC_EnableHardwareTrigger(ADCx, true);          //ʹ��Ӳ������ģʽ 
  
  
   /*����ת��ͨ����*/
  adcChannelConfigStruct.channelNumber = DEMO_ADC_ETC_CHANNEL1;    //ѡ����ת��ͨ���������ת��ͨ��
  adcChannelConfigStruct.enableInterruptOnConversionCompleted = false; //��ֹת�������
  
  ADC_SetChannelConfig(ADCx, DEMO_ADC_CHANNEL_GROUP0, &adcChannelConfigStruct);
  
  
  /*����Ӳ��У׼*/
  while(kStatus_Success != ADC_DoAutoCalibration(ADCx))
  {
    PRINTF("У׼ʧ��\r\n");
  }
  PRINTF("У׼��� Done.\r\n");
  
  
}

/*����Ϊ�����ⲿ����*/
void ADC_ETC_Config(void)
{
  adc_etc_config_t adcEtcConfig;//�����ⲿ����������
  adc_etc_trigger_config_t adcEtcTriggerConfig; //�����ⲿ�����������Ĵ������ԡ���Ҫ�������ȼ���������ʽ������ͨ������
  adc_etc_trigger_chain_config_t adcEtcTriggerChainConfig; // �����ⲿ��������������Ĵ���ͨ��
  
  
  
  ADC_ETC_GetDefaultConfig(&adcEtcConfig);
  adcEtcConfig.enableTSCBypass = false;
  adcEtcConfig.XBARtriggerMask = 1<<ADC_ETC_CHANNEL_GROUPx; /* �����ⲿ��������ͨ��0*/
  ADC_ETC_Init(DEMO_ADC_ETC_BASE, &adcEtcConfig);
  
  
  /* 
   *����ADC�ⲿ����Դ���ԡ�
  */
  adcEtcTriggerConfig.enableSyncMode = true;                        //ʹ��ͬ��ģʽ��ͬ����ADC1��ADC2����ͬ�Ĵ���Դ����
  adcEtcTriggerConfig.enableSWTriggerMode = true;                    //ʹ���������ģʽ
  adcEtcTriggerConfig.triggerChainLength = DEMO_ADC_ETC_CHAIN_LENGTH; /* Chain length is 1.�����ж��ٸ�ADCͨ������ת�� */
  adcEtcTriggerConfig.triggerPriority = 0U;                          //�ⲿ�������ȼ�
  adcEtcTriggerConfig.sampleIntervalDelay = 0U;                      //���ò���ʱ����                
  adcEtcTriggerConfig.initialDelay = 0U;                             //���ô�������ʼ��ʱ
  //DEMO_ADC_ETC_BASE ����������ַ���������� ����������
  ADC_ETC_SetTriggerConfig(DEMO_ADC_ETC_BASE, ADC_ETC_CHANNEL_GROUPx, &adcEtcTriggerConfig);//�����ⲿXBAR����������
  
  
  
  /*��ת��ͨ��ָ����chainGroup */
  adcEtcTriggerChainConfig.enableB2BMode = true;
  
  adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U<<DEMO_ADC_CHANNEL_GROUP0;       //ѡ��Ҫ������ADC_HC1�Ĵ���
  
  
  adcEtcTriggerChainConfig.ADCChannelSelect = DEMO_ADC_ETC_CHANNEL1;                   // ADC����ͨ��
  
  ADC_ETC_ClearInterruptStatusFlags(DEMO_ADC_ETC_BASE, (adc_etc_external_trigger_source_t)ADC_ETC_CHANNEL_GROUPx, kADC_ETC_Done0StatusFlagMask);
  
  adcEtcTriggerChainConfig.InterruptEnable = kADC_ETC_Done0InterruptEnable;            // ʹ�ܸ�ͨ����ת������жϣ��������ж��ܹ�������. 
  
  ADC_ETC_SetTriggerChainConfig(DEMO_ADC_ETC_BASE, ADC_ETC_CHANNEL_GROUPx, 0, &adcEtcTriggerChainConfig); //���ô���chain1.  ��ǰ�涨���� chainl ������Ϊ1
  
 /*****************************************************************************************************************************/


  /* ʹ���ж�. */
  EnableIRQ(ADC_ETC_IRQ0_IRQn);


}



/**
  * @brief  ��ʼ������ADC��IO
  * @param  ��
  * @retval ��
  */
void ADC_Config(void)
{
  
  ADC_IOMUXC_MUX_Config();//�������Ź���
  ADC_IOMUXC_PAD_Config();//�����������ò���������ǿ�ȡ�ת�����ʵ�
  ADC_IO_Mode_Config();   //��������ģʽ������/������Ƿ����жϵ�
  ADC_Mode_Config();      //��ʼ��ADC����ģʽ�����ҽ���Ӳ��У׼��
  
  ADC_ETC_Config();

}

/*�жϷ�����*/
void EXAMPLE_ADC_ETC_DONE0_Handler(void)
{
  ADC_ETC_ClearInterruptStatusFlags(DEMO_ADC_ETC_BASE, (adc_etc_external_trigger_source_t)ADC_ETC_CHANNEL_GROUPx, kADC_ETC_Done0StatusFlagMask);
  g_AdcConversionValue0 = ADC_ETC_GetADCConversionValue(DEMO_ADC_ETC_BASE, ADC_ETC_CHANNEL_GROUPx, 0U); /* Get trigger0 chain0 result. */
  b_Value0_Conversion_complete_flag = true;
}



