#include "fsl_gpio.h"
#include "pad_config.h"
#include "./tmr/bsp_tmr.h"
#include "fsl_debug_console.h"

/**
* @brief  ����TMR������Ź���
* @param  ��
* @retval ��
*/
void TMR_GPIO_Config(void)
{
    /*����GPIO�������ýṹ��*/
  gpio_pin_config_t gpt_config; 
  
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_00_QTIMER3_TIMER0, 0U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_00_QTIMER3_TIMER0, TMR_INPUT_PAD_CONFIG_DATA);
  
  
  gpt_config.direction = kGPIO_DigitalOutput; //����ģʽ
  gpt_config.outputLogic =  1;                //Ĭ�ϸߵ�ƽ
  gpt_config.interruptMode = kGPIO_NoIntmode; //��ʹ���ж�
  
  /* ��ʼ��  GPIO. */
  GPIO_PinInit(TMR_TIMER0_INPUT_GPIO, TMR_TIMER0_INPUT_GPIO_PIN, &gpt_config);
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
  QTMR_Init(BOARD_QTMR_BASEADDR, BOARD_QTMR_PWM_CHANNEL,  &qtmrConfig);

  /* ����һ��PWM���������ָ��PWM��Ƶ����ռ�ձ�*/
  if((QTMR_SetupPwm(BOARD_QTMR_BASEADDR, BOARD_QTMR_PWM_CHANNEL, TMR3_CH0_PWM_FREQUENCY, TMR3_CH0_PWM_DUTYCYCLE, false, QTMR_SOURCE_CLOCK / 8)) != kStatus_Success)
  {
    while(1)
    {
      PRINTF("error");
    }
  }
  
  /* ��ʼ����������������ʱ�ӵ������ؼ��� */
  QTMR_StartTimer(BOARD_QTMR_BASEADDR, BOARD_QTMR_PWM_CHANNEL, kQTMR_PriSrcRiseEdge);
}











