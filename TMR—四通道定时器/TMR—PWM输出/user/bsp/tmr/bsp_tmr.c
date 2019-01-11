#include "fsl_gpio.h"
#include "pad_config.h"
#include "./tmr/bsp_tmr.h"
#include "fsl_debug_console.h"

/**
* @brief  配置TMR相关引脚功能
* @param  无
* @retval 无
*/
void TMR_GPIO_Config(void)
{
    /*定义GPIO引脚配置结构体*/
  gpio_pin_config_t gpt_config; 
  
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_00_QTIMER3_TIMER0, 0U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_00_QTIMER3_TIMER0, TMR_INPUT_PAD_CONFIG_DATA);
  
  
  gpt_config.direction = kGPIO_DigitalOutput; //输入模式
  gpt_config.outputLogic =  1;                //默认高电平
  gpt_config.interruptMode = kGPIO_NoIntmode; //不使用中断
  
  /* 初始化  GPIO. */
  GPIO_PinInit(TMR_TIMER0_INPUT_GPIO, TMR_TIMER0_INPUT_GPIO_PIN, &gpt_config);
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
  QTMR_Init(BOARD_QTMR_BASEADDR, BOARD_QTMR_PWM_CHANNEL,  &qtmrConfig);

  /* 创建一个PWM输出，并且指定PWM的频率与占空比*/
  if((QTMR_SetupPwm(BOARD_QTMR_BASEADDR, BOARD_QTMR_PWM_CHANNEL, TMR3_CH0_PWM_FREQUENCY, TMR3_CH0_PWM_DUTYCYCLE, false, QTMR_SOURCE_CLOCK / 8)) != kStatus_Success)
  {
    while(1)
    {
      PRINTF("error");
    }
  }
  
  /* 开始计数，并且设置在时钟的上升沿计数 */
  QTMR_StartTimer(BOARD_QTMR_BASEADDR, BOARD_QTMR_PWM_CHANNEL, kQTMR_PriSrcRiseEdge);
}











