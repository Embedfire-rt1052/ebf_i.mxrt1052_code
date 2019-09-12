#include "./pit/bsp_pit.h"



/*!
* @brief 配置PIT定时器
*        配置PIT 触发ADC_ETC
*/
void PIT_Configuration(void)
{

  /* 定义PIT初始化结构体 */
  pit_config_t pitConfig;

  /* Init pit module */
  PIT_GetDefaultConfig(&pitConfig);
  PIT_Init(PIT, &pitConfig);

  /*  设置计数周期*/
  PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(PIT_TIMER_us, PIT_SOURCE_CLOCK));
    
}

/*!
* @brief 开启PIT指定通道的计数。
*        
*/
void PIT_start_timer(void)
{
  PIT_StartTimer(PIT_BASE_ADDR, PIT_CHANNEL_NUMBER);
}

