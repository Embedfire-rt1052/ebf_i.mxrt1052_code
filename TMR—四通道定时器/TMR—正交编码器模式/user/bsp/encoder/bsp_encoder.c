#include "./encoder/bsp_encoder.h"
#include "./led/bsp_led.h"  


volatile uint64_t interrupt_counter = 0;


void Quadrature_Count_Init(void)
{

  qtmr_config_t qtmrConfig; /*定义TMR 定时器初始化结构体*/
  
  /*初始化TMR 定时器*/
  QTMR_GetDefaultConfig(&qtmrConfig);
  qtmrConfig.primarySource = kQTMR_ClockDivide_128;
  QTMR_Init(QTMR_BASEADDR_2, QTMR_CH0, &qtmrConfig);
  
  /*设置自动重装载值*/
  QTMR_SetTimerPeriod(QTMR_BASEADDR_2, QTMR_CH0, MSEC_TO_COUNT(TMR_TIMIER, (QTMR_SOURCE_CLOCK / 128)));

  /*使能中断*/
  EnableIRQ(QTMR2_IRQ_ID);
  
  /*使能比较中断*/
  QTMR_EnableInterrupts(QTMR_BASEADDR_2, QTMR_CH0, kQTMR_CompareInterruptEnable);
  
  /*开启通道2的计时，在时钟的上升沿计数*/
  QTMR_StartTimer(QTMR_BASEADDR_2, QTMR_CH0, kQTMR_PriSrcRiseEdge);

}


/*TMR定时器中断服务函数*/
void QTMR2_IRQ_HANDLER(void)
{
  interrupt_counter++;
  
  /*模拟编码器时序*/
  if(interrupt_counter % 2)
  {
    if((interrupt_counter/2)%2)
    {
      RGB_RED_LED_OFF
    }
    else
    {
      RGB_RED_LED_ON
    }
  }
  else
  {
    if((interrupt_counter/2)%2)
    {
      RGB_GREEN_LED_ON
    }
    else
    {
      RGB_GREEN_LED_OFF
    }
  }
    /* 清除中断标志位*/
  QTMR_ClearStatusFlags(QTMR_BASEADDR_2,QTMR_CH0, kQTMR_CompareFlag);

}








