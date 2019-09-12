#ifndef __TOUCHPAD_H_
#define	__TOUCHPAD_H_

#include "fsl_common.h"
 
/*********************************************************
 * TPAD ��ʱ����ͨ�������벶ץ���˿ڡ����źź궨��
 *********************************************************/

#define TPAD_QTMR_BASEADDR          		TMR3
#define TPAD_QTMR_INPUT_CAPTURE_CHANNEL  	kQTMR_Channel_3
#define TPAD_QTMR_CounterInputPin        	kQTMR_Counter3InputPin
#define TPAD_QTMR_IOMUXC			IOMUXC_GPIO_AD_B1_03_QTIMER3_TIMER3

#define TPAD_GPIO 				GPIO1
#define TPAD_GPIO_PIN 		    		(19U)
#define TPAD_IOMUXC			      	IOMUXC_GPIO_AD_B1_03_GPIO1_IO19


/* �жϺź��жϴ������ */
#define TPAD_QTMR_IRQ_ID 			TMR3_IRQn
#define TPAD_QTMR_IRQ_HANDLER 			TMR3_IRQHandler


/* ��ȡQTMRʱ��Դ */
#define TPAD_QTMR_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_IpgClk)


/* ���벶�����ŵ�PAD���� */
#define TMR_INPUT_PAD_CONFIG_DATA       (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_0_OUTPUT_DRIVER_DISABLED| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_1_OPEN_DRAIN_ENABLED| \
                                        PKE_0_PULL_KEEPER_DISABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_3_22K_OHM_PULL_UP| \
                                        HYS_1_HYSTERESIS_ENABLED)   
  /* ����˵�� : */
  /* ת������: ת��������
  ����ǿ��: �ر�
  �ٶ����� : medium(100MHz)
  ��©����: ʹ�� 
  ��/����������: �ر�
  ��/������ѡ��: ������ʹ��
  ����/����ѡ��: 22Kŷķ����
  �ͻ�������: ���� ��������ʱ��Ч��ʩ���ش�������ʹ�ܺ���Թ�������������*/


/* ���벶�����ŵ�PAD���� */
#define GPIO_PAD_CONFIG_DATA       (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_1_PULL_SELECTED| \
                                        PUS_1_47K_OHM_PULL_UP| \
                                        HYS_0_HYSTERESIS_DISABLED)   
    /* ����˵�� : */
    /* ת������: ת��������
      ����ǿ��: R0/6 
      �������� : medium(100MHz)
      ��©����: �ر� 
      ��/����������: ����
      ��/������ѡ��: ������
      ����/����ѡ��: 4.7K����
      �ͻ�������: �ر� */ 

        
        
/*********************************************************
 * ��������
 *********************************************************/
uint8_t TPAD_Init(void);
uint8_t TPAD_Scan(uint8_t mode);
 

#endif /* __TOUCHPAD_H_ */


