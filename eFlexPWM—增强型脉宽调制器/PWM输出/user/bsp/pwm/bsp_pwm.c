#include "fsl_debug_console.h"
 




#include "./bsp/pwm/bsp_pwm.h"
  /**
* @brief  初始化 PWM 使用到的外部引脚  
* @retval 无
*/
void PWM_gpio_config(void)
{
  gpio_pin_config_t PWM_pin_config;
  
   /*设置外部引脚的复用功能*/
  IOMUXC_SetPinMux(PWM1_PWMA00_IOMUXC, 0U);                                      
  IOMUXC_SetPinMux(PWM1_PWMB00_IOMUXC, 0U); 
  IOMUXC_SetPinMux(PWM1_PWMA01_IOMUXC, 0U);                                      
  IOMUXC_SetPinMux(PWM1_PWMA02_IOMUXC, 0U); 
  
  /*设置引脚的 pad 属性 */
  IOMUXC_SetPinConfig(PWM1_PWMA00_IOMUXC, PWM_PAD_CONFIG_DATA);
  IOMUXC_SetPinConfig(PWM1_PWMB00_IOMUXC, PWM_PAD_CONFIG_DATA);
  IOMUXC_SetPinConfig(PWM1_PWMA01_IOMUXC, PWM_PAD_CONFIG_DATA);
  IOMUXC_SetPinConfig(PWM1_PWMA02_IOMUXC, PWM_PAD_CONFIG_DATA);
  
  /*配置引脚为输出模式，默认电平位高电平*/
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
  
  /*设置时钟*/
  CLOCK_SetDiv(kCLOCK_AhbDiv, 0x2); /* Set AHB PODF to 2, divide by 3 */
  CLOCK_SetDiv(kCLOCK_IpgDiv, 0x3); /* Set IPG PODF to 3, divede by 4 */
  
  
   /* Set the PWM Fault inputs to a low value */
   XBARA_Init(XBARA1);
   XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1Fault0);
   XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1Fault1);
   XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1234Fault2);
   XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1234Fault3); 
   
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




/*PWM 初始化函数*/
static void PWM_DRV_Init3PhPwm(void)
{
    uint16_t deadTimeVal;
    pwm_signal_param_t pwmSignal[2];
    uint32_t pwmSourceClockInHz;
    uint32_t pwmFrequencyInHz = 1000;

    pwmSourceClockInHz = PWM_SRC_CLK_FREQ;

    /* Set deadtime count, we set this to about 650ns */
    deadTimeVal = ((uint64_t)pwmSourceClockInHz * 650) / 1000000000;

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

