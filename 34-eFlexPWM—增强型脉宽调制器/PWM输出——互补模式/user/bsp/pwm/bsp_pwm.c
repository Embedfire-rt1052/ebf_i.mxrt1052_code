#include "fsl_debug_console.h"
#include "./bsp/pwm/bsp_pwm.h"

/**
* @brief  ��ʼ�� PWM ʹ�õ����ⲿ����  
* @retval ��
*/
void PWM_gpio_config(void)
{
  gpio_pin_config_t PWM_pin_config;
  
   /*�����ⲿ���ŵĸ��ù���*/
  IOMUXC_SetPinMux(PWM1_PWMA00_IOMUXC, 0U);                                      
  IOMUXC_SetPinMux(PWM1_PWMB00_IOMUXC, 0U); 

  /*�������ŵ� pad ���� */
  IOMUXC_SetPinConfig(PWM1_PWMA00_IOMUXC, PWM_PAD_CONFIG_DATA);
  IOMUXC_SetPinConfig(PWM1_PWMB00_IOMUXC, PWM_PAD_CONFIG_DATA);

  /*��������Ϊ���ģʽ��Ĭ�ϵ�ƽλ�ߵ�ƽ*/
  PWM_pin_config.direction = kGPIO_DigitalOutput;
  PWM_pin_config.outputLogic = 1;
  PWM_pin_config.interruptMode = kGPIO_NoIntmode;
   
  GPIO_PinInit(PWM1_PWMA00_GPIO, PWM1_PWMA00_GPIO_PIN, &PWM_pin_config);
  GPIO_PinInit(PWM1_PWMB00_GPIO, PWM1_PWMB00_GPIO_PIN, &PWM_pin_config);

}
  
 /**
* @brief  ��ʼ�� PWM  ���ò��� 
* @retval ��
*/
void PWM_config(void)
{
  uint32_t pwmSourceClockInHz;     //���ڱ������Ƶ�� 
  uint16_t deadTimeVal = 0;        //���ڱ�����������ֵ
  pwm_signal_param_t pwmSignal[2]; //�������� pwm �Ĳ�����Ƶ�ʡ����ڵ�
  pwm_config_t pwmConfig;          //����pwm ���ýṹ��
  
  


  /* ����IPG clock ʱ��Ƶ�ʣ����ֻ��ʵ���ķ�Ƶ*/
  CLOCK_SetDiv(kCLOCK_IpgDiv, 0x3); /* Set IPG PODF to 3, divede by 4 */
  

   /*����pwm ��������Ϊ�ߵ�ƽ����ʾû�д���ֻ�е�pwm û�д���������߽�ֹ����������������pwm��*/
   XBARA_Init(XBARA1);
   XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1Fault0);
   XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1Fault1);
   XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1234Fault2);
   XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1234Fault3); 
          
   /*��ȡĬ�ϵ����ã�Ĭ����������
    *pwmConfig->enableDebugMode = false;   //��ֹ��DEBUGģʽ
    *config->enableWait = false;           //��ֹ�����ڵȴ�ģʽ
    *config->reloadSelect = kPWM_LocalReload;//PWMģ��ʹ�ñ��������ź�����pwm
    *config->faultFilterCount = 0;          //����ɸѡ����������Ϊ0
    *config->faultFilterPeriod = 0;         //�����˲�Ƶ������Ϊ0����ʹ�ù����˲�
    *config->clockSource = kPWM_BusClock;   //ѡ��ʱ��Դ
    *config->prescale = kPWM_Prescale_Divide_1;//����ʱ�ӷ�Ƶ
    *config->initializationControl = kPWM_Initialize_LocalSync;//����ͬ���źų�ʼ��PWM
    *config->forceTrigger = kPWM_Force_Local;  //ѡ��ǿ�Ƹ����ź�
    *config->reloadFrequency = kPWM_LoadEveryOportunity;//ѡ��PWM�Զ���װƵ��
    *config->reloadLogic = kPWM_ReloadImmediate;//һ��������LDOKλ������Ĵ������������µ�ֵ 
    *config->pairOperation = kPWM_Independent;  //�����ڶ���ģʽ
   */
   PWM_GetDefaultConfig(&pwmConfig);
   
   /* �޸�Ĭ�����ò��� */
   pwmConfig.reloadLogic = kPWM_ReloadPwmFullCycle; //��ֵ����һ��pwm�����������֮����ص�����Ĵ�����
   pwmConfig.pairOperation = kPWM_ComplementaryPwmA;// PwmA �� PwmB ��Ϊ����ͨ����PwmA ��Ϊ��ͨ��
   pwmConfig.prescale = kPWM_Prescale_Divide_128;
   pwmConfig.enableDebugMode = true;                // ʹ��DebugMode 
   
   /* ��ʼ�� PWM �����жϳ�ʼ���Ƿ�ɹ�*/
    if (PWM_Init(BOARD_PWM_BASEADDR, kPWM_Module_0, &pwmConfig) == kStatus_Fail)
    {
        PRINTF("PWM initialization failed\n");
    }
   
    /*��õ�ǰ����Ƶ��*/
    pwmSourceClockInHz = PWM_SRC_CLK_FREQ;
    /* ������ʱ��ת��Ϊpwmʱ��Դ��ʱ�Ӹ���*/
    deadTimeVal = ((uint64_t)pwmSourceClockInHz * PWM_deadtime) / 1000000000;
   
    /*����pwm ����*/
    pwmSignal[0].pwmChannel = kPWM_PwmA; //ָ��pwm ͨ��
    pwmSignal[0].level = kPWM_HighTrue;  //����������� 
    pwmSignal[0].dutyCyclePercent = PWM_duty_Cycle_Percent;  //���� pwm ��ռ�ձ�   
    pwmSignal[0].deadtimeValue = deadTimeVal; //�趨����ֵ

    pwmSignal[1].pwmChannel = kPWM_PwmB;        
    pwmSignal[1].level = kPWM_HighTrue;
    pwmSignal[1].dutyCyclePercent = PWM_duty_Cycle_Percent;
    pwmSignal[1].deadtimeValue = deadTimeVal;
    

    
    /*����pwm ����������pwm ��Ƶ�� ռ�ձȣ�����ʱ���*/
    PWM_SetupPwm(BOARD_PWM_BASEADDR, kPWM_Module_0, pwmSignal, 2, kPWM_SignedEdgeAligned, PWM_frequency_Hz,
                 pwmSourceClockInHz);

    /*����Set LDOK λ������ʼ���������ص���Ӧ�ļĴ���*/
    PWM_SetPwmLdok(BOARD_PWM_BASEADDR, kPWM_Control_Module_0 , true);

    /*����pwm1 ��ģ��0(Submodules 0) ��pwm���*/
    PWM_StartTimer(BOARD_PWM_BASEADDR, kPWM_Control_Module_0 );
}

