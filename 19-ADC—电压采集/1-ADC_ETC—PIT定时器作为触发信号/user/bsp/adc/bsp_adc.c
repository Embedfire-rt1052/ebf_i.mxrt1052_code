
#include "fsl_debug_console.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"  
#include "fsl_adc.h"
  
#include "pad_config.h"  
#include "./bsp/nvic/bsp_nvic.h"
#include "./bsp/adc/bsp_adc.h" 


extern volatile uint32_t g_AdcConversionValue0;
extern volatile uint32_t g_AdcConversionValue1;
extern volatile  bool b_Value0_Conversion_complete_flag;
extern volatile  bool b_Value1_Conversion_complete_flag;   


/**
* @brief  ��ʼ��ADC���IOMUXC��MUX��������
* @param  ��
* @retval ��
*/
static void ADC_IOMUXC_MUX_Config(void)
{
  IOMUXC_SetPinMux(CORE_BOARD_ADC_IOMUXC_CH0, 0U);  
  IOMUXC_SetPinMux(CORE_BOARD_ADC_IOMUXC_CH15, 0U);
}

/**
* @brief  ��ʼ��ADC���IOMUXC��MUX��������
* @param  ��
* @retval ��
*/
static void ADC_IOMUXC_PAD_Config(void)
{
  IOMUXC_SetPinConfig(CORE_BOARD_ADC_IOMUXC_CH0, ADC_PAD_CONFIG_DATA);  
  IOMUXC_SetPinConfig(CORE_BOARD_ADC_IOMUXC_CH15, ADC_PAD_CONFIG_DATA);   
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
  GPIO_PinInit(CORE_BOARD_ADC_GPIO_CH15, CORE_BOARD_ADC_GPIO_PIN_CH15, &adc_config);
}


 /**
  * @brief  ��ʼ��ADC����ģʽ
  * @param  ��
  * @retval ��
  */
static void ADC_Mode_Config(void)
{
  adc_config_t adcConfigStrcut;                   //����ADC ģʽ���ýṹ��
  adc_channel_config_t adcChannelConfigStruct;    //ADC ͨ�����ýṹ��
  
  /*���ù���ģʽ*/
  ADC_GetDefaultConfig(&adcConfigStrcut);         //��ȡADC Ĭ�Ϲ���ģʽ
  adcConfigStrcut.resolution = kADC_Resolution12Bit;
  
  ADC_Init(ADCx, &adcConfigStrcut);               //����ADC����ģʽ
  ADC_EnableHardwareTrigger(ADCx, true);          //ʹ��Ӳ������ģʽ
  

  /*����ת��ͨ����*/
  
  //����ADCת��ͨ����Ӧ���ⲿ����ͨ����ADC_ETC��صļĴ���ָ��
  adcChannelConfigStruct.channelNumber = DEMO_ADC_ETC_CHANNLE_NUMBER_x; 
  //��ֹת������ж�
  adcChannelConfigStruct.enableInterruptOnConversionCompleted = false;   
  ADC_SetChannelConfig(ADCx, DEMO_ADC_CHANNEL_GROUP0, &adcChannelConfigStruct);
  
  adcChannelConfigStruct.channelNumber = DEMO_ADC_ETC_CHANNLE_NUMBER_x;
  ADC_SetChannelConfig(ADCx, DEMO_ADC_CHANNEL_GROUP1, &adcChannelConfigStruct);


  /*����Ӳ��У׼*/
  while(kStatus_Success != ADC_DoAutoCalibration(ADCx))
  {
    PRINTF("У׼ʧ�� \r\n");
  }
  PRINTF("У׼���\r\n");                                                                                               
}

