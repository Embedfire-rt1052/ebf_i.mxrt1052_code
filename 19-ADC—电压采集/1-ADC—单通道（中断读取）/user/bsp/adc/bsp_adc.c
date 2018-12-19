
#include "fsl_debug_console.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"  
#include "fsl_adc.h"
  
#include "pad_config.h"  
#include "./adc/bsp_adc.h" 

extern volatile bool g_AdcConversionDoneFlag;
extern volatile uint32_t g_AdcConversionValue;
extern volatile uint32_t g_AdcInterruptCounter;
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
  /*����ADCͨ�����ýṹ��*/
  adc_channel_config_t adcChannelConfigStruct;
  
   /** ADC��GPIO���� **/   
  adc_config.direction = kGPIO_DigitalInput; //����ģʽ
//  adc_config.outputLogic =  1;                //Ĭ�ϸߵ�ƽ
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

  
  ADC_GetDefaultConfig(&adcConfigStrcut); //��ȡADC Ĭ�Ϲ���ģʽ
  ADC_Init(ADCx, &adcConfigStrcut); //����ADC����ģʽ
  
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

 /**
  * @brief  ����ת��������ӡת�����
  * @param  ��
  * @retval ��
  */
static void ADC_start(void)
{
  adc_channel_config_t adcChannelConfigStruct;//����ADC ͨ�����ýṹ��
    /*�����û�ͨ��*/
  adcChannelConfigStruct.channelNumber = DEMO_ADC_USER_CHANNEL;
  adcChannelConfigStruct.enableInterruptOnConversionCompleted = true;
  while (1)
  {
//      PRINTF("Press any key to get user channel's ADC value.\r\n");
//      GETCHAR();
      g_AdcConversionDoneFlag = false;
      /*
          When in software trigger mode, each conversion would be launched once calling the "ADC16_ChannelConfigure()"
          function, which works like writing a conversion command and executing it. For another channel's conversion,
          just to change the "channelNumber" field in channel configuration structure, and call the function
          "ADC_ChannelConfigure()"" again.
          Also, the "enableInterruptOnConversionCompleted" inside the channel configuration structure is a parameter
         for
          the conversion command. It takes affect just for the current conversion. If the interrupt is still required
          for the following conversion, it is necessary to assert the "enableInterruptOnConversionCompleted" every
         time
          for each command.
         */
      ADC_SetChannelConfig(ADCx, DEMO_ADC_CHANNEL_GROUP, &adcChannelConfigStruct);
      while (g_AdcConversionDoneFlag == false)
      {
       }
      PRINTF("ADC Value: %d\r\n", g_AdcConversionValue);
      //PRINTF("ADC Interrupt Counter: %d\r\n", g_AdcInterruptCounter);
  }
}


/**
  * @brief  ��ʼ������ADC��IO
  * @param  ��
  * @retval ��
  */
void ADC_Config(void)
{
  ADC_IOMUXC_MUX_Config();
  ADC_IOMUXC_PAD_Config();
  ADC_IO_Mode_Config();
  
  /*�����ж�*/
  EnableIRQ(ADC_IRQ);
}


void ADC_IRQHandler(void)
{
    g_AdcConversionDoneFlag = true;
    /* Read conversion result to clear the conversion completed flag. */
    g_AdcConversionValue = ADC_GetChannelConversionValue(ADCx, DEMO_ADC_CHANNEL_GROUP);
    //g_AdcInterruptCounter++;
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}