#include "fsl_gpio.h"
#include "pad_config.h"
#include "fsl_debug_console.h"

#include "./tmr/bsp_tmr.h"



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

void TMR_Quadrature_Count_init(TMR_Type *base, qtmr_channel_selection_t channel)
{    
  /*初始化外部输入引脚*/
  MR_Quadrature_Count_GPIO_Init();
  
  base->CHANNEL[channel].CTRL &= ~(0x0f<<9);  //选择主时钟源
  base->CHANNEL[channel].CTRL |=  (0x02<<9);
  
  base->CHANNEL[channel].CTRL &= ~(0x03<<7);  //选择辅助时钟源
  base->CHANNEL[channel].CTRL |= (0x03<<7);    
  
  
  /*设置SCTRL寄存器到复位值*/
  base->CHANNEL[channel].SCTRL &= 0X00;
  base->CHANNEL[channel].SCTRL |= 0x01;   
  
  /*设置定时器计数寄存器和计数寄存器的自动重装载值为0（复位值）*/
  base->CHANNEL[channel].CNTR = 0X00;
  base->CHANNEL[channel].LOAD = 0X00;
  
  /*设置两个比较寄存器的初始值*/
  base->CHANNEL[channel].COMP1 = 0XFFFF;
  base->CHANNEL[channel].COMP2 = 0X00;
  
  base->CHANNEL[channel].CSCTRL = 0X00;
  
  
  base->CHANNEL[channel].CTRL &= ~(0x07<<13);//清除工作模式选择位CTRL[CM]
  base->CHANNEL[channel].CTRL |= (0X04<<13);  //选择使用正交解码模式
  
  //QTMR_StartTimer(base, channel, kQTMR_PriSrcRiseEdge);

  /*使能中断*/
  EnableIRQ(QTMR3_IRQ_ID);   
  
  /*使能比较中断*/
  QTMR_EnableInterrupts(base, channel, kQTMR_CompareInterruptEnable);
}



/*TMR定时器中断服务函数*/
void QTMR3_IRQ_HANDLER(void)
{

  /* 清除中断标志位*/
  QTMR_ClearStatusFlags(QTMR_BASEADDR,QTMR_PWM_CHANNEL_0, kQTMR_CompareFlag);
  
}

/**************************MR_Init end**************************/
