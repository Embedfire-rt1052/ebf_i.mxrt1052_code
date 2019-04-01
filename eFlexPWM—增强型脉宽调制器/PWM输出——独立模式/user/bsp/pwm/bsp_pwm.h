#ifndef __BSP_PWM_H
#define __BSP_PWM_H

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#include "fsl_pwm.h"
#include "fsl_xbara.h"
#include "pad_config.h"



/*********************************************************
 * PWM1 ����˿ڡ����źż�IOMUXC���ú궨��
 
ʹ��i.MX RT1052-Pro�װ壺
 *  FLEXPWM1_PWMA0-----GPIO_SD_B0_00-----(CN5, 68)
 *  FLEXPWM1_PWMB0-----GPIO_SD_B0_01-----(CN5, 70)
 *  FLEXPWM1_PWMA1-----GPIO_SD_B0_02-----(CN5,65)
 *  FLEXPWM1_PWMA2-----GPIO_SD_B0_04-----(CN5,69)
 *���Ź���            ���ű��        �����ڿ������϶�Ӧλ��
 (CN5, 68) �������������ӵ�������CN5����ĵ�68��


ʹ��i.MX RT1052-Mini�װ�

 *  FLEXPWM1_PWMA0-----GPIO_SD_B0_00-----(CN4, 68)
 *  FLEXPWM1_PWMB0-----GPIO_SD_B0_01-----(CN4, 70)
 *  FLEXPWM1_PWMA1-----GPIO_SD_B0_02-----(CN4,65)
 *  FLEXPWM1_PWMA2-----GPIO_SD_B0_04-----(CN4,69)
 *���Ź���         ���ű��     �����ڿ������϶�Ӧλ��
 (CN4, 68) �������������ӵ�������CN4����ĵ�68�� 

 *********************************************************/
/* FLEXPWM1_PWMA0  (CN4, 68) */
#define PWM1_PWMA00_GPIO      GPIO3
#define PWM1_PWMA00_GPIO_PIN  (12U)
#define PWM1_PWMA00_IOMUXC    IOMUXC_GPIO_SD_B0_00_FLEXPWM1_PWMA00  


/* �궨�壬 ���� PWM ����ַ */   
#define BOARD_PWM_BASEADDR PWM1      //����ʹ�õ� PWM ģ��
/*�궨�壬 �õ�IpgCLKʱ��Ƶ��*/
#define PWM_SRC_CLK_FREQ CLOCK_GetFreq(kCLOCK_IpgClk)

/*�ܹ����õ�PWM��Χ��ʱ��Դ��ѡ��ʱ�ӷ�Ƶ�йأ��Ա�ʵ��Ϊ����
*ʱ��Ƶ�ʣ�132MHz
*ʱ�ӷ�Ƶ��1 ������Ƶ�ʣ�132MHz 
*�����Ĵ�����16λ��������ԼΪ65535
*���PWM ���Ƶ�� = 132000000/65535 Լ 2050Hz
*/

#define PWM_frequency_Hz 3000
#define PWM_duty_Cycle_Percent 50  //50 ��ʾռ�ձ�50%

/*******************************************************************************
 * uart��������
 ******************************************************************************/
#define PWM_PAD_CONFIG_DATA            (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_1_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_0_100K_OHM_PULL_DOWN| \
                                        HYS_0_HYSTERESIS_DISABLED) 
    /* ����˵�� : */
    /* ת������: ת��������
        ����ǿ��: R0/6 
        �������� : medium(100MHz)
        ��©����: �ر� 
        ��/����������: ʹ��������/������
        ��/������ѡ��: ������
        ����/����ѡ��: 100Kŷķ����(ѡ���˱�������������Ч)
        �ͻ�������: ��ֹ */


void PWM_gpio_config(void);
void PWM_config(void);
#endif /* __BSP_PWM_H */



