#ifndef __BEEP_H_
#define	__BEEP_H_

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "pad_config.h" 

/*********************************************************
 * BEEP GPIO�˿ڡ����źż�IOMUXC���ú궨��
 *********************************************************/
#define BOARD_BEEP_GPIO 		    GPIO1
#define BOARD_BEEP_GPIO_PIN 		    (3U)
#define BOARD_BEEP_IOMUXC	            IOMUXC_GPIO_AD_B0_03_GPIO1_IO03


/* ����BEEP����ʹ�õ�PAD���� */
#define BEEP_PAD_CONFIG_DATA            (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_0_PULL_KEEPER_DISABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_0_100K_OHM_PULL_DOWN| \
                                        HYS_0_HYSTERESIS_DISABLED)   
    /* ����˵�� : */
    /* ת������: ת��������
      ����ǿ��: R0/6 
      �������� : medium(100MHz)
      ��©����: �ر� 
      ��/����������: �ر�
      ��/������ѡ��: �������������ѹرգ�������Ч��
      ����/����ѡ��: 100Kŷķ�����������ѹرգ�������Ч��
      �ͻ�������: �ر� */  


/******************** BEEP �����궨�� **************************/
#define BEEP_ON   GPIO_PinWrite(BOARD_BEEP_GPIO, BOARD_BEEP_GPIO_PIN, 1U);
#define BEEP_OFF  GPIO_PinWrite(BOARD_BEEP_GPIO, BOARD_BEEP_GPIO_PIN, 0U);


/******************** BEEP ��������********************/
void Beep_GPIO_Config(void);


#endif /* __BEEP_H_ */
