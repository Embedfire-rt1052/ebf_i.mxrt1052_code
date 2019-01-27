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

 /**
* @brief  初始化 PWM  配置参数 
* @retval 无
*/
void PWM_config(void)
{
  uint32_t pwmSourceClockInHz;  //用于保存计数频率 
  uint16_t deadTimeVal = 0;     //用于保存死区计数值（），单通道输出没有用到死区，设为0即可
  pwm_config_t pwmConfig;       //定义pwm 初始化结构体 用于设置PWM工作模式
  pwm_signal_param_t pwmSignal; //用于设置 pwm 的参数，频率、周期等
  

  
  /*设置AHB总线时钟和IP总线时钟*/
  CLOCK_SetDiv(kCLOCK_AhbDiv, 0x2); /* Set AHB PODF to 2, divide by 3 */
  CLOCK_SetDiv(kCLOCK_IpgDiv, 0x3); /* Set IPG PODF to 3, divede by 4 */
  /*禁止错误输出*/
  PWM1->SM[0].DISMAP[0]=0;
  PWM1->SM[1].DISMAP[0]=0;
//  BOARD_PWM_BASEADDR->SM[kPWM_Control_Module_0].DISMAP[0]=0;

   
   /*配置 PWM 初始化参数*/
   PWM_GetDefaultConfig(&pwmConfig);
   pwmConfig.reloadLogic = kPWM_ReloadPwmFullCycle; //设置重载周期
   pwmConfig.pairOperation = kPWM_Independent;      //设置位独立模式 
   pwmConfig.enableDebugMode = true;                //使能 Debug 模式
   /* 初始化PWM1 的子模块0 （submodule 0） */
   if (PWM_Init(BOARD_PWM_BASEADDR, kPWM_Module, &pwmConfig) == kStatus_Fail)
   {
       PRINTF("PWM initialization failed\n");        
   }

    
    pwmSignal.pwmChannel = PWM_A_or_B;    //选择PWM 通道 PWMA 或者PWMB
    pwmSignal.level  =     kPWM_HighTrue; //选择输出极性
    pwmSignal.dutyCyclePercent = PWM_duty_Cycle_Percent;     //设置占空比   
    pwmSignal.deadtimeValue = deadTimeVal;//设置死区值，在该模式下没有使用到死区，设为0即可
    /*获得当前计数频率
    *注意:读取时钟频率之后要放在 pwm 初始化之后，即函数PWM_Init()之后
    *因为设置时钟分频之后读取到的频率才是PWM 计数频率，函数PWM_SetupPwm（）最后一个参数是pwm计数频率
    *而不是pwm模块时钟源的频率。PWM时钟源经过分频后得到pwm 计数频率
    */
    pwmSourceClockInHz = PWM_SRC_CLK_FREQ;
    /*设置pwm 参数*/
    PWM_SetupPwm(BOARD_PWM_BASEADDR, kPWM_Module, &pwmSignal, 1, kPWM_SignedEdgeAligned, PWM_frequency_Hz,
                 pwmSourceClockInHz);
    
    
   /* 设置从各自的缓冲区加载数据 */
   PWM_SetPwmLdok(BOARD_PWM_BASEADDR,kPWM_Control_Module , true);

   /* 开启PWM 输出*/
   PWM_StartTimer(BOARD_PWM_BASEADDR,  kPWM_Control_Module);

 
}










