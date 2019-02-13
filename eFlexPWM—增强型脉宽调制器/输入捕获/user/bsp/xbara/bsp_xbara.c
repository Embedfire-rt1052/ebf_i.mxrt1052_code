#include "./bsp/xbara/bsp_xbara.h"


 /**
  * @brief  初始化XBARA
  * @param  无
  * @retval 无
  */
void XBARA_Configuration(void)
{
  /* 初始化XBARA1模块*/
  XBARA_Init(DEMO_XBARA_BASE);

  /*建立输入与输出信号之间的联系*/
  XBARA_SetSignalsConnection(DEMO_XBARA_BASE, DEMO_XBARA_INPUT_PITCH0, DEMO_XBARA_OUTPUT_ADC_ETC);
}

