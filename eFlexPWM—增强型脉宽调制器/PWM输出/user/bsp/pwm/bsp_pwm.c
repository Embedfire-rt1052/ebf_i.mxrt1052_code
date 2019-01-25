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


void PWM_config(void)
{
  /* Structure of initialize PWM */
  pwm_config_t pwmConfig;
  
  /*����ʱ��*/
  CLOCK_SetDiv(kCLOCK_AhbDiv, 0x2); /* Set AHB PODF to 2, divide by 3 */
  CLOCK_SetDiv(kCLOCK_IpgDiv, 0x3); /* Set IPG PODF to 3, divede by 4 */
  
  
   /* Set the PWM Fault inputs to a low value */
//   XBARA_Init(XBARA1);
//   XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1Fault0);
//   XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1Fault1);
//   XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1234Fault2);
//   XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1234Fault3); 
   
   
   
//   void PWM_GetDefaultConfig(pwm_config_t *config)
//{
//    assert(config);
//
//    /* PWM is paused in debug mode ʹ�ܹ�����DEBUGģʽ*/
//    config->enableDebugMode = false;   
//    /* PWM is paused in wait mode  ��ֹ�����ڵȴ�ģʽ*/
//    config->enableWait = false;
//    /* PWM module uses the local reload signal to reload registers PWMģ��ʹ�ñ��������ź�����pwm*/
//    config->reloadSelect = kPWM_LocalReload;
//    /* Fault filter count is set to 0   ����ɸѡ����������Ϊ0*/
//    config->faultFilterCount = 0;
//    /* Fault filter period is set to 0 which disables the fault filter �����˲�Ƶ������Ϊ0����ʹ�ù����˲�*/
//    config->faultFilterPeriod = 0;
//    /* Use the IP Bus clock as source clock for the PWM submodule  ѡ��ʱ��Դ*/
//    config->clockSource = kPWM_BusClock;
//    /* Clock source prescale is set to divide by 1     ����ʱ�ӷ�Ƶ*/
//    config->prescale = kPWM_Prescale_Divide_1;
//    /* Local sync causes initialization                 ����ͬ���������³�ʼ��*/
//    config->initializationControl = kPWM_Initialize_LocalSync;
//    /* The local force signal, CTRL2[FORCE], from the submodule is used to force updates ѡ��ǿ�Ƹ����ź� */
//    config->forceTrigger = kPWM_Force_Local;
//    /* PWM reload frequency, reload opportunity is PWM half cycle or full cycle. ѡ��PWM�Զ���װƵ��
//     * This field is not used in Immediate reload mode
//     */
//    config->reloadFrequency = kPWM_LoadEveryOportunity;
//    /* Buffered-registers get loaded with new values as soon as LDOK bit is set һ��������LDOKλ������Ĵ����ͻ�����µ�ֵ */
//    config->reloadLogic = kPWM_ReloadImmediate;
//    /* PWM A & PWM B operate as 2 independent channels */
//    config->pairOperation = kPWM_Independent;
//}
   
   
   PWM_GetDefaultConfig(&pwmConfig);
   /* Use full cycle reload */
   pwmConfig.reloadLogic = kPWM_ReloadPwmFullCycle; 
   /* PWM A & PWM B form a complementary PWM pair */
   pwmConfig.pairOperation = kPWM_ComplementaryPwmA;   
   pwmConfig.enableDebugMode = true;
   
   /* Initialize submodule 0 */
    if (PWM_Init(BOARD_PWM_BASEADDR, kPWM_Module_0, &pwmConfig) == kStatus_Fail)
    {
        PRINTF("PWM initialization failed\n");
        //return 1;
    }

    /* Initialize submodule 1 */
    pwmConfig.clockSource = kPWM_Submodule0Clock;
    pwmConfig.initializationControl = kPWM_Initialize_MasterSync;
    if (PWM_Init(BOARD_PWM_BASEADDR, kPWM_Module_1, &pwmConfig) == kStatus_Fail)
    {
        PRINTF("PWM initialization failed\n");
        //return 1;
    }
   /* Call the init function with demo configuration */
    PWM_DRV_Init3PhPwm();

    /* Set the load okay bit for all submodules to load registers from their buffer */
    PWM_SetPwmLdok(BOARD_PWM_BASEADDR, kPWM_Control_Module_0 | kPWM_Control_Module_1 | kPWM_Control_Module_2, true);

    /* Start the PWM generation from Submodules 0, 1 and 2 */
    PWM_StartTimer(BOARD_PWM_BASEADDR, kPWM_Control_Module_0 | kPWM_Control_Module_1 | kPWM_Control_Module_2);

 
}




/*PWM ��ʼ������*/
static void PWM_DRV_Init3PhPwm(void)
{
    uint16_t deadTimeVal;
    pwm_signal_param_t pwmSignal[2];
    uint32_t pwmSourceClockInHz;
    uint32_t pwmFrequencyInHz = 1000;

    pwmSourceClockInHz = PWM_SRC_CLK_FREQ;

    /* Set deadtime count, we set this to about 650ns */
    deadTimeVal = ((uint64_t)pwmSourceClockInHz * 650000) / 1000000000;

    
//    typedef struct _pwm_signal_param
//{
//    pwm_channels_t pwmChannel; /*!< PWM channel being configured; PWM A or PWM B */
//    uint8_t dutyCyclePercent;  /*!< PWM pulse width, value should be between 0 to 100
//                                    0=inactive signal(0% duty cycle)...
//                                    100=always active signal (100% duty cycle)*/
//    pwm_level_select_t level;  /*!< PWM output active level select */
//    uint16_t deadtimeValue;    /*!< The deadtime value; only used if channel pair is operating in complementary mode */
//} pwm_signal_param_t;
    pwmSignal[0].pwmChannel = kPWM_PwmA;
    pwmSignal[0].level = kPWM_HighTrue;
    pwmSignal[0].dutyCyclePercent = 50; /* 1 percent dutycycle */
    pwmSignal[0].deadtimeValue = deadTimeVal;

    pwmSignal[1].pwmChannel = kPWM_PwmB;
    pwmSignal[1].level = kPWM_HighTrue;
    /* Dutycycle field of PWM B does not matter as we are running in PWM A complementary mode */
    pwmSignal[1].dutyCyclePercent = 50;
    pwmSignal[1].deadtimeValue = deadTimeVal;

    /*********** PWMA_SM0 - phase A, configuration, setup 2 channel as an example ************/
    PWM_SetupPwm(BOARD_PWM_BASEADDR, kPWM_Module_0, pwmSignal, 2, kPWM_SignedCenterAligned, pwmFrequencyInHz,
                 pwmSourceClockInHz);

    /*********** PWMA_SM1 - phase B configuration, setup PWM A channel only ************/
    PWM_SetupPwm(BOARD_PWM_BASEADDR, kPWM_Module_1, pwmSignal, 1, kPWM_SignedCenterAligned, pwmFrequencyInHz,
                 pwmSourceClockInHz);

    /*********** PWMA_SM2 - phase C configuration, setup PWM A channel only ************/
    PWM_SetupPwm(BOARD_PWM_BASEADDR, kPWM_Module_2, pwmSignal, 1, kPWM_SignedCenterAligned, pwmFrequencyInHz,
                 pwmSourceClockInHz);
}

