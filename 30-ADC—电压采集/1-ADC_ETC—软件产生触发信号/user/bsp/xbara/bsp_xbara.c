#include "./xbara/bsp_xbara.h"



void XBARA_Configuration(void)
{
    /* Init xbara module. */
    XBARA_Init(DEMO_XBARA_BASE);

    /*
     *Configure the XBARA signal connections. 
     * 设置DEMO_XBARA_INPUT_PITCH0（输入）与DEMO_XBARA_OUTPUT_ADC_ETC（输出）之间的关联
    */
    XBARA_SetSignalsConnection(DEMO_XBARA_BASE, DEMO_XBARA_INPUT_PITCH0, DEMO_XBARA_OUTPUT_ADC_ETC);
}





