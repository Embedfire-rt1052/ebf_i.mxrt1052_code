#ifndef __BSP_XBARTA_H
#define __BSP_XBARTA_H

#include "fsl_common.h"
#include "fsl_xbara.h"

/*定义 XBARA1 */
#define DEMO_XBARA_BASE XBARA1 

/*定义输入信号与输出信号*/
#define DEMO_XBARA_INPUT_PITCH0   kXBARA1_InputPitTrigger0       //定时器触发0
#define DEMO_XBARA_OUTPUT_ADC_ETC kXBARA1_OutputAdcEtcXbar0Trig3 //ADC_ETC通道1触发输入3，即外部触发请求通道3

void XBARA_Configuration(void);

#endif /* __BSP_XBARTA_H */






/*定义 XBARA1 */
#define DEMO_XBARA_BASE XBARA1 

/*定义输入信号与输出信号*/
#define DEMO_XBARA_INPUT_PITCH0   kXBARA1_InputPitTrigger0       //定时器触发0
/*ADC_ETC通道1触发输入3，即外部触发请求通道3*/
#define DEMO_XBARA_OUTPUT_ADC_ETC kXBARA1_OutputAdcEtcXbar0Trig3 

