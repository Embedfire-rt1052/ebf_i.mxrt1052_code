#include "./pit/bsp_pit.h"




 /**
  * @brief  ����PIT��ʱ��
  * @param  ��
  * @retval ��
  */
void PIT_Configuration(void)
{

  /* ����PIT��ʼ���ṹ�� */
  pit_config_t pitConfig;

  /* ����PIT��ʱ��ʱ�� OSC_CLK*/
  CLOCK_SetMux(kCLOCK_PerclkMux, 1U);
  /* ���� PERCLK_CLK ʱ�ӷ�ƵΪ 1 */
  CLOCK_SetDiv(kCLOCK_PerclkDiv, 0U);
  
  /* ��ʼ��PIT����ģʽ */
  PIT_GetDefaultConfig(&pitConfig);
  PIT_Init(PIT, &pitConfig);

  /* ���ü�������*/
  PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(PIT_TIMER_us, PIT_SOURCE_CLOCK));
    
}

/**
* @brief ����PITָ��ͨ���ļ�����
* @param  ��
* @retval ��    
*/
void PIT_start_timer(void)
{
  PIT_StartTimer(PIT_BASE_ADDR, PIT_CHANNEL_NUMBER);
}

