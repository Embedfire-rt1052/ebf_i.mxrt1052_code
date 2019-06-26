#ifndef __485_H
#define __485_H


#include "pad_config.h" 
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"  

/*485 ���͡����տ������Ŷ���
*Ϊ�ߵ�ƽ������ʹ��
*Ϊ�͵�ƽ������ʹ��
*/

#define 485_RE_GPIO             GPIO1
#define 485_RE_GPIO_PIN         (26U)
#define 485_RE_IOMUXC           IOMUXC_GPIO_AD_B1_10_GPIO1_IO26




/* 485 ���͡����տ������ŵ�PAD���� */
#define 485_RE_PAD_CONFIG_DATA          (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_0_PULL_KEEPER_DISABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_0_100K_OHM_PULL_DOWN| \
                                        HYS_0_HYSTERESIS_DISABLED)   
    /* ����˵�� : 
      ת������: ת��������
      ����ǿ��: R0/6 
      �������� : medium(100MHz)
      ��©����: �ر� 
      ��/����������: �ر�
      ��/������ѡ��: �������������ѹرգ�������Ч��
      ����/����ѡ��: 100Kŷķ�����������ѹرգ�������Ч��*/

#endif /* __485_H */