/*����Ϊ�����ⲿ����*/
void ADC_ETC_Config(void)
{
  adc_etc_config_t adcEtcConfig;//�����ⲿ����ȫ�ֿ�������
  adc_etc_trigger_config_t adcEtcTriggerConfig; //�����ⲿ����ת��ͨ���顣��Ҫ�������ȼ���������ʽ������ͨ������
  adc_etc_trigger_chain_config_t adcEtcTriggerChainConfig; // �����ⲿ����ת��ͨ��������ADCת��ͨ��
  
  
  /*�����ⲿ����������*/
  ADC_ETC_GetDefaultConfig(&adcEtcConfig);
  
  /*���Ҫʹ��ADC2�����Ҫ���ø�����*/
  adcEtcConfig.enableTSCBypass = false;//���ø�λ����ʹ��ADC2
  
  adcEtcConfig.XBARtriggerMask = 1<<ADC_ETC_XBARA_TRIGGER_CHANNELx; /* ����ѡ���˴���ͨ��Ϊ����ͨ��0*/
  ADC_ETC_Init(DEMO_ADC_ETC_BASE, &adcEtcConfig);
  
  
  /*  �����ⲿ����ת��ͨ���顣*/
  adcEtcTriggerConfig.enableSyncMode = false;                      //�첽ģʽ
  adcEtcTriggerConfig.enableSWTriggerMode = false;                 //��ֹ�������
  adcEtcTriggerConfig.triggerChainLength = DEMO_ADC_ETC_CHAIN_LENGTH; //����ת��ͨ��������ʵ��ת��ͨ������ΪDEMO_ADC_ETC_CHAIN_LENGTH+1
  adcEtcTriggerConfig.triggerPriority = 0U;                         //�ⲿ�������ȼ�
  adcEtcTriggerConfig.sampleIntervalDelay = 0U;                                  
  adcEtcTriggerConfig.initialDelay = 0U;                            
  
  ADC_ETC_SetTriggerConfig(DEMO_ADC_ETC_BASE, ADC_ETC_XBARA_TRIGGER_CHANNELx, &adcEtcTriggerConfig);//�����ⲿXBAR����������
  
  
  
  /*�����ⲿ����ת��ͨ��������ADCת��ͨ��*/
  /*****************************************************************************************************************************/
  adcEtcTriggerChainConfig.enableB2BMode = true;                               //ʹ��B2Bģʽ
  adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U<< DEMO_ADC_CHANNEL_GROUP0; //ѡ��Ҫ����ת����ADCת��ͨ��
  adcEtcTriggerChainConfig.ADCChannelSelect = DEMO_ADC_ETC_CHANNLE_NUMBER_0;                               // ѡ���ⲿ����ͨ��
  adcEtcTriggerChainConfig.InterruptEnable = kADC_ETC_Done0InterruptEnable;    // ʹ�ܸ�ͨ����ת������ж�0 
  ADC_ETC_SetTriggerChainConfig(DEMO_ADC_ETC_BASE, ADC_ETC_XBARA_TRIGGER_CHANNELx, 0U, &adcEtcTriggerChainConfig); //
 /*****************************************************************************************************************************/


  /****************************************************************************************************************************/                             
  adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U<< DEMO_ADC_CHANNEL_GROUP1;         //ѡ��Ҫ����ת����ADCת��ͨ��
  adcEtcTriggerChainConfig.ADCChannelSelect = DEMO_ADC_ETC_CHANNLE_NUMBER_15;                                      //ѡ���ⲿ����ͨ��
  adcEtcTriggerChainConfig.InterruptEnable = kADC_ETC_Done1InterruptEnable;            //ʹ�ܵ�ǰͨ��ת������ж�1.
  ADC_ETC_SetTriggerChainConfig(DEMO_ADC_ETC_BASE, ADC_ETC_XBARA_TRIGGER_CHANNELx, 1U,&adcEtcTriggerChainConfig);  
  /********************************************************************************************************************************/

  
  /*�����ж����ȼ�,*/
  set_IRQn_Priority(ADC_ETC_IRQ1_IRQn,Group4_PreemptPriority_6, Group4_SubPriority_0);
  set_IRQn_Priority(ADC_ETC_IRQ0_IRQn,Group4_PreemptPriority_6, Group4_SubPriority_1);
  
  /*ʹ���ж� NVIC. */
  EnableIRQ(ADC_ETC_IRQ1_IRQn);
  EnableIRQ(ADC_ETC_IRQ0_IRQn);

}



/**
  * @brief  ��ʼ������ADC��ADC_ETC
  * @param  ��
  * @retval ��
  */
void ADC_Config(void)
{
  
  ADC_IOMUXC_MUX_Config();//�������Ź���
  ADC_IOMUXC_PAD_Config();//�����������ò���������ǿ�ȡ�ת�����ʵ�
  ADC_IO_Mode_Config();   //��������ģʽ������/������Ƿ����жϵ�
  ADC_Mode_Config();      //��ʼ��ADC����ģʽ�����ҽ���Ӳ��У׼��
  
  ADC_ETC_Config();       //�����ⲿ����������

}

/*�жϷ�����*/
void EXAMPLE_ADC_ETC_DONE0_Handler(void)
{
  /*���ת������жϱ�־λ*/
  ADC_ETC_ClearInterruptStatusFlags(DEMO_ADC_ETC_BASE, (adc_etc_external_trigger_source_t)ADC_ETC_XBARA_TRIGGER_CHANNELx, kADC_ETC_Done0StatusFlagMask);
  /*��ȡת�����*/
  g_AdcConversionValue0 = ADC_ETC_GetADCConversionValue(DEMO_ADC_ETC_BASE, ADC_ETC_XBARA_TRIGGER_CHANNELx, 0U); /* Get trigger0 chain0 result. */
  b_Value0_Conversion_complete_flag = true;
}

void EXAMPLE_ADC_ETC_DONE1_Handler(void)
{
  /*���ת������жϱ�־λ*/
  ADC_ETC_ClearInterruptStatusFlags(DEMO_ADC_ETC_BASE, (adc_etc_external_trigger_source_t)ADC_ETC_XBARA_TRIGGER_CHANNELx, kADC_ETC_Done1StatusFlagMask);
  /*��ȡת�����*/
  g_AdcConversionValue1 = ADC_ETC_GetADCConversionValue(DEMO_ADC_ETC_BASE, ADC_ETC_XBARA_TRIGGER_CHANNELx, 1U); /* Get trigger0 chain1 result. */ 
  b_Value1_Conversion_complete_flag = true;
}










