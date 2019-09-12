#ifndef __BSP_PWM_H
#define __BSP_PWM_H

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#include "fsl_pwm.h"
#include "fsl_xbara.h"
#include "pad_config.h"



/*********************************************************
 * PWM1_PWMA00/PWM1_PWMB00  GPIO�˿ڡ����źż�IOMUXC���ú궨��
 
ʹ��i.MX RT1052-Pro�װ壺
 *  FLEXPWM1_PWMA00-----GPIO_SD_B0_00-----(CN5, 68)
 *  FLEXPWM1_PWMB00-----GPIO_SD_B0_01-----(CN5, 70)
 *  PWM1_PWMX0_GPIO-----GPIO_AD_B0_02-----(CN5, 14)
 *���Ź���               ���ű��        �����ڿ������϶�Ӧλ��
 (CN5, 70) �������������ӵ�������CN5����ĵ�70��

ʹ��i.MX RT1052-Mini�װ�

 *  FLEXPWM1_PWMA00-----GPIO_SD_B0_00-----(CN4, 68)
 *  FLEXPWM1_PWMB00-----GPIO_SD_B0_01-----(CN4, 70)
 *  PWM1_PWMX0_GPIO-----GPIO_AD_B0_02-----(CN4, 14)
 *   ���Ź���             ���ű��       �����ڿ������϶�Ӧλ��
 (CN4, 70) �������������ӵ�������CN4����ĵ�70�� 

 *********************************************************/
/* FLEXPWM1_PWMA00 */
#define PWM1_PWMA00_GPIO      GPIO3
#define PWM1_PWMA00_GPIO_PIN  (12U)
#define PWM1_PWMA00_IOMUXC    IOMUXC_GPIO_SD_B0_00_FLEXPWM1_PWMA00  

/* FLEXPWM1_PWMB00 */
#define PWM1_PWMB00_GPIO       GPIO3
#define PWM1_PWMB00_GPIO_PIN   (13U)
#define PWM1_PWMB00_IOMUXC     IOMUXC_GPIO_SD_B0_01_FLEXPWM1_PWMB00    

/*FLEXPWM1_PWMX0*/
#define PWM1_PWMX0_GPIO       GPIO1
#define PWM1_PWMX0_GPIO_PIN   (2U)
#define PWM1_PWMX0_IOMUXC     IOMUXC_GPIO_AD_B0_02_FLEXPWM1_PWMX00      
   


/* �궨�壬 ���� PWM ����ַ */
#define BOARD_PWM_BASEADDR PWM1
/*�궨�壬 �õ�IpgCLKʱ��Ƶ��*/
#define PWM_SRC_CLK_FREQ CLOCK_GetFreq(kCLOCK_IpgClk)

/*�ܹ����õ�PWM��Χ��ʱ��Դ��ѡ��ʱ�ӷ�Ƶ�йأ��Ա�ʵ��Ϊ����
*ʱ��Ƶ�ʣ�132MHz
*ʱ�ӷ�Ƶ��3 ������ʱ��Ƶ�ʣ�44MHz 
*�����Ĵ�����16λ��������ԼΪ65535
*���PWM ���Ƶ�� = 44000000/65535 Լ 680Hz
*/

#define PWM_frequency_Hz 1000
#define PWM_duty_Cycle_Percent 50  //50 ��ʾռ�ձ�50%
#define PWM_deadtime 65000   // ��λ ns (Ϊ�˱�����ʾ�����Ϲ۲죬����ʱ�����ýϴ�)

/*******************************************************************************
 * PWM1_PWMA00/PWM1_PWMB00 ��������
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


/* ���벶�����ŵ�PAD���� */
#define TMR_INPUT_PAD_CONFIG_DATA       (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_2_100K_OHM_PULL_UP| \
                                        HYS_0_HYSTERESIS_DISABLED)   
    /* ����˵�� : */
    /* ת������: ת��������
      ����ǿ��: R0/6 
      �������� : medium(100MHz)
      ��©����: �ر� 
      ��/����������: ����
      ��/������ѡ��: ������
      ����/����ѡ��: 100K����
      �ͻ�������: �ر� */ 



void PWM_gpio_config(void);
void Capture_gpio_config(void);
void PWM_config(void);
void Capture_config(void);
//static void PWM_DRV_Init3PhPwm(void);

#endif /* __BSP_PWM_H */



