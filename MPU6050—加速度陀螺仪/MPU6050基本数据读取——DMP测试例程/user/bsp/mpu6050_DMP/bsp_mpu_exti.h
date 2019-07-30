#ifndef __MPU_EXTI_H
#define	__MPU_EXTI_H

//#include "stm32f4xx.h"
/******************************************************************
 * ��
 ******************************************************************/
//WAUP����
#define MPU_EXTI_GPIO          GPIO1
#define MPU_EXTI_GPIO_PIN      (14U)
#define MPU_EXTI_IOMUXC        IOMUXC_GPIO_AD_B0_14_GPIO1_IO14
//�ж���أ�IRQ�жϺż�IRQHandler�жϷ�����
#define MPU_EXTI_IRQ           GPIO1_Combined_0_15_IRQn
#define MPU_EXTI_IRQHandler    GPIO1_Combined_0_15_IRQHandler


/* �����Ƿ񱻰��µ��жϱ�־ */
//extern __IO bool g_KeyDown[2];

void EXTI_MPU_Config(void);

#endif /* __EXTI_H */
