#ifndef __TOUCHPAD_H_
#define	__TOUCHPAD_H_

#include "fsl_common.h"
 
/*********************************************************
 * TPAD ��ʱ����ͨ�������벶ץ���˿ڡ����źź궨��
 *********************************************************/
#define TPAD_QTMR_BASEADDR          			TMR3
#define TPAD_QTMR_INPUT_CAPTURE_CHANNEL  	kQTMR_Channel_3
#define TPAD_QTMR_CounterInputPin        	kQTMR_Counter3InputPin
#define TPAD_QTMR_IOMUXC				  				IOMUXC_GPIO_AD_B1_03_QTIMER3_TIMER3

#define TPAD_GPIO 				    						GPIO1
#define TPAD_GPIO_PIN 		    						(19U)
#define TPAD_IOMUXC			      						IOMUXC_GPIO_AD_B1_03_GPIO1_IO19

/* �жϺź��жϴ������ */
#define TPAD_QTMR_IRQ_ID 									TMR3_IRQn
#define TPAD_QTMR_IRQ_HANDLER 						TMR3_IRQHandler

/* ��ȡQTMRʱ��Դ */
#define TPAD_QTMR_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_IpgClk)

/*********************************************************
 * ��������
 *********************************************************/
uint8_t TPAD_Init(void);
uint8_t TPAD_Scan(uint8_t mode);
 

#endif /* __TOUCHPAD_H_ */

