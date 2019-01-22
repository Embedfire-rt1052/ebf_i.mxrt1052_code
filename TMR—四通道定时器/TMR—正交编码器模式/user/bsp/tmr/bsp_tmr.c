#include "fsl_gpio.h"
#include "pad_config.h"
#include "fsl_debug_console.h"

#include "./bsp/nvic/bsp_nvic.h"
#include "./bsp/tmr/bsp_tmr.h"



void TMR_Quadrature_Count_GPIO_Init(void)
{
    /*定义GPIO引脚配置结构体*/
  gpio_pin_config_t gpt_config; 
  
  /*配置初始化结构体*/
  gpt_config.direction = kGPIO_DigitalInput; //输入模式
  gpt_config.interruptMode = kGPIO_NoIntmode; //不使用中断
  
  
   /*初始化QTMR定时器CH2 外部引脚*/
  IOMUXC_SetPinMux(QTMR_CH2_IOMUXC, 0U);
  IOMUXC_SetPinConfig(QTMR_CH2_IOMUXC, TMR_Quadrature_Count_PAD_CONFIG_DATA);
  GPIO_PinInit(QTMR_CH2_GPIO, QTMR_CH0_GPIO_PIN, &gpt_config);
  
  /*初始化QTMR定时器CH3 外部引脚*/
  IOMUXC_SetPinMux(QTMR_CH3_IOMUXC, 0U);
  IOMUXC_SetPinConfig(QTMR_CH3_IOMUXC, TMR_Quadrature_Count_PAD_CONFIG_DATA);
  GPIO_PinInit(QTMR_CH3_GPIO, QTMR_CH3_GPIO_PIN, &gpt_config);   
}



void TMR_Quadrature_Count_init(void)
{    
  
  qtmr_config_t qtmrConfig; /*定义TMR 定时器初始化结构体*/
 
  /*初始化外部输入引脚*/
  TMR_Quadrature_Count_GPIO_Init();
  
   /*初始化TMR 定时器*/
  QTMR_GetDefaultConfig(&qtmrConfig);
  qtmrConfig.primarySource = kQTMR_ClockCounter2InputPin;  //主计数源 选择定时器输入引脚2   
  qtmrConfig.secondarySource = kQTMR_Counter3InputPin;     //辅助计数源 选择定时器输入引脚3
  qtmrConfig.debugMode = kQTMR_RunNormalInDebug;           //在debug模式下定时器正常运行
  QTMR_Init(QTMR_BASEADDR, QTMR_PWM_CHANNEL_0, &qtmrConfig);
  
  /*开启定时器*/
  QTMR_StartTimer(QTMR_BASEADDR, QTMR_PWM_CHANNEL_0, kQTMR_QuadCountMode);


}


/**************************MR_Init end**************************/
