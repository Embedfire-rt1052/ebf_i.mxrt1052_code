
#include "fsl_debug_console.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"  
#include "fsl_adc.h"
  
#include "pad_config.h"  
#include "./adc/bsp_adc.h" 

extern volatile bool ADC_ConversionDoneFlag;
extern volatile uint32_t ADC_ConvertedValue;

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

  
   /** ADC��GPIO���� **/   
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
  adc_config_t adcConfigStrcut; //����ADC ģʽ���ýṹ��
  adc_channel_config_t adcChannelConfigStruct; //ADC ͨ�����ýṹ��
  
  ADC_GetDefaultConfig(&adcConfigStrcut); //��ȡADC Ĭ�Ϲ���ģʽ
  adcConfigStrcut.resolution = kADC_Resolution12Bit;
  ADC_Init(ADCx, &adcConfigStrcut); //����ADC����ģʽ
  
  adcChannelConfigStruct.channelNumber = DEMO_ADC_USER_CHANNEL;
  adcChannelConfigStruct.enableInterruptOnConversionCompleted = false; //��ֹת������ж�
  
  /*����ת��ͨ����ת��ͨ����֮��Ĺ����������һ���漰ת��ͨ����*/
  ADC_SetChannelConfig(ADCx, DEMO_ADC_CHANNEL_GROUP0, &adcChannelConfigStruct);
  ADC_SetChannelConfig(ADCx, DEMO_ADC_CHANNEL_GROUP1, &adcChannelConfigStruct);
  
  ///*����ADC��Ӳ����ƽ��ֵ*/
  //ADC_SetHardwareAverageConfig(ADCx, kADC_HardwareAverageCount32);
  
  /*����Ӳ��У׼*/
  if (kStatus_Success == ADC_DoAutoCalibration(ADCx))
  {
     PRINTF("У׼��� Done.\r\n");
  }
  else
  {
     PRINTF("У׼ʧ��\r\n");
  }
  

}

/*����Ϊ�����ⲿ����*/
void ADC_ETC_Config(void)
{
  adc_etc_config_t adcEtcConfig;//�ⲿ����
  adc_etc_trigger_config_t adcEtcTriggerConfig; //adc �ⲿ��������
  adc_etc_trigger_chain_config_t adcEtcTriggerChainConfig; // ADC ����������
  
  ADC_ETC_GetDefaultConfig(&adcEtcConfig);
  adcEtcConfig.XBARtriggerMask = 1U; /* Enable the external XBAR trigger0.�����ⲿ����*/
  ADC_ETC_Init(DEMO_ADC_ETC_BASE, &adcEtcConfig);
  
  
  /* 
   *Set the external XBAR trigger0 configuration. 
   *����ADC�ⲿ����Դ���ԡ�
  */
  adcEtcTriggerConfig.enableSyncMode = false;
  adcEtcTriggerConfig.enableSWTriggerMode = false;
  adcEtcTriggerConfig.triggerChainLength = DEMO_ADC_ETC_CHAIN_LENGTH; /* Chain length is 2. */
  adcEtcTriggerConfig.triggerPriority = 0U;
  adcEtcTriggerConfig.sampleIntervalDelay = 0U;
  adcEtcTriggerConfig.initialDelay = 0U;
  //DEMO_ADC_ETC_BASE ����������ַ���������� ����������
  ADC_ETC_SetTriggerConfig(DEMO_ADC_ETC_BASE, 0U, &adcEtcTriggerConfig);
  
  
      /*************************************************************************************************************************/
    adcEtcTriggerChainConfig.enableB2BMode = true;
    adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U
                                                   << DEMO_ADC_CHANNEL_GROUP0; /* Select ADC_HC0 register to trigger. �����������˴���Դ*/
    adcEtcTriggerChainConfig.ADCChannelSelect =
        DEMO_ADC_ETC_CHANNEL0; /* ADC_HC0 will be triggered to sample Corresponding channel. */
    adcEtcTriggerChainConfig.InterruptEnable = kADC_ETC_Done0InterruptEnable; /* Enable the Done0 interrupt. */
    ADC_ETC_SetTriggerChainConfig(DEMO_ADC_ETC_BASE, 0U, 0U,
                                  &adcEtcTriggerChainConfig); /* Configure the trigger0 chain0. */
   /*****************************************************************************************************************************/


    /****************************************************************************************************************************/                             
    adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U
                                                   << DEMO_ADC_CHANNEL_GROUP1; /* Select ADC_HC1 register to trigger. */
    adcEtcTriggerChainConfig.ADCChannelSelect =
        DEMO_ADC_ETC_CHANNEL1; /* ADC_HC1 will be triggered to sample Corresponding channel. */
    adcEtcTriggerChainConfig.InterruptEnable = kADC_ETC_Done1InterruptEnable; /* Enable the Done1 interrupt. */
    ADC_ETC_SetTriggerChainConfig(DEMO_ADC_ETC_BASE, 0U, 1U,
                                  &adcEtcTriggerChainConfig); /* Configure the trigger0 chain1. */
    /********************************************************************************************************************************/




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
  ADC_IO_Mode_Config(); //��������ģʽ������/������Ƿ����жϵ�
  ADC_Mode_Config(); //��ʼ��ADC����ģʽ�����ҽ���Ӳ��У׼��
  
  /*�����ж�*/
  EnableIRQ(ADC_IRQ); // ����ADC �жϡ�
}


/*ADC�жϷ�����*/
void ADC_IRQHandler(void)
{
  /*����ADCת����ɱ�־*/
  ADC_ConversionDoneFlag = true;
  /*��ȡת���������ȡ֮��Ӳ���Զ����ת������жϱ�־λ*/
  ADC_ConvertedValue = ADC_GetChannelConversionValue(ADCx, DEMO_ADC_CHANNEL_GROUP);
}

