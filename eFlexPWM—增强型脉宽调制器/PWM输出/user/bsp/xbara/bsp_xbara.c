#include "./bsp/xbara/bsp_xbara.h"


 /**
  * @brief  ��ʼ��XBARA
  * @param  ��
  * @retval ��
  */
void XBARA_Configuration(void)
{
  /* ��ʼ��XBARA1ģ��*/
  XBARA_Init(DEMO_XBARA_BASE);

  /*��������������ź�֮�����ϵ*/
  XBARA_SetSignalsConnection(DEMO_XBARA_BASE, DEMO_XBARA_INPUT_PITCH0, DEMO_XBARA_OUTPUT_ADC_ETC);
}

