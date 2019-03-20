
#include "fsl_debug_console.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"  
#include "fsl_adc.h"
  
#include "pad_config.h" 
#include "./bsp/nvic/bsp_nvic.h"
#include "./bsp/adc/bsp_adc.h" 

extern volatile bool ADC_ConversionDoneFlag;
extern volatile uint32_t ADC_ConvertedValue;

/**
* @brief  ��ʼ��ADC���IOMUXC��MUX��������
* @param  ��
* @retval ��
*/
static void ADC_IOMUXC_MUX_Config(void)
{
  IOMUXC_SetPinMux(CORE_BOARD_ADC_IOMUXC, 0U);  
}
/**
* @brief  ��ʼ��ADC���IOMUXC��MUX��������
* @param  ��
* @retval ��
*/
static void ADC_IOMUXC_PAD_Config(void)
{
  IOMUXC_SetPinConfig(CORE_BOARD_ADC_IOMUXC, ADC_PAD_CONFIG_DATA);   
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
   /*ADC��GPIO����*/   
  adc_config.direction = kGPIO_DigitalInput; //����ģʽ
//adc_config.outputLogic =  1;                //Ĭ�ϸߵ�ƽ��������ģʽ�����ø�ѡ����Ч
  adc_config.interruptMode = kGPIO_NoIntmode; //��ʹ���ж�
  
  GPIO_PinInit(CORE_BOARD_ADC_GPIO, CORE_BOARD_ADC_GPIO_PIN, &adc_config);
}


 /**
  * @brief  ��ʼ��ADC����ģʽ
  * @param  ��
  * @retval ��
  */
static void ADC_Mode_Config(void)
{
  adc_config_t adcConfigStrcut; //����ADC ģʽ���ýṹ��
  
  /*��ʼ��ADC����ģʽ*/
  adc_channel_config_t adcChannelConfigStruct;    //ADC ͨ�����ýṹ��
  ADC_GetDefaultConfig(&adcConfigStrcut); //��ȡADC Ĭ�Ϲ���ģʽ
  adcConfigStrcut.resolution = kADC_Resolution12Bit;
  ADC_Init(ADCx, &adcConfigStrcut); //����ADC����ģʽ
  
  /*����ADC��Ӳ����ƽ��ֵ*/
  ADC_SetHardwareAverageConfig(ADCx, kADC_HardwareAverageCount32);
  
  /*ADCת��ͨ������*/
  adcChannelConfigStruct.channelNumber = DEMO_ADC_USER_CHANNEL;//����ADCת��ͨ����Ӧ���ⲿ����ͨ��
  adcChannelConfigStruct.enableInterruptOnConversionCompleted = true; //ʹ��ת������ж�
  ADC_SetChannelConfig(ADCx, DEMO_ADC_CHANNEL_GROUP, &adcChannelConfigStruct);

  /*����Ӳ��У׼*/
  if (kStatus_Success == ADC_DoAutoCalibration(ADCx))
  {
     PRINTF("У׼��� Done.\r\n");
  }
  else
  {
     PRINTF("У׼ʧ��\r\n");
  }
  
  
  /*�����ж����ȼ�,*/
  set_IRQn_Priority(ADC_IRQ,Group4_PreemptPriority_6, Group4_SubPriority_0);
  /*�����ж�*/
  EnableIRQ(ADC_IRQ); // ����ADC �жϡ�
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
  
  
}


/*ADC�жϷ�����*/
void ADC_IRQHandler(void)
{
  /*����ADCת����ɱ�־*/
  ADC_ConversionDoneFlag = true;
  /*��ȡת���������ȡ֮��Ӳ���Զ����ת������жϱ�־λ*/
  ADC_ConvertedValue = ADC_GetChannelConversionValue(ADCx, DEMO_ADC_CHANNEL_GROUP);
}









