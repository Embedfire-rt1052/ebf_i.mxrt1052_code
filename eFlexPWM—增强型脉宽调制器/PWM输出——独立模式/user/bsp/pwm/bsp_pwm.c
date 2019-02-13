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
  /*设置引脚的 pad 属性 */
  IOMUXC_SetPinConfig(PWM1_PWMA00_IOMUXC, PWM_PAD_CONFIG_DATA);

  /*配置引脚为输出模式，默认电平位高电平*/
  PWM_pin_config.direction = kGPIO_DigitalOutput;
  PWM_pin_config.outputLogic = 1;
  PWM_pin_config.interruptMode = kGPIO_NoIntmode;
   
  GPIO_PinInit(PWM1_PWMA00_GPIO, PWM1_PWMA00_GPIO_PIN, &PWM_pin_config);

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
  

   /*设置pwm 错误输入为高电平，表示没有错误，只有当pwm 没有错误或者禁止错误检测才能正常输出pwm波*/
  XBARA_Init(XBARA1);
  XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1Fault0);
  XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1Fault1);
  XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1234Fault2);
  XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1234Fault3);

//  /*禁止错误检测*/
//  PWM1->SM[0].DISMAP[0]=0;

   
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
   pwmConfig.reloadLogic = kPWM_ReloadPwmFullCycle; //设置重载周期
   pwmConfig.pairOperation = kPWM_Independent;      //设置位独立模式 
   pwmConfig.enableDebugMode = true;                //使能 Debug 模式
   /* 初始化 PWM 并且判断初始化是否成功*/
   if (PWM_Init(BOARD_PWM_BASEADDR, kPWM_Module_0, &pwmConfig) == kStatus_Fail)
   {
       PRINTF("PWM initialization failed\n");        
   }

    pwmSignal.pwmChannel = kPWM_PwmA;    //选择PWM 通道 PWMA 或者PWMB
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
    PWM_SetupPwm(BOARD_PWM_BASEADDR, kPWM_Module_0, &pwmSignal, 1, kPWM_SignedEdgeAligned, PWM_frequency_Hz,
                 pwmSourceClockInHz);
    
    /*设置Set LDOK 位，将初始化参数加载到相应的寄存器*/
    PWM_SetPwmLdok(BOARD_PWM_BASEADDR,kPWM_Control_Module_0 , true);

    /* 开启PWM 输出*/
    PWM_StartTimer(BOARD_PWM_BASEADDR,  kPWM_Control_Module_0);

}










