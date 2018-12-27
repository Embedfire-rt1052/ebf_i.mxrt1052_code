#include "./pit/bsp_pit.h"
#include "./led/bsp_led.h"


unsigned int  k = 0;
void PIT_TIMER_Init(void)
{
  pit_config_t pitConfig;//定义PIT初始化结构体
  
  /*获得默认配置参数*/
  PIT_GetDefaultConfig(&pitConfig);
  /*初始化PIT */
  PIT_Init(PIT, &pitConfig);
  
  /* 设置PIT定时器通道2自动重装载值 */
  PIT_SetTimerPeriod(PIT, PIT_CHANNEL_2, TIME_2_COUNT);
   
  /*清除通道2的中断标志位*/
   PIT_ClearStatusFlags(PIT, PIT_CHANNEL_2, kPIT_TimerFlag);
  /* 使能通道2的计时完成中断 */
  PIT_EnableInterrupts(PIT, PIT_CHANNEL_2, kPIT_TimerInterruptEnable);
  
  /*PIT设置为定时器连接模式*/
  PIT_SetTimerChainMode(PIT, PIT_CHANNEL_2 , 1);
  

  /* 设置PIT定时器通道1自动重装载值 */
  PIT_SetTimerPeriod(PIT, PIT_CHANNEL_1, USEC_TO_COUNT(TIME_1, PIT_SOURCE_CLOCK));
  
  


  /* 使能PIT定时器中断 */
  EnableIRQ(PIT_IRQ_ID);
}



void PIT_LED_HANDLER(void)
{
  k++;
  
  /* 清除中断标志位.*/
  PIT_ClearStatusFlags(PIT, kPIT_Chnl_2, kPIT_TimerFlag);

  if(k%2)
  {
    RGB_RED_LED_ON;
  }
  else
  {
    RGB_RED_LED_OFF;
  }


}

