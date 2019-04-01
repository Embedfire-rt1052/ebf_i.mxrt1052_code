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

  /*设置引脚的 pad 属性 */
  IOMUXC_SetPinConfig(PWM1_PWMA00_IOMUXC, PWM_PAD_CONFIG_DATA);
  IOMUXC_SetPinConfig(PWM1_PWMB00_IOMUXC, PWM_PAD_CONFIG_DATA);

  /*配置引脚为输出模式，默认电平位高电平*/
  PWM_pin_config.direction = kGPIO_DigitalOutput;
  PWM_pin_config.outputLogic = 1;
  PWM_pin_config.interruptMode = kGPIO_NoIntmode;
   
  GPIO_PinInit(PWM1_PWMA00_GPIO, PWM1_PWMA00_GPIO_PIN, &PWM_pin_config);
  GPIO_PinInit(PWM1_PWMB00_GPIO, PWM1_PWMB00_GPIO_PIN, &PWM_pin_config);

}
  
 /**
* @brief  初始化 PWM  配置参数 
* @retval 无
*/
void PWM_config(void)
{
  uint32_t pwmSourceClockInHz;     //用于保存计数频率 
  uint16_t deadTimeVal = 0;        //用于保存死区计数值
  pwm_signal_param_t pwmSignal[2]; //用于设置 pwm 的参数，频率、周期等
  pwm_config_t pwmConfig;          //定义pwm 配置结构体
  
  


  /* 设置IPG clock 时钟频率，最高只能实现四分频*/
  CLOCK_SetDiv(kCLOCK_IpgDiv, 0x3); /* Set IPG PODF to 3, divede by 4 */
  

   /*设置pwm 错误输入为高电平，表示没有错误，只有当pwm 没有错误输入或者禁止错误检测才能正常输出pwm波*/
   XBARA_Init(XBARA1);
   XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1Fault0);
   XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1Fault1);
   XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1234Fault2);
   XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1234Fault3); 
          
   /*获取默认的配置，默认配置如下
    *pwmConfig->enableDebugMode = false;   //禁止在DEBUG模式
    *config->enableWait = false;           //禁止工作在等待模式
    *config->reloadSelect = kPWM_LocalReload;//PWM模块使用本地重载信号重载pwm
    *config->faultFilterCount = 0;          //故障筛选器计数设置为0
    *config->faultFilterPeriod = 0;         //故障滤波频率设置为0，不使用故障滤波
    *config->clockSource = kPWM_BusClock;   //选择时钟源
    *config->prescale = kPWM_Prescale_Divide_1;//设置时钟分频
    *config->initializationControl = kPWM_Initialize_LocalSync;//本地同步信号初始化PWM
    *config->forceTrigger = kPWM_Force_Local;  //选择强制更新信号
    *config->reloadFrequency = kPWM_LoadEveryOportunity;//选择PWM自动重装频率
    *config->reloadLogic = kPWM_ReloadImmediate;//一旦设置了LDOK位，缓存寄存器立即加载新的值 
    *config->pairOperation = kPWM_Independent;  //工作在独立模式
   */
   PWM_GetDefaultConfig(&pwmConfig);
   
   /* 修改默认配置参数 */
   pwmConfig.reloadLogic = kPWM_ReloadPwmFullCycle; //新值在上一个pwm周期输出结束之后加载到缓冲寄存器中
   pwmConfig.pairOperation = kPWM_ComplementaryPwmA;// PwmA 和 PwmB 作为互补通道，PwmA 作为主通道
   pwmConfig.prescale = kPWM_Prescale_Divide_128;
   pwmConfig.enableDebugMode = true;                // 使能DebugMode 
   
   /* 初始化 PWM 并且判断初始化是否成功*/
    if (PWM_Init(BOARD_PWM_BASEADDR, kPWM_Module_0, &pwmConfig) == kStatus_Fail)
    {
        PRINTF("PWM initialization failed\n");
    }
   
    /*获得当前计数频率*/
    pwmSourceClockInHz = PWM_SRC_CLK_FREQ;
    /* 将死区时间转换为pwm时钟源的时钟个数*/
    deadTimeVal = ((uint64_t)pwmSourceClockInHz * PWM_deadtime) / 1000000000;
   
    /*配置pwm 参数*/
    pwmSignal[0].pwmChannel = kPWM_PwmA; //指定pwm 通道
    pwmSignal[0].level = kPWM_HighTrue;  //设置输出极性 
    pwmSignal[0].dutyCyclePercent = PWM_duty_Cycle_Percent;  //设置 pwm 的占空比   
    pwmSignal[0].deadtimeValue = deadTimeVal; //设定死区值

    pwmSignal[1].pwmChannel = kPWM_PwmB;        
    pwmSignal[1].level = kPWM_HighTrue;
    pwmSignal[1].dutyCyclePercent = PWM_duty_Cycle_Percent;
    pwmSignal[1].deadtimeValue = deadTimeVal;
    

    
    /*设置pwm 参数，包括pwm 的频率 占空比，死区时间等*/
    PWM_SetupPwm(BOARD_PWM_BASEADDR, kPWM_Module_0, pwmSignal, 2, kPWM_SignedEdgeAligned, PWM_frequency_Hz,
                 pwmSourceClockInHz);

    /*设置Set LDOK 位，将初始化参数加载到相应的寄存器*/
    PWM_SetPwmLdok(BOARD_PWM_BASEADDR, kPWM_Control_Module_0 , true);

    /*开启pwm1 子模块0(Submodules 0) 的pwm输出*/
    PWM_StartTimer(BOARD_PWM_BASEADDR, kPWM_Control_Module_0 );
}

