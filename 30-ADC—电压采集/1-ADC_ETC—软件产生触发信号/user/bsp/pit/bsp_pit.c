#include "./pit/bsp_pit.h"



/*!
* @brief ����PIT��ʱ��
*        ����PIT ����ADC_ETC
*/
void PIT_Configuration(void)
{

  /* ����PIT��ʼ���ṹ�� */
  pit_config_t pitConfig;

  /* Init pit module */
  PIT_GetDefaultConfig(&pitConfig);
  PIT_Init(PIT, &pitConfig);

  /*  ���ü�������*/
  PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(PIT_TIMER_us, PIT_SOURCE_CLOCK));
    
}

/*!
* @brief ����PITָ��ͨ���ļ�����
*        
*/
void PIT_start_timer(void)
{
  PIT_StartTimer(PIT_BASE_ADDR, PIT_CHANNEL_NUMBER);
}

