#include "fsl_gpio.h"
#include "pad_config.h"
#include "fsl_debug_console.h"

#include "./bsp/tmr/bsp_tmr.h"


/**
* @brief  配置TMR相关引脚功能
* @param  无
* @retval 无
*/
void TMR_GPIO_Config(void)
{
  /*定义GPIO引脚配置结构体*/
  gpio_pin_config_t gpt_config; 
  
  /*配置初始化结构体*/
  gpt_config.direction = kGPIO_DigitalOutput; //输出模式
  gpt_config.outputLogic =  1;                //默认高电平
  gpt_config.interruptMode = kGPIO_NoIntmode; //不使用中断
  
  /*初始化QTMR定时器CH0 外部引脚*/
  IOMUXC_SetPinMux(QTMR_CH0_IOMUXC, 0U);
  IOMUXC_SetPinConfig(QTMR_CH0_IOMUXC, TMR_PWM_OUTPUT_PAD_CONFIG_DATA);
  GPIO_PinInit(QTMR_CH0_GPIO, QTMR_CH0_GPIO_PIN, &gpt_config);
  
  /*初始化QTMR定时器CH1 外部引脚*/
  IOMUXC_SetPinMux(QTMR_CH1_IOMUXC, 0U);
  IOMUXC_SetPinConfig(QTMR_CH1_IOMUXC, TMR_PWM_OUTPUT_PAD_CONFIG_DATA);
  GPIO_PinInit(QTMR_CH1_GPIO, QTMR_CH1_GPIO_PIN, &gpt_config);
  
  /*初始化QTMR定时器CH2 外部引脚*/
  IOMUXC_SetPinMux(QTMR_CH2_IOMUXC, 0U);
  IOMUXC_SetPinConfig(QTMR_CH2_IOMUXC, TMR_PWM_OUTPUT_PAD_CONFIG_DATA);
  GPIO_PinInit(QTMR_CH2_GPIO, QTMR_CH2_GPIO_PIN, &gpt_config);
  
  /*初始化QTMR定时器CH3 外部引脚*/
  IOMUXC_SetPinMux(QTMR_CH3_IOMUXC, 0U);
  IOMUXC_SetPinConfig(QTMR_CH3_IOMUXC, TMR_PWM_OUTPUT_PAD_CONFIG_DATA);
  GPIO_PinInit(QTMR_CH3_GPIO, QTMR_CH3_GPIO_PIN, &gpt_config);
}


/**
* @brief  配置TMR工作模式
* @param  无
* @retval 无
*/  
void TMR_Init(void)
{
  qtmr_config_t qtmrConfig; /*定义TMR 定时器初始化结构体*/
  
  /*初始化输入引脚*/
  TMR_GPIO_Config();
  
  /*初始化TMR 定时器*/
  QTMR_GetDefaultConfig(&qtmrConfig);

  qtmrConfig.primarySource = kQTMR_ClockDivide_8;
  QTMR_Init(QTMR_BASEADDR, QTMR_PWM_CHANNEL_0,  &qtmrConfig);
  /* 创建CH0的PWM输出，并且指定PWM的频率与占空比*/
  QTMR_SetupPwm(QTMR_BASEADDR, QTMR_PWM_CHANNEL_0, TMR3_CH0_PWM_FREQUENCY, TMR3_CH0_PWM_DUTYCYCLE, false, QTMR_SOURCE_CLOCK / 8);

  qtmrConfig.primarySource = kQTMR_ClockDivide_8;
  QTMR_Init(QTMR_BASEADDR, QTMR_PWM_CHANNEL_1,  &qtmrConfig); 
  /* 创建CH1的PWM输出，并且指定PWM的频率与占空比*/
  QTMR_SetupPwm(QTMR_BASEADDR, QTMR_PWM_CHANNEL_1, TMR3_CH1_PWM_FREQUENCY, TMR3_CH1_PWM_DUTYCYCLE, false, QTMR_SOURCE_CLOCK / 8);

  qtmrConfig.primarySource = kQTMR_ClockDivide_8;
  QTMR_Init(QTMR_BASEADDR, QTMR_PWM_CHANNEL_2,  &qtmrConfig);
  /* 创建CH2的PWM输出，并且指定PWM的频率与占空比*/
  QTMR_SetupPwm(QTMR_BASEADDR, QTMR_PWM_CHANNEL_2, TMR3_CH2_PWM_FREQUENCY, TMR3_CH2_PWM_DUTYCYCLE, false, QTMR_SOURCE_CLOCK / 8);

  qtmrConfig.primarySource = kQTMR_ClockDivide_128;
  QTMR_Init(QTMR_BASEADDR, QTMR_PWM_CHANNEL_3,  &qtmrConfig);
  /* 创建CH3的PWM输出，并且指定PWM的频率与占空比*/
  QTMR_SetupPwm(QTMR_BASEADDR, QTMR_PWM_CHANNEL_3, TMR3_CH3_PWM_FREQUENCY, TMR3_CH3_PWM_DUTYCYCLE, false, QTMR_SOURCE_CLOCK / 128);

  
  /* 开启定时器，并且设置在时钟的上升沿计数 */
  QTMR_StartTimer(QTMR_BASEADDR, QTMR_PWM_CHANNEL_0, kQTMR_PriSrcRiseEdge);
  QTMR_StartTimer(QTMR_BASEADDR, QTMR_PWM_CHANNEL_1, kQTMR_PriSrcRiseEdge);
  QTMR_StartTimer(QTMR_BASEADDR, QTMR_PWM_CHANNEL_2, kQTMR_PriSrcRiseEdge);
  QTMR_StartTimer(QTMR_BASEADDR, QTMR_PWM_CHANNEL_3, kQTMR_PriSrcRiseEdge);
  
}







