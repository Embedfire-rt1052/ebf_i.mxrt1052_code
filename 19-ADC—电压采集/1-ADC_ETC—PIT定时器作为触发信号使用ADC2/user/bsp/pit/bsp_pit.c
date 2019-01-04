#include "./pit/bsp_pit.h"




 /**
  * @brief  配置PIT定时器
  * @param  无
  * @retval 无
  */
void PIT_Configuration(void)
{

  /* 定义PIT初始化结构体 */
  pit_config_t pitConfig;

  /* 设置PIT定时器时钟 OSC_CLK*/
  CLOCK_SetMux(kCLOCK_PerclkMux, 1U);
  /* 设置 PERCLK_CLK 时钟分频为 1 */
  CLOCK_SetDiv(kCLOCK_PerclkDiv, 0U);
  
  /* 初始化PIT工作模式 */
  PIT_GetDefaultConfig(&pitConfig);
  PIT_Init(PIT, &pitConfig);

  /* 设置计数周期*/
  PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(PIT_TIMER_us, PIT_SOURCE_CLOCK));
    
}

/**
* @brief 开启PIT指定通道的计数。
* @param  无
* @retval 无    
*/
void PIT_start_timer(void)
{
  PIT_StartTimer(PIT_BASE_ADDR, PIT_CHANNEL_NUMBER);
}

