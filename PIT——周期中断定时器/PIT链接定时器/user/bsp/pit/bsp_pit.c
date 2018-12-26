#include "./pit/bsp_pit.h"
#include "./led/bsp_led.h"

extern volatile bool pit_flag;
int k = 0;
void PIT_TIMER_Init(void)
{
  pit_config_t pitConfig;
  
  /*获得默认配置参数*/
  PIT_GetDefaultConfig(&pitConfig);
  /* Init pit module */
  PIT_Init(PIT, &pitConfig);
  
  /* Set timer period for channel 0 */
  PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(1000000U, PIT_SOURCE_CLOCK));
  
  /* Enable timer interrupts for channel 0 */
  PIT_EnableInterrupts(PIT, PIT_CHANNEL_X, kPIT_TimerInterruptEnable);
  
  /* Enable at the NVIC */
  EnableIRQ(PIT_IRQ_ID);
}



void PIT_LED_HANDLER(void)
{
  k++;
  /* Clear interrupt flag.*/
  PIT_ClearStatusFlags(PIT, PIT_CHANNEL_X, kPIT_TimerFlag);
  
  if(k%2)
  {
    RGB_RED_LED_ON;
  }
  else
  {
    RGB_RED_LED_OFF;
  }

    

}