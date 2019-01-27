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
  IOMUXC_SetPinMux(PWM1_PWMA01_IOMUXC, 0U);                                      
  IOMUXC_SetPinMux(PWM1_PWMA02_IOMUXC, 0U); 
  
  /*�������ŵ� pad ���� */
  IOMUXC_SetPinConfig(PWM1_PWMA00_IOMUXC, PWM_PAD_CONFIG_DATA);
  IOMUXC_SetPinConfig(PWM1_PWMB00_IOMUXC, PWM_PAD_CONFIG_DATA);
  IOMUXC_SetPinConfig(PWM1_PWMA01_IOMUXC, PWM_PAD_CONFIG_DATA);
  IOMUXC_SetPinConfig(PWM1_PWMA02_IOMUXC, PWM_PAD_CONFIG_DATA);
  
  /*��������Ϊ���ģʽ��Ĭ�ϵ�ƽλ�ߵ�ƽ*/
  PWM_pin_config.direction = kGPIO_DigitalOutput;
  PWM_pin_config.outputLogic = 1;
  PWM_pin_config.interruptMode = kGPIO_NoIntmode;
   
  GPIO_PinInit(PWM1_PWMA00_GPIO, PWM1_PWMA00_GPIO_PIN, &PWM_pin_config);
  GPIO_PinInit(PWM1_PWMB00_GPIO, PWM1_PWMB00_GPIO_PIN, &PWM_pin_config);
  GPIO_PinInit(PWM1_PWMA01_GPIO, PWM1_PWMA01_GPIO_PIN, &PWM_pin_config);
  GPIO_PinInit(PWM1_PWMA02_GPIO, PWM1_PWMA02_GPIO_PIN, &PWM_pin_config);
}

 /**
* @brief  ��ʼ�� PWM  ���ò��� 
* @retval ��
*/
void PWM_config(void)
{
  uint32_t pwmSourceClockInHz;  //���ڱ������Ƶ�� 
  uint16_t deadTimeVal = 0;     //���ڱ�����������ֵ��������ͨ�����û���õ���������Ϊ0����
  pwm_config_t pwmConfig;       //����pwm ��ʼ���ṹ�� ��������PWM����ģʽ
  pwm_signal_param_t pwmSignal; //�������� pwm �Ĳ�����Ƶ�ʡ����ڵ�
  

  
  /*����AHB����ʱ�Ӻ�IP����ʱ��*/
  CLOCK_SetDiv(kCLOCK_AhbDiv, 0x2); /* Set AHB PODF to 2, divide by 3 */
  CLOCK_SetDiv(kCLOCK_IpgDiv, 0x3); /* Set IPG PODF to 3, divede by 4 */
  /*��ֹ�������*/
  PWM1->SM[0].DISMAP[0]=0;
  PWM1->SM[1].DISMAP[0]=0;
//  BOARD_PWM_BASEADDR->SM[kPWM_Control_Module_0].DISMAP[0]=0;

   
   /*���� PWM ��ʼ������*/
   PWM_GetDefaultConfig(&pwmConfig);
   pwmConfig.reloadLogic = kPWM_ReloadPwmFullCycle; //������������
   pwmConfig.pairOperation = kPWM_Independent;      //����λ����ģʽ 
   pwmConfig.enableDebugMode = true;                //ʹ�� Debug ģʽ
   /* ��ʼ��PWM1 ����ģ��0 ��submodule 0�� */
   if (PWM_Init(BOARD_PWM_BASEADDR, kPWM_Module, &pwmConfig) == kStatus_Fail)
   {
       PRINTF("PWM initialization failed\n");        
   }

    
    pwmSignal.pwmChannel = PWM_A_or_B;    //ѡ��PWM ͨ�� PWMA ����PWMB
    pwmSignal.level  =     kPWM_HighTrue; //ѡ���������
    pwmSignal.dutyCyclePercent = PWM_duty_Cycle_Percent;     //����ռ�ձ�   
    pwmSignal.deadtimeValue = deadTimeVal;//��������ֵ���ڸ�ģʽ��û��ʹ�õ���������Ϊ0����
    /*��õ�ǰ����Ƶ��
    *ע��:��ȡʱ��Ƶ��֮��Ҫ���� pwm ��ʼ��֮�󣬼�����PWM_Init()֮��
    *��Ϊ����ʱ�ӷ�Ƶ֮���ȡ����Ƶ�ʲ���PWM ����Ƶ�ʣ�����PWM_SetupPwm�������һ��������pwm����Ƶ��
    *������pwmģ��ʱ��Դ��Ƶ�ʡ�PWMʱ��Դ������Ƶ��õ�pwm ����Ƶ��
    */
    pwmSourceClockInHz = PWM_SRC_CLK_FREQ;
    /*����pwm ����*/
    PWM_SetupPwm(BOARD_PWM_BASEADDR, kPWM_Module, &pwmSignal, 1, kPWM_SignedEdgeAligned, PWM_frequency_Hz,
                 pwmSourceClockInHz);
    
    
   /* ���ôӸ��ԵĻ������������� */
   PWM_SetPwmLdok(BOARD_PWM_BASEADDR,kPWM_Control_Module , true);

   /* ����PWM ���*/
   PWM_StartTimer(BOARD_PWM_BASEADDR,  kPWM_Control_Module);

 
}










