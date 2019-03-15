#include "./bsp/nvic/bsp_nvic.h"
#include "./bsp/pit/bsp_pit.h"
#include "./bsp/led/bsp_led.h"

unsigned int k = 0;

/**
  * @brief  PIT定时器初始化
  * @param  无
  * @retval 无
  */
void PIT_TIMER_Init(void)
{
  pit_config_t pitConfig; //定义PIT初始化结构体
  
  /* 设置PIT定时器时钟 OSC_CLK*/
  CLOCK_SetMux(kCLOCK_PerclkMux, 1U);
  /* 设置 PERCLK_CLK 时钟分频为 1 */
  CLOCK_SetDiv(kCLOCK_PerclkDiv, 0U);

  /*获得PIT定时器默认配置参数*/
  PIT_GetDefaultConfig(&pitConfig);
  /*初始化PIT */
  PIT_Init(PIT, &pitConfig);
  
  /* 设置PIT定时器通道0自动重装载值 */
  PIT_SetTimerPeriod(PIT, PIT_CHANNEL_X, USEC_TO_COUNT(TIME_0, PIT_SOURCE_CLOCK));
  
  /*清除通道0的中断标志位*/
  PIT_ClearStatusFlags(PIT, PIT_CHANNEL_X, kPIT_TimerFlag);
  /* 使能通道0的计时完成中断 */
  PIT_EnableInterrupts(PIT, PIT_CHANNEL_X, kPIT_TimerInterruptEnable);
  
  /*设置中断优先级,*/
  set_IRQn_Priority(PIT_IRQ_ID,Group4_PreemptPriority_6, Group4_SubPriority_0);
  /* 使能PIT定时器中断 */
  EnableIRQ(PIT_IRQ_ID);
}


/**
  * @brief  PIT中断服务函数
  * @param  无
  * @retval 无
  */
void PIT_LED_HANDLER(void)
{
  k++;
  /* 清除中断标志位.*/
  PIT_ClearStatusFlags(PIT, PIT_CHANNEL_X, kPIT_TimerFlag);
  
  if(0 == (k/2))
  {
    RGB_RED_LED_ON; //RGB led 灯红灯亮
  }
  else
  {
    RGB_RED_LED_OFF;//RGB led 灯熄灭
  }
}


