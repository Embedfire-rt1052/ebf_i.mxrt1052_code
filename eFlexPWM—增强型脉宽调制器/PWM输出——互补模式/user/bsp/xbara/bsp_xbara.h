#ifndef __BSP_XBARTA_H
#define __BSP_XBARTA_H

#include "fsl_common.h"
#include "fsl_xbara.h"

/*���� XBARA1 */
#define DEMO_XBARA_BASE XBARA1 

/*���������ź�������ź�*/
#define DEMO_XBARA_INPUT_PITCH0   kXBARA1_InputPitTrigger0       //��ʱ������0
#define DEMO_XBARA_OUTPUT_ADC_ETC kXBARA1_OutputAdcEtcXbar1Trig3 //ADC_ETCͨ��2��������3�����ⲿ��������ͨ��7





void XBARA_Configuration(void);

#endif /* __BSP_XBARTA_H */

