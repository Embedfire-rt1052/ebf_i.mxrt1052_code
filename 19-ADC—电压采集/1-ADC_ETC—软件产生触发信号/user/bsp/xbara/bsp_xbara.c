#include "./xbara/bsp_xbara.h"



void XBARA_Configuration(void)
{
    /* Init xbara module. */
    XBARA_Init(DEMO_XBARA_BASE);

    /*
     *Configure the XBARA signal connections. 
     * ����DEMO_XBARA_INPUT_PITCH0�����룩��DEMO_XBARA_OUTPUT_ADC_ETC�������֮��Ĺ���
    */
    XBARA_SetSignalsConnection(DEMO_XBARA_BASE, DEMO_XBARA_INPUT_PITCH0, DEMO_XBARA_OUTPUT_ADC_ETC);
}





